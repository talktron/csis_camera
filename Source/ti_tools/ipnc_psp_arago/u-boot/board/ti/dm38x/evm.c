/*
 * evm.c
 *
 * Board functions for DM38X CSK
 *
 * Copyright (C) 2011, Texas Instruments, Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <cpsw.h>
#include <errno.h>
#include <spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <i2c.h>

#include "evm.h"

DECLARE_GLOBAL_DATA_PTR;
#define RMII_REFCLK_SRC		(PLL_SUBSYS_BASE + 0x2E8)
#define GMII_SEL		(CTRL_BASE + 0x650)
#define WDT_UNFREEZE		(CTRL_BASE + 0x100)
#define WDT_DISABLE1		(0xAAAA)
#define WDT_DISABLE2		(0x5555)
#define WDT0_ENABLE		(0x13)
#define CPGMAC_SL1_CTRL		(CPSW_BASE + 0x704)
#define CPGMAC_SL2_CTRL		(CPSW_BASE + 0x744)
#define MODE_RGMII		0x30a
#define MODE_GMII		0x00
#define MODE_RMII		0x305
#define GIGABIT_MODE		7

#define GPIO1_OE_REG		(TI81XX_GPIO1_BASE + 0x134)
#define GPIO1_CLRDATA		(TI81XX_GPIO1_BASE + 0x190)
#define GPIO2_OE_REG		(TI81XX_GPIO2_BASE + 0x134)
#define GPIO2_CLRDATA		(TI81XX_GPIO2_BASE + 0x190)
#define GPIO2_SETDATA		(TI81XX_GPIO2_BASE + 0x194)

#define MCASP_AHCLK_CLKSRC	(PLL_SUBSYS_BASE + 0x2D4)
#define AHCLKX_SRC_OSC1		0x240

#define PMIC_SLAVE_ADDRESS	0x2D
#define PMIC_VDD1_OP_REG	0x22
#define CVDD_ARM_REG		(PMIC_VDD1_OP_REG)
#define PMIC_VDD2_OP_REG	0x25
#define CVDD_HDVICP_REG		(PMIC_VDD2_OP_REG)
#define PMIC_VDDCTRL_OP_REG	0x28
#define CVDD_CORE_REG		(PMIC_VDDCTRL_OP_REG)
#define PMIC_VDD_1V35		0x3F

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;


/* UART Defines */
#ifdef CONFIG_SPL_BUILD
static const struct dmm_lisa_map_regs evm_lisa_map_regs = {
	.dmm_lisa_map_0			= 0x80600100,
	.dmm_lisa_map_1			= 0x80600100,
	.dmm_lisa_map_2			= 0x80600100,
	.dmm_lisa_map_3			= 0x80600100,
};

/* DDR3 Configuration */
static const struct cmd_control evm_ddr3_cctrl_data = {
	.cmd0csratio	= 0x80,
	.cmd0iclkout	= 0x00,

	.cmd1csratio	= 0x80,
	.cmd1iclkout	= 0x00,

	.cmd2csratio	= 0x80,
	.cmd2iclkout	= 0x00,
};

static const struct ddr_data evm_ddr3_data[4] = {
{
	.datardsratio0      = ((0x3D<<10) | (0x3D<<0)),
	.datawdsratio0      = ((0x4A<<10) | (0x4A<<0)),
	.datawiratio0       = 0,
	.datagiratio0       = 0,
	.datafwsratio0      = ((0x91<<10) | (0x91<<0)),
	.datawrsratio0      = ((0x8A<<10) | (0x8A<<0)),
},
{
	.datardsratio0      = ((0x3E<<10) | (0x3E<<0)),
	.datawdsratio0      = ((0x4A<<10) | (0x4A<<0)),
	.datawiratio0       = 0,
	.datagiratio0       = 0,
	.datafwsratio0      = ((0x98<<10) | (0x98<<0)),
	.datawrsratio0      = ((0x87<<10) | (0x87<<0)),
},
{
	.datardsratio0      = ((0x3C<<10) | (0x3C<<0)),
	.datawdsratio0      = ((0x51<<10) | (0x51<<0)),
	.datawiratio0       = 0,
	.datagiratio0       = 0,
	.datafwsratio0      = ((0x9A<<10) | (0x9A<<0)),
	.datawrsratio0      = ((0x88<<10) | (0x88<<0)),
},
{
	.datardsratio0      = ((0x37<<10) | (0x37<<0)),
	.datawdsratio0      = ((0x51<<10) | (0x51<<0)),
	.datawiratio0       = 0,
	.datagiratio0       = 0,
	.datafwsratio0      = ((0x99<<10) | (0x99<<0)),
	.datawrsratio0      = ((0x88<<10) | (0x88<<0)),
}
};

static const struct emif_regs evm_ddr3_emif0_regs = {
#ifdef CONFIG_DDR_533_MHZ
	.sdram_config            = 0x61C11A32,
	.ref_ctrl		 = 0x0000103D,
	.sdram_tim1		 = 0x0EEF2673,
	.sdram_tim2		 = 0x308F7FDA,
	.sdram_tim3		 = 0x507F88AF,
	.emif_ddr_phy_ctlr_1	 = 0x00170209
#else
	/* DDR 400Mhz */
	.sdram_config			= 0x61C11A32,
	.ref_ctrl			= 0x00000C30,
	.sdram_tim1			= 0x0AAAD4F3,
	.sdram_tim2			= 0x206B7FDA,
	.sdram_tim3			= 0x507F869F,
	.emif_ddr_phy_ctlr_1		= 0x00170209
#endif
};

void set_uart_mux_conf(void)
{
	/* Set UART pins */
	enable_uart_pin_mux();
}

void set_mux_conf_regs(void)
{
	/* Set I2c pins */
	enable_i2c_pin_mux();

	/* Set MMC pins */
	enable_mmc_pin_mux();

	/* Set Ethernet pins */
	enable_enet_pin_mux();

	/* Set GPMC pins */
	enable_gpmc_pin_mux();

	/* Set GPIO pins */
	enable_gpio_pin_mux();

	/* set hdmi mux  */
	enable_hdmi_pin_mux();

	enable_mcasp_pin_mux();

	enable_usb_pin_mux();

	enable_spi_pin_mux();

	enable_wlan_pin_mux();

	enable_misc_pin_mux();
}

void sdram_init(void)
{
	config_dmm(&evm_lisa_map_regs);

	config_ddr(0, NULL, &evm_ddr3_data[0], &evm_ddr3_cctrl_data,
			&evm_ddr3_emif0_regs, 0);
}
#endif

/******************************************************************************
 * Routine: watchdog_init
 * Description: Unfreeze Watchdog in DM38X
 *****************************************************************************/
static void watchdog_init(void)
{
    /* WDT */
    /* For WDT to be functional, it needs to be first stopped by writing
     * the pattern 0xAAAA followed by 0x5555 in the WDT start/stop register.
     * After that a write-once register in Control module needs to be
     * configured to unfreeze the timer.
     * Note: It is important to stop the watchdog before unfreezing it
    */
	struct wd_timer *wdtimer = (struct wd_timer *)WDT_BASE;

	writel(WDT_DISABLE1, &wdtimer->wdtwspr);
	while (readl(&wdtimer->wdtwwps) != 0x0)
			;
	writel(WDT_DISABLE2, &wdtimer->wdtwspr);
	while (readl(&wdtimer->wdtwwps) != 0x0)
			;

    /* Unfreeze WDT */
    writel(WDT0_ENABLE, WDT_UNFREEZE);
}

/*
 * GPIO initialization for peripherals with gpio reset
 */
static void gpio_init(void)
{
	u32  addr, val;

	/* Selection signal to CAMERA & UART Muxer */
	addr = GPIO1_OE_REG;		/*GPIO_OE Output Enable Register*/
	val = __raw_readl(addr);
	val &= ~(1<<16);
	writel(val, addr);
	writel((1<<16), GPIO1_CLRDATA);	/* Clear bit 16 in GPIO_DATAOUT */

	/* Reset Camera */
	addr = GPIO2_OE_REG;		/*GPIO_OE Output Enable Register*/
	val = __raw_readl(addr);
	val &= ~(1<<18);
	writel(val, addr);
	writel((1<<18), GPIO2_CLRDATA);	/* Clear bit 18 in GPIO_DATAOUT */
	udelay(1000);
	writel((1<<18), GPIO2_SETDATA);	/*Sets bit 18 in GPIO_DATAOUT*/
}

/*
 * Reset the board
 */
void reset_cpu(ulong addr)
{
        addr = __raw_readl(PRM_RSTCTRL);
        addr &= ~BIT(1);
        addr |= BIT(1);
        __raw_writel(addr, PRM_RSTCTRL);
}

/*
 * Initialize PMIC ARM, HDVICP and CORE voltages to 1.35V
 * by using below formula
 *
 *         Vout = (val * 0.0125) + 0.5625 Volts
 */
void i2c_pmic_init(void)
{
	/* For ARM voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_ARM_REG, PMIC_VDD_1V35);

	/* For HDVICP voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_HDVICP_REG, PMIC_VDD_1V35);

	/* For CORE voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_CORE_REG, PMIC_VDD_1V35);
}

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
	gpmc_init();
	watchdog_init();
	gpio_init();

	/* setup RMII_REFCLK to be sourced from audio_pll */
	__raw_writel(0x4, RMII_REFCLK_SRC);

	/*program GMII_SEL register for RGMII mode */
	__raw_writel(0x30a, GMII_SEL);

	/* McASP1 AHCLKX source from OSC1*/
	writel(AHCLKX_SRC_OSC1, MCASP_AHCLK_CLKSRC);

	/* Init PMIC supplies for arm and hdvicp */
	i2c_pmic_init();

	return 0;
}

#if !defined(CONFIG_SPL_BUILD) && defined(CONFIG_GENERIC_MMC)
int board_mmc_init(bd_t *bis)
{
	omap_mmc_init(1, 0, 0, -1, -1);

	return 0;
}
#endif

#ifdef CONFIG_DRIVER_TI_CPSW
static void cpsw_control(int enabled)
{
	/* VTP can be added here */
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x50,
		.sliver_reg_ofs	= 0x700,
		.phy_addr	= 25,
	},
	{
		.slave_reg_ofs	= 0x90,
		.sliver_reg_ofs	= 0x740,
		.phy_addr	= 1,
	},
};

static struct cpsw_platform_data cpsw_data = {
	.mdio_base		= CPSW_MDIO_BASE,
	.cpsw_base		= CPSW_BASE,
	.mdio_div		= 0xff,
	.channels		= 8,
	.cpdma_reg_ofs		= 0x100,
	.slaves			= 1,
	.slave_data		= cpsw_slaves,
	.ale_reg_ofs		= 0x600,
	.ale_entries		= 1024,
	.host_port_reg_ofs	= 0x28,
	.hw_stats_reg_ofs	= 0x400,
	.bd_ram_ofs		= 0x2000,
	.mac_control		= (1 << 5),
	.control		= cpsw_control,
	.host_port_num		= 0,
	.version		= CPSW_CTRL_VERSION_1,
};
#endif

int board_eth_init(bd_t *bis)
{
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo, val=0;

	if (!eth_getenv_enetaddr("ethaddr", mac_addr)) {
		printf("<ethaddr> not set. Reading from E-fuse\n");
		/* try reading mac address from efuse */
		mac_lo = readl(&cdev->macid0l);
		mac_hi = readl(&cdev->macid0h);
		mac_addr[0] = mac_hi & 0xFF;
		mac_addr[1] = (mac_hi & 0xFF00) >> 8;
		mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
		mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
		mac_addr[4] = mac_lo & 0xFF;
		mac_addr[5] = (mac_lo & 0xFF00) >> 8;

		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("ethaddr", mac_addr);
		else
			printf("Unable to read MAC address. Set <ethaddr>\n");
	}
	writel(MODE_GMII, GMII_SEL);
	val = readl(CPGMAC_SL1_CTRL);
	/*Force GIG mode to get clock supply from PHY*/
	val |= 0x20021;
	val &= ~(1<<GIGABIT_MODE);
	writel(val, CPGMAC_SL1_CTRL);
	return cpsw_register(&cpsw_data);
}
