/**  ---------------------------------------------------------------------------
-- Legal & Copyright:   (c) 2018 Kutleng Engineering Technologies (Pty) Ltd    - 
--                                                                             -
-- This program is the proprietary software of Kutleng Engineering Technologies-
-- and/or its licensors, and may only be used, duplicated, modified or         -
-- distributed pursuant to the terms and conditions of a separate, written     -
-- license agreement executed between you and Kutleng (an "Authorized License")-
-- Except as set forth in an Authorized License, Kutleng grants no license     -
-- (express or implied), right to use, or waiver of any kind with respect to   -
-- the Software, and Kutleng expressly reserves all rights in and to the       -
-- Software and all intellectual property rights therein.  IF YOU HAVE NO      -
-- AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, -
-- AND SHOULD IMMEDIATELY NOTIFY KUTLENG AND DISCONTINUE ALL USE OF THE        -
-- SOFTWARE.                                                                   -
--                                                                             -
-- Except as expressly set forth in the Authorized License,                    -
--                                                                             -
-- 1.     This program, including its structure, sequence and organization,    -
-- constitutes the valuable trade secrets of Kutleng, and you shall use all    -
-- reasonable efforts to protect the confidentiality thereof,and to use this   -
-- information only in connection with South African Radio Astronomy           -
-- Observatory (SARAO) products.                                               -
--                                                                             -
-- 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED     -
-- "AS IS" AND WITH ALL FAULTS AND KUTLENG MAKES NO PROMISES, REPRESENTATIONS  -
-- OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH       -
-- RESPECT TO THE SOFTWARE.  KUTLENG SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED-
-- WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A        -
-- PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET        -
-- ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE-
-- ENJOYMENT, QUIET POSSESSION USE OR PERFORMANCE OF THE SOFTWARE.             -
--                                                                             -
-- 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL KUTLENG OR -
-- ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT-
-- , OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO  -
-- YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF KUTLENG HAS BEEN       -
-- ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF -
-- THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR ZAR R1, WHICHEVER IS    -
-- GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF       -
-- ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.                                    -
-- --------------------------------------------------------------------------- -
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS                    -
-- PART OF THIS FILE AT ALL TIMES.                                             -
--=============================================================================-
-- Company          : Kutleng Dynamic Electronics Systems (Pty) Ltd            -
-- @author          : Benjamin Hector Hlophe                                   -
--                                                                             -
-- Design Name      : CASPER BSP                                               -
-- Module Name      : skarabveth                                               -
-- Project Name     : SKARAB2                                                  -
-- Target Devices   : Xilinx Virtex UltraScale+ FPGA                           -
-- Tool Versions    : XDMA V4.01 engine                                        -
-- @brief           : This kernel module provides an abstracted ethernet       -
--                    interface through PCIe.                                  -
-- @todo              Investigate using PCIe XVC extensions for FPGA functions -
--                    like:virtual jtag and partial reconfiguration over MCAP  -
--                                                                             -
-- Dependencies     : Adapted from the snull driver and Xilinx XDMA driver     -
-- @version $Revision History : V1.0 - Initial design                          -
----------------------------------------------------------------------------- */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/dma-mapping.h>
#include <linux/pagemap.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include "skarabveth.h"

#define DRIVER_MODULE_NAME			"skarabveth"
#define DRIVER_MODULE_DESCRIPTION	"SKARAB Virtual Ethernet Driver"
#define DRIVER_MODULE_RELEASE_DATE	"Jan. 2020"



MODULE_DESCRIPTION (DRIVER_MODULE_DESCRIPTION);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Benjamin Hlophe,Kutleng Engineering Technologies");
MODULE_VERSION("1.0"" "DRIVER_MODULE_RELEASE_DATE);

unsigned long   gHwRegisterBaseAddress;                    /**< Base register address (Hardware address)          */
unsigned long   gRegisterBaseLength;                       /**< Base register address Length                      */
void           *gVirtualRegisterBaseAddress = NULL;        /**< Base register address (Virtual address, for I/O). */
struct pci_dev *gPCIeDevice = NULL;     		           /**< PCI device structure.                             */
int             gPCIeIrq;       		        	       /**< IRQ assigned by PCI system.                       */
unsigned int    gStatFlags = 0x00;         	 			   /**< Status flags used for cleanup. */

static struct net_device *skarabveth;                      /**< Netowrk device handle. */

int skarabveth_rebuild_header(struct sk_buff *skb);

static int timeout = 5; 

#ifdef __VETH_DEBUG__
inline void __write_register(const char *fn, u32 value, void *iomem, unsigned long off)
{
	printk(KERN_ERROR"%s: w reg 0x%lx(0x%p), 0x%x.\n", fn, off, iomem, value);
	iowrite32(value, iomem);
}
#define write_register(v,mem,off) __write_register(__func__, v, mem, off)
#else
#define write_register(v,mem,off) iowrite32(v, mem)
#endif

inline u32 read_register(void *iomem)
{
	return ioread32(iomem);
}

/** 
 \fn int skarabveth_open(struct net_device *dev) 
 \details This function is called whenever the driver is loaded in memory.
 
 
 \param [inout] struct net_device *dev The network device handle
 
 \todo Perhaps put the probing of the device here?

 \note The virtual ethernet drivers will work if the XDMA module is found
       on the PCIe bus.
 */
static int skarabveth_open (struct net_device *dev)
{
	printk("skarabveth_open called\n");
	
	netif_start_queue(dev);
	return 0;
}

/** 
 \fn int skarabveth_release(struct net_device *dev) 
 \details This function is called whenever the driver is removed from memory.
 
 
 \param [inout] struct net_device *dev The network device handle
 */
static int skarabveth_release (struct net_device *dev)
{
	printk ("skarabveth_release called\n");
	netif_stop_queue(dev);
	return 0;
}


/** 
 \fn int skarabveth_xmit(struct sk_buff *skb,
                         struct net_device *dev) 
 \details This function is used for data transmission of ethernet packets.
 
 
 \param [in] struct sk_buff *skb The Linux ethernet packet buffer
 \param [inout] struct net_device *dev The network device handle
 */
static int skarabveth_xmit (struct sk_buff *skb, 
				struct net_device *dev)
{
	unsigned int tx_packet_length;
	struct skarabveth_priv *priv = netdev_priv(dev);
	
	tx_packet_length = skb->len;
	priv->stats.tx_bytes += tx_packet_length;
	priv->stats.tx_packets++;
	priv->stats.rx_bytes += tx_packet_length;
	priv->stats.rx_packets++;
	/** @todo : remove the packet loopback and free the SKB in the DMA chain
	 * after transmission. */
	//dev_kfree_skb(skb);
	netif_rx(skb);//Loop back the packet
	return 0;
}

/** 
 \fn int skarabveth_ioctl(struct net_device *dev,
						  struct ifreq *request,
						  int cmd) 
 \details This function is called by Linux FS for custom IO operations.
 
 
 \param [inout] struct net_device *dev The network device handle
 \param [in] struct ifreq *request The IOCTL request
 \param [in] int cmd Commands for the IOCTL operation
 */
int skarabveth_ioctl(struct net_device *dev, struct ifreq *request, int cmd)
{
	printk("skarabveth_ioctl called....\n");
	return 0;
}

/** 
 \fn int skarabveth_rebuild_header(struct sk_buff *skb)
 \details This function is called to fill up an eth header, 
          since arp is not supported on the interface
 
 \param [inout] struct sk_buff *skb The Linux ethernet packet buffer
 */
int skarabveth_rebuild_header(struct sk_buff *skb)
{
	struct ethhdr *eth = (struct ethhdr *) skb->data;
	struct net_device *dev = skb->dev;
    
	memcpy(eth->h_source, dev->dev_addr, dev->addr_len);
	memcpy(eth->h_dest, dev->dev_addr, dev->addr_len);
	eth->h_dest[ETH_ALEN-1]   ^= 0x01;   
	return 0;
}

/** 
 \fn int skarabveth_header(struct sk_buff *skb, struct net_device *dev,
						   unsigned short type, const void *daddr, 
						   const void *saddr, unsigned int len) 
 \details This function is called to assemble the ethernet header and fill
          it with the ethernet MAC address.
 
 \param [inout] struct sk_buff *skb The Linux ethernet packet buffer
 \param [inout] struct net_device *dev The network device handle
 \param [in] unsigned short type 
 \param [in] const void *daddr 
 \param [in] const void *saddr 
 \param [in] unsigned int len 
 */
int skarabveth_header(struct sk_buff *skb, struct net_device *dev,
                unsigned short type, const void *daddr, const void *saddr,
                unsigned int len)
{
	struct ethhdr *eth = (struct ethhdr *)skb_push(skb,ETH_HLEN);

	eth->h_proto = htons(type);
	memcpy(eth->h_source, saddr ? saddr : dev->dev_addr, dev->addr_len);
	memcpy(eth->h_dest,   daddr ? daddr : dev->dev_addr, dev->addr_len);
	eth->h_dest[ETH_ALEN-1]   ^= 0x01;   
	return (dev->hard_header_len);
}



/** 
 \fn int skarabveth_change_mtu(struct net_device *dev,
						  int vethernet_mtu) 
 \details This function is called by ethtool to change device MTU.
 
 
 \param [inout] struct net_device *dev The network device handle
 \param [in] int vethernet_mtu The new MTU for the interface
 */
int skarabveth_change_mtu(struct net_device *dev, int vethernet_mtu)
{
	unsigned long flags;
	struct skarabveth_priv *priv = netdev_priv(dev);
	spinlock_t *lock = &priv->lock;
    
	/* check ranges */
	if ((vethernet_mtu < 68) || (vethernet_mtu > 1500))
		return -EINVAL;
	spin_lock_irqsave(lock, flags);
	dev->mtu = vethernet_mtu;
	spin_unlock_irqrestore(lock, flags);
	return 0; 
}

/** 
 \fn int skarabveth_tx_timeout(struct net_device *dev),
 
 \details This function is called by Linux when there is a TX timeout.
 
 
 \param [inout] struct net_device *dev The network device handle
 */
void skarabveth_tx_timeout (struct net_device *dev)
{
	struct skarabveth_priv *priv = netdev_priv(dev);
	printk("Transmit timeout at %ld\n", jiffies);
	priv->stats.tx_errors++;
	netif_wake_queue(dev);
	return;
}

/** 
 \fn int skarabveth_config(struct net_device *dev,
						  struct ifmap *map) 
 \details This function is called by ifconfig to make configuration
          changes to the device.
 
 
 \param [inout] struct net_device *dev The network device handle
 \param [in] struct ifmap *map The changes for the interface
 */
int skarabveth_config(struct net_device *dev, struct ifmap *map)
{
	if (dev->flags & IFF_UP) /* can't act on a running interface */
		return -EBUSY;

	/* Don't allow changing the I/O address */
	if (map->base_addr != dev->base_addr) {
		printk(KERN_WARNING "skarabveth: Can't change I/O address\n");
		return -EOPNOTSUPP;
	}

	/* Allow changing the IRQ */
	if (map->irq != dev->irq) {
		dev->irq = map->irq;
        	/* request_irq() is delayed to open-time */
	}

	/* ignore other fields */
	return 0;
}


/** 
 \fn struct net_device_stats * skarabveth_stats(struct net_device *dev),
 
 \details This is a helper function to return interface statistics.
 
 
 \param [inout] struct net_device *dev The network device handle
 */
struct net_device_stats *skarabveth_stats(struct net_device *dev)
{
	struct skarabveth_priv *priv = netdev_priv(dev);
	return &priv->stats;
}

static const struct header_ops skarabveth_header_ops = {
   .create  = skarabveth_header,
   .parse = NULL, //skarabveth_rebuild_header,
   .cache   = NULL,  /* disable caching */
};

static const struct net_device_ops skarabveth_netdev_ops = {
	.ndo_open = skarabveth_open,
	.ndo_stop = skarabveth_release,
	.ndo_init = NULL,
	.ndo_set_config = skarabveth_config,
	.ndo_start_xmit = skarabveth_xmit,
	.ndo_do_ioctl = skarabveth_ioctl,
	.ndo_get_stats = skarabveth_stats,
	.ndo_change_mtu = skarabveth_change_mtu,
	.ndo_tx_timeout = skarabveth_tx_timeout,
};


/** 
 \fn irqreturn_t skarabveth_irq_handler(int irq, void *dev_id)
 
 \details This legacy IRQ handler function.
 
 
 \param [inout] int irq The interupt id
 \param [inout] void *dev_id The network device handle
 */
irqreturn_t skarabveth_irq_handler(int irq, void *dev_id)
{
	trace_printk("PCIe XDMA Legacy-PCIe-Shared-IRQ called %d\n",irq);
	return 0;
}
/** 
 \fn irqreturn_t skarabveth_irq_msi_handler(int irq, void *dev_id)
 
 \details This MSI IRQ handler function.
 
 
 \param [inout] int irq The interupt id
 \param [inout] void *dev_id The network device handle
 */
irqreturn_t skarabveth_irq_msi_handler(int irq, void *dev_id)
{
	trace_printk("PCIe XDMA MSI_N-IRQ called irq = %d \n",irq);

	return IRQ_HANDLED;
//	return IRQ_RETVAL(1);
}

/** 
 \fn void skarabveth_init(struct net_device *dev)
 
 \details This is a helper function to initialise the interface.
 
 
 \param [inout] struct net_device *dev The network device handle
 */
void skarabveth_init (struct net_device *dev)
{
	struct skarabveth_priv *priv;
	
	priv=netdev_priv(dev);
	
	memset(priv,0,sizeof(struct skarabveth_priv));
	
	spin_lock_init(&priv->lock);
	
	priv->dev = dev;

	ether_setup(dev); /* assign some of the fields */
	
	dev->netdev_ops = &skarabveth_netdev_ops;
	dev->header_ops	= &skarabveth_header_ops;
	dev->watchdog_timeo = timeout;
	/* keep the default flags, just add NOARP */
	dev->flags	|= IFF_NOARP;	
	printk ("skarabveth device initialized\n");
}


/** 
 \fn void skarabveth_init_module(void)
 
 \details This the kernel module initialise functionfor the interface.
 
 
 */
int skarabveth_init_module (void)
{
	int result;
	/**
	  Find the Xilinx EP device.  
	  The device is found by matching device and vendor ID's which is 
	  defined at the top of this file. Be default, the driver will
	  look for 10EE & 903f.
	  If the core is generated with other settings, the defines at the
	  top must be changed or the driver will not load
	 */
	gPCIeDevice = pci_get_device (PCI_VENDOR_ID_XILINX,
								  PCI_XDMA_DEVICE_ID_XILINX_PCIE,
								  gPCIeDevice);
	if (NULL == gPCIeDevice)
	{
		/** If a matching device or vendor ID is not found,
		  return failure and update kernel log.
		  NOTE: In fedora systems, the kernel log is 
		  located at: /var/log/messages
		 */
		printk(KERN_ERR"%s: skarabveth_init_module: Error PCIe Device Vedor[%04x]:[%04x] not found.\n", 
			   DRIVER_MODULE_NAME,PCI_VENDOR_ID_XILINX,PCI_XDMA_DEVICE_ID_XILINX_PCIE);
		return (ERROR);
	}
	
	/**
	 PCIe XDMA Bus Master Enable
	 */
	 
	if (0 > pci_enable_device(gPCIeDevice))
	{
		printk(KERN_ERR"%s: skarabveth_init_module: Could not enable XDMA to be PCIe bus master.\n", DRIVER_MODULE_NAME);		
		return (ERROR);
	}

	/**
	 PCIe Set DMA Mask to enable 64 bit 
	 */
	
    if (pci_set_dma_mask(gPCIeDevice, DMA_BIT_MASK(64)) || pci_set_consistent_dma_mask(gPCIeDevice, DMA_BIT_MASK(64)))
	{
        printk(KERN_ERR"%s: skarabveth_init_module: Error could not set pci dma mask.\n", DRIVER_MODULE_NAME);
		return (ERROR);
    }
    
	
	/**
	  Get Base Address of registers from pci structure. 
	  Should come from pci_dev structure, but that element seems to
	  be missing on the development system.
	 */
	gHwRegisterBaseAddress = pci_resource_start (gPCIeDevice, 0);

	if (0 > gHwRegisterBaseAddress)
	{
		printk(KERN_WARNING"%s: skarabveth_init_module: Error Base Address not set.\n", DRIVER_MODULE_NAME);
		return (ERROR);
	}     

	/**
	  Print Base Address to kernel log
	 */
	printk(KERN_ALERT"%s: skarabveth_init_module : PCIe Base Address hw val =0X%08X\n", DRIVER_MODULE_NAME, (unsigned int)gHwRegisterBaseAddress);
	
	/**
	  Get the Base Address Length
	 */
	gRegisterBaseLength = pci_resource_len(gPCIeDevice, 0);
	
	/**
	  Print the Base Address Length to Kernel Log
	 */
	printk(KERN_ALERT"%s: skarabveth_init_module :  Register Base Length %d\n", DRIVER_MODULE_NAME, (unsigned int)gRegisterBaseLength);
	
	/**
	  Remap the I/O register block so that it can be safely accessed.
	  I/O register block starts at gBaseHdwrIR and is 32 bytes long.
	  It is cast to char because that is the way Linus does it.
	  Reference "/usr/src/Linux-2.4/Documentation/IO-mapping.txt".
	 */
	gVirtualRegisterBaseAddress = pci_iomap(gPCIeDevice, 0, gRegisterBaseLength);
	if (!gVirtualRegisterBaseAddress)
	{
		printk(KERN_WARNING"%s: skarabveth_init_module : PCIe could not remap memory.\n", DRIVER_MODULE_NAME);
		return (ERROR);
	} 
	
	/**
	  Print out the aquired virtual base addresss
	 */
	printk(KERN_ERR"%s: skarabveth_init_module: Virt HW address %p\n", DRIVER_MODULE_NAME, gVirtualRegisterBaseAddress);

	/**
	  Get IRQ from pci_dev structure. It may have been remapped 
	  by the kernel, and this value will be the correct one.
	 */
	gPCIeIrq = gPCIeDevice->irq;
	printk(KERN_ALERT"%s: skarabveth_init_module: PCIe Device IRQ: %X\n",DRIVER_MODULE_NAME, gPCIeIrq);

	/**---START: Initialize Hardware */

	/**
	  Try to gain exclusive control of memory for demo hardware.
	 */
	request_mem_region(gHwRegisterBaseAddress, BAR_APPERTURE_SIZE, "kutlengvdmadrv-xdma");
	/**
	  Update flags
	 */

	gStatFlags = gStatFlags | HAVE_MEM_REGION;
	
	printk(KERN_ALERT"%s: skarabveth_init_module: Initialize Hardware Done..\n",DRIVER_MODULE_NAME);
	
	/**
	  Request IRQ from OS.
	  In past architectures, the SHARED and SAMPLE_RANDOM flags 
	  were called: SA_SHIRQ and SA_SAMPLE_RANDOM respectively.
	  In older Fedora core installations, the request arguments may
	  need to be reverted back. SA_SHIRQ | SA_SAMPLE_RANDOM
	 */
	printk(KERN_INFO"%s: ISR Setup..\n", DRIVER_MODULE_NAME);

	result = pci_enable_msi(gPCIeDevice);

	if (0 > result) 
	{
		printk(KERN_ERR"%s: skarabveth_init_module: Unable to initialise MSI IRQ",DRIVER_MODULE_NAME);
		printk(KERN_ERR"%s: skarabveth_init_module: Allocating legacy shared IRQ",DRIVER_MODULE_NAME);

		result = request_irq(gPCIeIrq,skarabveth_irq_handler , IRQF_SHARED , DRIVER_MODULE_NAME"-XDMA-SHR-Intr", gPCIeDevice);

		if (0 > result)
		{
			printk(KERN_ERR"%s: skarabveth_init_module: Unable to allocate legacy shared IRQ",DRIVER_MODULE_NAME);
		}else
		{


		   /**
			 Update flags stating IRQ was successfully obtained
			*/
			printk(KERN_ERR"%s: skarabveth_init_module: Allocated legacy shared IR IRQ",DRIVER_MODULE_NAME);
			gStatFlags = gStatFlags | HAVE_IRQ;
		}

	}else
	{
		
	
	   /**
		 Update MSI enable was successful
		*/
		printk(KERN_ERR"%s: skarabveth_init_module: Enabled N MSI IRQ Vectors",DRIVER_MODULE_NAME);
		gStatFlags = gStatFlags | HAVE_MSI_IRQ;

		result = request_irq(gPCIeDevice->irq, skarabveth_irq_msi_handler, 0 , DRIVER_MODULE_NAME"-XDMA-MSI_N-Intr", gPCIeDevice);

		if (0 > result)
		{
			printk(KERN_ERR"%s: skarabveth_init_module: Unable to allocate MSI_N_IRQ",DRIVER_MODULE_NAME);
		}else
		{


		   /**
			 Update flags stating IRQ was successfully obtained
			*/
			printk(KERN_ERR"%s: skarabveth_init_module: Allocated MSI_N_IRQ",DRIVER_MODULE_NAME);
			gStatFlags = gStatFlags | HAVE_MSI_N_IRQ;
		}

	}
	
	pci_set_master(gPCIeDevice);
	{
		printk(KERN_ERR"%s: skarabveth_init_module: PCIe Device Bus Mastering set.\n", DRIVER_MODULE_NAME);
	}
	
	/** 
	 Allocate the netdev structure in memory	  
	 */
	skarabveth = alloc_netdev(sizeof(struct skarabveth_priv),"veth%d",NET_NAME_UNKNOWN,skarabveth_init);
	
	if ((result = register_netdev (skarabveth))) {
		printk(KERN_ERR"skarabveth: Error %d  initializing card skarabveth card",result);
		/* Deallocate all the PCIe device structures and undo all the memory allocations */
		
		return result;
	}
	printk ("skarabveth device registered\n");
	return SUCCESS;
}
	
/** 
 \fn void skarabveth_cleanup(void)
 
 \details This the kernel module uninitialise functionfor the interface.
 
 
 */
void skarabveth_cleanup (void)
{
	/**
	  Check if we have a memory region and free it
	 */	 
	
	if (gStatFlags & HAVE_MEM_REGION)
	{
		(void) release_mem_region(gHwRegisterBaseAddress, BAR_APPERTURE_SIZE);
	}
	
    /**
      Check if we have an MSI IRQs and free it
     */
    if (gStatFlags & HAVE_MSI_IRQ)
    {
    	if (gStatFlags & HAVE_MSI_N_IRQ)  (void) free_irq(gPCIeDevice->irq, gPCIeDevice);
    	pci_disable_msi(gPCIeDevice);
    }
	
    /**
      Check if we have an IRQ and free it
     */
    if (gStatFlags & HAVE_IRQ)
    {
        (void) free_irq(gPCIeIrq, gPCIeDevice);
    }
	
    /**
      Free up memory pointed to by virtual address
     */	
    if (gVirtualRegisterBaseAddress != NULL)
    {
        iounmap(gVirtualRegisterBaseAddress);
    }
    
    gVirtualRegisterBaseAddress = NULL;

    gStatFlags = 0;	
	printk(KERN_WARNING"Cleaning Up the Module\n");
	unregister_netdev (skarabveth);
	free_netdev(skarabveth);
	return;
}
	
module_init (skarabveth_init_module);
module_exit (skarabveth_cleanup);
