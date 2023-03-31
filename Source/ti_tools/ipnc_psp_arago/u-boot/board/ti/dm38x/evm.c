/*
 * evm.c
 *
 * Functions for custom DM388 CSIS board
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

#define GPIO0_OE_REG		(TI81XX_GPIO0_BASE + 0x134)
#define GPIO0_CLRDATA		(TI81XX_GPIO0_BASE + 0x190)
#define GPIO0_SETDATA		(TI81XX_GPIO0_BASE + 0x194)

#define GPIO1_OE_REG		(TI81XX_GPIO1_BASE + 0x134)
#define GPIO1_CLRDATA		(TI81XX_GPIO1_BASE + 0x190)
#define GPIO1_SETDATA		(TI81XX_GPIO1_BASE + 0x194)

#define GPIO2_OE_REG		(TI81XX_GPIO2_BASE + 0x134)
#define GPIO2_CLRDATA		(TI81XX_GPIO2_BASE + 0x190)
#define GPIO2_SETDATA		(TI81XX_GPIO2_BASE + 0x194)

#define MCASP_AHCLK_CLKSRC	(PLL_SUBSYS_BASE + 0x2D4)
#define AHCLKX_SRC_OSC1		0x240

// CSIS board uses same TPS65911 I2C address
// and same VDD!, VDD2, and VDDCTR regs
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

/* DDR2 Configuration */
static const struct cmd_control evm_ddr2_cctrl_data = {
	.cmd0csratio	= 0x80,		// CMD0_REG_PHY_CTRL_SLAVE_RATIO_0
	.cmd0iclkout	= 0x00,		// CMD0_REG_PHY_INVERT_CLKOUT_0

	.cmd1csratio	= 0x80,		// CMD1_REG_PHY_CTRL_SLAVE_RATIO_0
	.cmd1iclkout	= 0x00,		// CMD1_REG_PHY_INVERT_CLKOUT_0

	.cmd2csratio	= 0x80,		// CMD2_REG_PHY_CTRL_SLAVE_RATIO_0
	.cmd2iclkout	= 0x00,		// CMD2_REG_PHY_INVERT_CLKOUT_0
};

static const struct emif_regs evm_ddr2_emif0_regs = {
//	.sdram_config			= 0x40801ab2,
	.sdram_config			= 0x438056b2,	// dm388_csis, x16 SDRAM, CAS latency 5, 50 ohm
//	.ref_ctrl			= 0x10000c30,
	.ref_ctrl			= 0x00000c30,	// dm388_csis, enable init and refresh
//	.sdram_tim1			= 0x0aaaf552,
	.sdram_tim1			= 0x088B159A,	// dm388_csis
//	.sdram_tim2			= 0x043631d2,
	.sdram_tim2			= 0x105131D2,	// dm388_csis
//	.sdram_tim3			= 0x00000327,
	.sdram_tim3			= 0x500004DF,	// dm388_csis
//	.emif_ddr_phy_ctlr_1		= 0x00000007
	.emif_ddr_phy_ctlr_1		= 0x00103208	// dm388_csis
};

static const struct ddr_data evm_ddr2_data[4] = {
{	.datardsratio0		= ((0x35<<10) | (0x35<<0)),	// DATA0_REG_PHY_RD_DQS_SLAVE_RATIO_0
	.datawdsratio0		= ((0x20<<10) | (0x20<<0)),	// DATA0_REG_PHY_WR_DQS_SLAVE_RATIO_0
	.datawiratio0		= ((0<<10) | (0<<0)),		// DATA0_REG_PHY_WRLVL_INIT_RATIO_0
	.datagiratio0		= ((0<<10) | (0<<0)),		// DATA0_REG_PHY_GATELVL_INIT_RATIO_0
	.datafwsratio0		= ((0x90<<10) | (0x90<<0)),	// DATA0_REG_PHY_FIFO_WE_SLAVE_RATIO_0
	.datawrsratio0		= ((0x50<<10) | (0x50<<0))	// DATA0_REG_PHY_WR_DATA_SLAVE_RATIO_0
},
{
	.datardsratio0		= ((0x35<<10) | (0x35<<0)),	// DATA1_REG_PHY_RD_DQS_SLAVE_RATIO_0
	.datawdsratio0		= ((0x20<<10) | (0x20<<0)),	// DATA1_REG_PHY_WR_DQS_SLAVE_RATIO_0
	.datawiratio0		= ((0<<10) | (0<<0)),		// DATA1_REG_PHY_WRLVL_INIT_RATIO_0
	.datagiratio0		= ((0<<10) | (0<<0)),		// DATA1_REG_PHY_GATELVL_INIT_RATIO_0
	.datafwsratio0		= ((0x90<<10) | (0x90<<0)),	// DATA1_REG_PHY_FIFO_WE_SLAVE_RATIO_0
	.datawrsratio0		= ((0x50<<10) | (0x50<<0)),	// DATA1_REG_PHY_WR_DATA_SLAVE_RATIO_0
},
{
	.datardsratio0		= ((0x35<<10) | (0x35<<0)),	// DATA2_REG_PHY_RD_DQS_SLAVE_RATIO_0
	.datawdsratio0		= ((0x20<<10) | (0x20<<0)),	// DATA2_REG_PHY_WR_DQS_SLAVE_RATIO_0
	.datawiratio0		= ((0<<10) | (0<<0)),		// DATA2_REG_PHY_WRLVL_INIT_RATIO_0
	.datagiratio0		= ((0<<10) | (0<<0)),		// DATA2_REG_PHY_GATELVL_INIT_RATIO_0
	.datafwsratio0		= ((0x90<<10) | (0x90<<0)),	// DATA2_REG_PHY_FIFO_WE_SLAVE_RATIO_0
	.datawrsratio0		= ((0x50<<10) | (0x50<<0)),	// DATA2_REG_PHY_WR_DATA_SLAVE_RATIO_0
},
{
	.datardsratio0		= ((0x35<<10) | (0x35<<0)),	// DATA3_REG_PHY_RD_DQS_SLAVE_RATIO_0
	.datawdsratio0		= ((0x20<<10) | (0x20<<0)),	// DATA3_REG_PHY_WR_DQS_SLAVE_RATIO_0
	.datawiratio0		= ((0<<10) | (0<<0)),		// DATA3_REG_PHY_WRLVL_INIT_RATIO_0
	.datagiratio0		= ((0<<10) | (0<<0)),		// DATA3_REG_PHY_GATELVL_INIT_RATIO_0
	.datafwsratio0		= ((0x90<<10) | (0x90<<0)),	// DATA3_REG_PHY_FIFO_WE_SLAVE_RATIO_0
	.datawrsratio0		= ((0x50<<10) | (0x50<<0)),	// DATA3_REG_PHY_WR_DATA_SLAVE_RATIO_0
}
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

	// TODO:  CSIS does not use Ethernet
	/* Set Ethernet pins */
	enable_enet_pin_mux();

	/* Set GPMC pins */
	enable_gpmc_pin_mux();

	// TODO:  CSIS uses different GPIO pins
	/* Set GPIO pins */
	enable_gpio_pin_mux();

	// TODO:  CSIS does not use HDMI
	/* set hdmi mux  */
	enable_hdmi_pin_mux();

	enable_mcasp_pin_mux();

	enable_usb_pin_mux();

	// TODO:  verify spi pins for CSIS
	enable_spi_pin_mux();

	// TODO:  verify wlan pins for CSIS
	enable_wlan_pin_mux();

	enable_misc_pin_mux();
}

void sdram_init(void)
{
	config_dmm(&evm_lisa_map_regs);

//	config_ddr(0, NULL, &evm_ddr3_data[0], &evm_ddr3_cctrl_data,
//			&evm_ddr3_emif0_regs, 0);
	config_ddr(0, NULL, &evm_ddr2_data[0], &evm_ddr2_cctrl_data,
			&evm_ddr2_emif0_regs, 0);
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
	u32  addr;
	u32 val = 0;
	u32 mask;

	// Select i2c for AR0522 sensor
	mask = (1<<15);
	writel(mask, GPIO0_CLRDATA);	// Clear I2C0_18_EN (GP0.15) in GPIO_DATAOUT
	addr = GPIO0_OE_REG;
	val = __raw_readl(addr);
//	val |= mask;			// Make I2C0_18_EN (GP0.15) pin an input
	val &= ~mask;			// Make I2C0_18_EN (GP0.15) pin an output
	writel(val, addr);		// I2C0_18_EN (GP0.15) will pull low to disable onboard sensor  9/6/22 DAT

	// Put GP0 bits in output mode
	// COEX1, COEX2, CAM_RESET, CAM_STROBE, RF_RTS, TPS_SLEEP all outputs
	mask = (1<<13)|(1<<14)|(1<<16)|(1<<17)|(1<<21)|(1<<26);
	writel(mask, GPIO0_CLRDATA);	// Clear bits in GPIO_DATAOUT
	writel((1<<21), GPIO0_SETDATA);	// Make RF_RTS high
	addr = GPIO0_OE_REG;
	val = __raw_readl(addr);
	val &= ~mask;
	writel(val, addr);		// Make GP0 pins be outputs, driven low

	// Put GP1 bits in output mode
	mask = (1<<12);			// WIFI_EN is output
//	writel(mask, GPIO1_SETDATA);	// Set WIFI_EN bit in GPIO_DATAOUT
	writel(mask, GPIO1_CLRDATA);	// Clear WIFI_EN bit in GPIO_DATAOUT
	addr = GPIO1_OE_REG;
	val = __raw_readl(addr);
	val &= ~mask;
	val |= (1<<11);			// Make WIFI_IRQ (GP!.11) be input
	writel(val, addr);		// Make WIFI_EN (GP1.12) pins be output, driven low
	printf("'gpio_init': WIFI_EN (GP1.12) low\n");

	// Put GP2 bits in output mode
	mask = (1<<11)|(1<<12);		// GPS_ENB, BT_EN are outputs
	writel(mask, GPIO2_CLRDATA);	// Clear GPS_ENB, BT_EN bits in GPIO_DATAOUT
	addr = GPIO2_OE_REG;
	val = __raw_readl(addr);
	val &= ~mask;
	writel(val, addr);		// Make GPS_ENB (GP2.11), BT_EN (GP2.12) pins be outputs, driven low

	// Reset AR0522 sensor
	mask = (1<<16);			// CAM_RESET is output
	addr = GPIO0_OE_REG;
	val = __raw_readl(addr);
	val |= mask;			// Make CAM_RESET (GP0.16) pin an input
	writel(val, addr);		// Pin gets pulled up to do sensor reset

	udelay(1000);

	writel((1<<16), GPIO0_CLRDATA);	// Clear CAM_RESET (GP0.16) in GPIO_DATAOUT
	val &= ~mask;
	writel(val, addr);		// Make CAM_RESET (GP0.16) pin an output, driving pin low

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

// CSIS board uses I2C0 for image sensor, adc and audio codec
// Also needs steering to image sensor via I2C0_18_EN (GP0.15,)
// CSIS board uses I2C1 for PMIC
//
// DM388CSK uses I2C0 for the PMIC
// DM388CSK uses I2C2 for HDMI_CODEC, which goes offboard to audio codec
// DM388CSK uses I2C2 for the camera sensor
//
// TODO:  not clear how to select which I2C bus to use for 'i2c_write'
// It's driven by 'cur_12c_bus' and may default to I2C0.  There is a func
// 'i2c_set_bus_num()' in 'i2c_core.c' that does the selection (?)

/*
 * Initialize PMIC ARM, HDVICP and CORE voltages to 1.35V
 * by using below formula
 *
 *         Vout = (val * 0.0125) + 0.5625 Volts
 */
void i2c_pmic_init(void)
{
	printf("'i2c_pmic_init'\n");

	/* For ARM voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_ARM_REG, PMIC_VDD_1V35);

	/* For HDVICP voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_HDVICP_REG, PMIC_VDD_1V35);

	/* For CORE voltage = 1.35 volts*/
	i2c_reg_write(PMIC_SLAVE_ADDRESS, CVDD_CORE_REG, PMIC_VDD_1V35);
}

/*
 * Basic board specific setup.  Pinmux has been handled already.
 * 'board_init' is called from the 'init_sequence_r' table in 'board_r.c'	7/11/22 DAT
 */
int board_init(void)
{

	printf("'board_init'\n");		// 7/11/22 DAT

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
