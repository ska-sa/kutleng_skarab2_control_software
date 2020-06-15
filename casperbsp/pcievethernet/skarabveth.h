/**  ---------------------------------------------------------------------------
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
#ifndef _SKARABVETH_DRV_
#define _SKARABVETH_DRV_

#include <linux/ioctl.h>

struct  skarabveth_priv{
	struct net_device *dev;
	struct net_device_stats stats;
	int status;
	spinlock_t lock;
};

irqreturn_t skarabveth_irq_handler(int irq, void *dev_id);
irqreturn_t skarabveth_irq_msi_handler(int irq, void *dev_id);

#define PCI_VENDOR_ID_XILINX      0x10ee
#define PCI_DEVICE_ID_XILINX_XDMA_PCIE 0x903f
#define BAR_APPERTURE_SIZE        1024       /**< We are using 1K BARS. */

#define HAVE_MEM_REGION        0x001      /**< I/O Memory region     */
#define HAVE_IRQ               0x002      /**< Interrupt             */
#define HAVE_MSI_IRQ           0x004      /**< MSI Interrupt         */
#define HAVE_MSI_N_IRQ         0x008      /**< MSI N Interrupt       */


#define SUCCESS 0
#define ERROR  -1

#endif /* _SKARABVETH_DRV_ */
