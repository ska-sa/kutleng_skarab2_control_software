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
#include <linux/aer.h>

#include "skarabveth.h"
#include "libxdma.h"
#include "libxdma_api.h"

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

static struct 	net_device *skarabveth;                    /**< Netowrk device handle. */

int skarabveth_rebuild_header(struct sk_buff *skb);

static int timeout = 5; 
/* SECTION: Module global variables */
static int xpdev_cnt = 0;

static const struct pci_device_id xilinx_dma_pci_ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_XILINX, PCI_XDMA_DEVICE_ID_XILINX_PCIE), },
	{0,}
};

MODULE_DEVICE_TABLE(pci, xilinx_dma_pci_ids);



/** 
 \fn int skarabveth_open(struct net_device *dev) 
 \details This function is called whenever the driver is loaded in memory.
 
 
 \param [inout] struct net_device *dev The network device handle
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Ethernet header length
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
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Error and status value
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
 \return  Device statistics
 */
struct net_device_stats *skarabveth_stats(struct net_device *dev)
{
	struct skarabveth_priv *priv = netdev_priv(dev);
	return &priv->stats;
}

static void xpdev_free(struct xdma_pci_dev *xpdev)
{
	struct xdma_dev *xdev = xpdev->xdev;

	pr_info("xpdev 0x%p, xdev 0x%p xdma_device_close.\n", xpdev, xdev);
	xdma_device_close(xpdev->pdev, xdev);
	xpdev_cnt--;

	kfree(xpdev);
}

static struct xdma_pci_dev *xpdev_alloc(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev = kmalloc(sizeof(*xpdev), GFP_KERNEL);	

	if (!xpdev)
		return NULL;
	memset(xpdev, 0, sizeof(*xpdev));

	xpdev->magic = MAGIC_DEVICE;
	xpdev->pdev = pdev;
	xpdev->user_max = MAX_USER_IRQ;
	xpdev->h2c_channel_max = XDMA_CHANNEL_NUM_MAX;
	xpdev->c2h_channel_max = XDMA_CHANNEL_NUM_MAX;

	xpdev_cnt++;
	return xpdev;
}

static int probe_xilinx_fpga_pcie_card(struct pci_dev *pdev, const struct pci_device_id *id)
{
	int rv = 0;
	struct xdma_pci_dev *xpdev = NULL;
	struct xdma_dev *xdev;
	void *hndl;

	xpdev = xpdev_alloc(pdev);
	if (!xpdev)
		return -ENOMEM;

	hndl = xdma_device_open(DRIVER_MODULE_NAME, pdev, &xpdev->user_max,
			&xpdev->h2c_channel_max, &xpdev->c2h_channel_max);
	if (!hndl)
		return -EINVAL;

	BUG_ON(xpdev->user_max > MAX_USER_IRQ);
	BUG_ON(xpdev->h2c_channel_max > XDMA_CHANNEL_NUM_MAX);
	BUG_ON(xpdev->c2h_channel_max > XDMA_CHANNEL_NUM_MAX);

	if (!xpdev->h2c_channel_max && !xpdev->c2h_channel_max)
		pr_warn("NO engine found!\n");

	if (xpdev->user_max) {
		u32 mask = (1 << (xpdev->user_max + 1)) - 1;

		rv = xdma_user_isr_enable(hndl, mask);
		if (rv)
			goto err_out;
	}

	/* make sure no duplicate */
	xdev = xdev_find_by_pdev(pdev);
	if (!xdev) {
		pr_warn("NO xdev found!\n");
		return -EINVAL;
	}
	BUG_ON(hndl != xdev );

	pr_info("%s xdma%d, pdev 0x%p, xdev 0x%p, 0x%p, usr %d, ch %d,%d.\n",
		dev_name(&pdev->dev), xdev->idx, pdev, xpdev, xdev,
		xpdev->user_max, xpdev->h2c_channel_max,
		xpdev->c2h_channel_max);

	xpdev->xdev = hndl;

    dev_set_drvdata(&pdev->dev, xpdev);

	return 0;

err_out:	
	pr_err("pdev 0x%p, err %d.\n", pdev, rv);
	xpdev_free(xpdev);
	return rv;
}

static void remove_xilinx_fpga_pcie_driver(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev;

	if (!pdev)
		return;

	xpdev = dev_get_drvdata(&pdev->dev);
	if (!xpdev)
		return;

	pr_info("pdev 0x%p, xdev 0x%p, 0x%p.\n",
		pdev, xpdev, xpdev->xdev);
	xpdev_free(xpdev);

        dev_set_drvdata(&pdev->dev, NULL);
}

static pci_ers_result_t xdma_error_detected(struct pci_dev *pdev,
					pci_channel_state_t state)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	switch (state) {
	case pci_channel_io_normal:
		return PCI_ERS_RESULT_CAN_RECOVER;
	case pci_channel_io_frozen:
		pr_warn("dev 0x%p,0x%p, frozen state error, reset controller\n",
			pdev, xpdev);
		xdma_device_offline(pdev, xpdev->xdev);
		pci_disable_device(pdev);
		return PCI_ERS_RESULT_NEED_RESET;
	case pci_channel_io_perm_failure:
		pr_warn("dev 0x%p,0x%p, failure state error, req. disconnect\n",
			pdev, xpdev);
		return PCI_ERS_RESULT_DISCONNECT;
	}
	return PCI_ERS_RESULT_NEED_RESET;
}

static pci_ers_result_t xdma_slot_reset(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	pr_info("0x%p restart after slot reset\n", xpdev);
	if (pci_enable_device_mem(pdev)) {
		pr_info("0x%p failed to renable after slot reset\n", xpdev);
		return PCI_ERS_RESULT_DISCONNECT;
	}

	pci_set_master(pdev);
	pci_restore_state(pdev);
	pci_save_state(pdev);
	xdma_device_online(pdev, xpdev->xdev);

	return PCI_ERS_RESULT_RECOVERED;
}

static void xdma_error_resume(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	pr_info("dev 0x%p,0x%p.\n", pdev, xpdev);
	pci_cleanup_aer_uncorrect_error_status(pdev);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
static void xdma_reset_prepare(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	pr_info("dev 0x%p,0x%p.\n", pdev, xpdev);
	xdma_device_offline(pdev, xpdev->xdev);
}

static void xdma_reset_done(struct pci_dev *pdev)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	pr_info("dev 0x%p,0x%p.\n", pdev, xpdev);
	xdma_device_online(pdev, xpdev->xdev);
}

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0)
static void xdma_reset_notify(struct pci_dev *pdev, bool prepare)
{
	struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

	pr_info("dev 0x%p,0x%p, prepare %d.\n", pdev, xpdev, prepare);

	if (prepare)
		xdma_device_offline(pdev, xpdev->xdev);
	else
		xdma_device_online(pdev, xpdev->xdev);
}
#endif

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

static const struct pci_error_handlers xdma_error_handler = {
	.error_detected	= xdma_error_detected,
	.slot_reset	= xdma_slot_reset,
	.resume		= xdma_error_resume,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
	.reset_prepare	= xdma_reset_prepare,
	.reset_done	= xdma_reset_done,
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,16,0)
	.reset_notify	= xdma_reset_notify,
#endif
};

static struct pci_driver xilinx_dma_pci_driver = {
	.name = DRIVER_MODULE_NAME,
	.id_table = xilinx_dma_pci_ids,
	.probe = probe_xilinx_fpga_pcie_card,
	.remove = remove_xilinx_fpga_pcie_driver,
	.err_handler = &xdma_error_handler,
};


/** 
 \fn void skarabveth_init(struct net_device *dev)
 
 \details This is a helper function to initialise the interface.
 
 
 \param [inout] struct net_device *dev The network device handle
 \return  None
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
 \fn int skarabveth_init_module(void)
 
 \details This the kernel module initialise functionfor the interface.
 \return  Error and status value
 
 
 */
int skarabveth_init_module (void)
{
	int result;
	

	result = pci_register_driver(&xilinx_dma_pci_driver);
	if (result < 0)
	{
		printk(KERN_ERR"skarabveth:pci_register_driver Error %d  initializing card skarabveth card",result);		
		return result;
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

    gStatFlags = 0;	
	printk(KERN_WARNING"Cleaning Up the Module\n");
	pci_unregister_driver(&xilinx_dma_pci_driver);
	unregister_netdev(skarabveth);
	free_netdev(skarabveth);
	return;
}
	
module_init (skarabveth_init_module);
module_exit (skarabveth_cleanup);
