/*
 * mux.c
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include <i2c.h>
#include "evm.h"

// CSIS board uses UART0 for debug console, not for WIFI serial comms, so remove R77, R78
// DM388CSK uses UART0 for debug console
static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(pincntl70), PULLUP_EN | MODE(0x01)},	/* UART0_RXD */
	{OFFSET(pincntl71), PULLUP_EN | MODE(0x01)},	/* UART0_TXD */
	
	{OFFSET(pincntl72), PULLUP_EN | MODE(0x01)},    /* UART0_CTS */
	{OFFSET(pincntl73), PULLUP_EN | MODE(0x01)},    /* UART0_RTS */
	{-1},
};

// CSIS board doesn't use UART1
// DM388CSK uses UART1 for Expansion UART
static struct module_pin_mux uart1_pin_mux[] = {
	{OFFSET(pincntl76), PULLUP_EN | MODE(0x04)},
	{OFFSET(pincntl77), PULLUP_EN | MODE(0x04)},
	{-1},
};

// CSIS board uses UART2 for RF comms, but with rework will use UART2 for WiFi comms
// DM388CSK uses UART2 for WiFi comms
static struct module_pin_mux uart2_pin_mux[] = {
	{OFFSET(pincntl69), PULLDOWN_EN | MODE(0x08)},  /* UART2_RXD */
	{OFFSET(pincntl68), PULLDOWN_EN | MODE(0x08)},  /* UART2_TXD */

	{-1},
};


// SD1 on both boards used for SDCard
static struct module_pin_mux mmc1_pin_mux[] = {
	{OFFSET(pincntl1), PULLUP_EN | MODE(0x01)},	/* SD1_CLK */
	{OFFSET(pincntl2), PULLUP_EN | MODE(0x01)},	/* SD1_CMD */
	{OFFSET(pincntl3), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[0] */
	{OFFSET(pincntl4), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[1] */
	{OFFSET(pincntl5), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[2] */
	{OFFSET(pincntl6), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[3] */

	// For CSIS board, SDCD is pincntl41 in mode 0x80 as GP0_12 !!!!
	// and pincntl80 has become SPI0_CS1n for the Battery
	{OFFSET(pincntl80), PULLUP_EN | MODE(0x02)},	/* SD1_SDCD */
//	{OFFSET(pincntl80), PULLDOWN_EN | MODE(0x02)},	/* SD1_SDCD */
	{OFFSET(pincntl41), PULLUP_EN | MODE(0x80)},	/* CSIS SD1_SDCD */

	{OFFSET(pincntl42), PULLUP_EN | MODE(0x80)},	/* CSIS SD1_WP */
	{-1},
};

// TODO:  CSIS board does not have ENET, and doesn't use any of these pins
static struct module_pin_mux enet_pin_mux[] = {
	{OFFSET(pincntl233), PULLUP_EN | MODE(0x01)},	/* MDCLK */
	{OFFSET(pincntl234), PULLUP_EN | MODE(0x01)},	/* MDIO */
	{OFFSET(pincntl235), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTCLK */
	{OFFSET(pincntl236), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MCOL */
	{OFFSET(pincntl237), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MCRS */
	{OFFSET(pincntl238), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXER */
	{OFFSET(pincntl239), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRCLK */
	{OFFSET(pincntl240), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[0] */
	{OFFSET(pincntl241), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[1] */
	{OFFSET(pincntl242), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[2] */
	{OFFSET(pincntl243), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[3] */
	{OFFSET(pincntl244), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[4] */
	{OFFSET(pincntl245), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[5] */
	{OFFSET(pincntl246), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[6] */
	{OFFSET(pincntl247), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXD[7] */
	{OFFSET(pincntl248), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MRXDV */
	{OFFSET(pincntl249), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_GMTCLK */
	{OFFSET(pincntl250), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[0] */
	{OFFSET(pincntl251), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[1] */
	{OFFSET(pincntl252), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[2] */
	{OFFSET(pincntl253), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[3] */
	{OFFSET(pincntl254), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[4] */
	{OFFSET(pincntl255), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[5] */
	{OFFSET(pincntl256), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[6] */
	{OFFSET(pincntl257), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXD[7] */
	{OFFSET(pincntl258), PULLDOWN_EN | MODE(0x01)},	/* EMAC[0]_MTXEN */
};

static struct module_pin_mux gpmc_pin_mux[] = {
	{OFFSET(pincntl128), PULLUP_EN | MODE(0x01)},/* GPMC_ADV_ALE */
	{OFFSET(pincntl122), PULLUP_EN | MODE(0x01)},	/* GPMC_CS0 */
	// CSIS board uses pincntl125 for Flash WPn, as GP1_26
	// as does DM388CSK, so making it CS3 is nonsense !
//	{OFFSET(pincntl125), PULLUP_EN | MODE(0x01)},	/* GPMC_CS3 */
	{OFFSET(pincntl125), PULLUP_EN | MODE(0x80)},	/* GPMC_CS3 8/18/22 DAT */

	{OFFSET(pincntl89), MODE(0x01)},	/* GPMC_D0 to GPMC_D15 */
	{OFFSET(pincntl90), MODE(0x01)},
	{OFFSET(pincntl91), MODE(0x01)},
	{OFFSET(pincntl92), MODE(0x01)},
	{OFFSET(pincntl93), MODE(0x01)},
	{OFFSET(pincntl94), MODE(0x01)},
	{OFFSET(pincntl95), MODE(0x01)},
	{OFFSET(pincntl96), MODE(0x01)},
	{OFFSET(pincntl97), MODE(0x01)},
	{OFFSET(pincntl98), MODE(0x01)},
	{OFFSET(pincntl99), MODE(0x01)},
	{OFFSET(pincntl100), MODE(0x01)},
	{OFFSET(pincntl101), MODE(0x01)},
	{OFFSET(pincntl102), MODE(0x01)},
	{OFFSET(pincntl103), MODE(0x01)},
	{OFFSET(pincntl104), MODE(0x01)},
	{OFFSET(pincntl129), PULLUP_EN | MODE(0x01)},	/* GPMC_OE_RE */
	{OFFSET(pincntl130), PULLUP_EN | MODE(0x01)},	/* GPMC_WE */
	{OFFSET(pincntl131), PULLDOWN_EN | MODE(0x01)},	/* GPMC_BE0_CLE */
	{OFFSET(pincntl133), PULLUP_EN | MODE(0x01)},	/* GPMC_WAIT0 */
};

// CSIS board uses I2C0 for image sensor, adc and audio codec
// Also needs steering to image sensor via I2C0_18_EN (GP0.15,)
// DM388CSK uses I2C0 for the PMIC
static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(pincntl263), PULLUP_EN | MODE(0x01)},	/* I2C_SCL */
	{OFFSET(pincntl264), PULLUP_EN | MODE(0x01)},	/* I2C_SDA */
};

// CSIS board uses I2C1 for PMIC
// DM388CSK doesn't use I2C1
static struct module_pin_mux i2c1_pin_mux[] = {
	{OFFSET(pincntl78), PULLUP_EN | MODE(0x01)},	/* I2C_SCL */
	{OFFSET(pincntl79), PULLUP_EN | MODE(0x01)},	/* I2C_SDA */
};

// CSIS board does not use I2C2
// DM388CSK sends I2C2 offboard for image sensor comms
// Also for HDMI_CODEC for audio codec
static struct module_pin_mux i2c2_pin_mux[] = {
	{OFFSET(pincntl74), PULLUP_EN | MODE(0x20)},	/* I2C_SCL */
	{OFFSET(pincntl75), PULLUP_EN | MODE(0x20)},	/* I2C_SDA */
};

static struct module_pin_mux gpio_pin_mux[] = {
	// TODO: for CSIS, some GPIO output pins must operate as open-collector
	//
	// CSIS uses GP0_19 for RF_CTS as GPIO input
	{OFFSET(pincntl165), PULLUP_EN | MODE(0x80)},	/* GP0_19 - RF_CTS */
	// CSIS uses GP0_21 for RF_RTS as GPIO output
	{OFFSET(pincntl167), PULLUP_EN | MODE(0x80)},	/* GP0_21 - RF_RTS */
	// CSIS uses GP2_11 for GPS_EN as GPIO output
	{OFFSET(pincntl146), PULLUP_EN | MODE(0x80)},	/* GP2_11 - GPS_EN */
	// CSIS uses GP2_12 for BT_EN as GPIO output
	{OFFSET(pincntl147), PULLUP_EN | MODE(0x80)},	/* GP2_12 - BT_EN */

	// CSIS uses GP0_13 for COEX1 as GPIO input
	{OFFSET(pincntl159), PULLUP_EN | MODE(0x80)},	/* GP0_13 - COEX1 */
	// CSIS uses GP0_14 for COEX2 as GPIO input
	{OFFSET(pincntl160), PULLUP_EN | MODE(0x80)},	/* GP0_14 - COEX2 */
	// CSIS uses GP0_15 for I2C0_18_EN as GPIO output
	{OFFSET(pincntl161), PULLUP_EN | MODE(0x80)},	/* GP0_15 - I2C0_18_EN */
	// CSIS uses GP0_16 for CAM_RESET as GPIO output
	{OFFSET(pincntl162), PULLUP_EN | MODE(0x80)},	/* GP0_16 - CAM_RESET */
	// CSIS uses GP0_17 for CAM_STROBE as GPIO output
	{OFFSET(pincntl163), PULLUP_EN | MODE(0x80)},	/* GP0_17 - CAM_STROBE */
	// CSIS uses GP0_26 for TPS_SLEEP as GPIO OC output
	{OFFSET(pincntl172), PULLUP_EN | MODE(0x80)},	/* GP0_26 - TPS_SLEEP */

	// DM388CSK GPIO
//	{OFFSET(pincntl16), PULLDOWN_EN | MODE(0x80)},	/* GP0_9 - LED */
//	{OFFSET(pincntl41), PULLUP_EN | MODE(0x80)},	/* GP0_12 - KEYPAD */
//	{OFFSET(pincntl42), PULLUP_EN | MODE(0x80)},	/* GP0_13 - KEYPAD */
//	{OFFSET(pincntl105), PULLDOWN_EN | MODE(0x80)},	/* GP2_5 - BT_EN */
//	{OFFSET(pincntl108), PULLDOWN_EN | MODE(0x80)},	/* GP1_14 */
//	{OFFSET(pincntl109), PULLUP_EN | MODE(0x80)},	/* GP1_15 PMIC_SLEEP*/
//	{OFFSET(pincntl110), PULLDOWN_EN | MODE(0x80)},	/* GP1_16 -	CAM/UART MUX_SEL*/
//	{OFFSET(pincntl123), PULLUP_EN | MODE(0x80)},	/* GP1_24 - LED */
//	{OFFSET(pincntl124), PULLUP_EN | MODE(0x80)},	/* GP1_25 - LED */
//	{OFFSET(pincntl132), PULLDOWN_EN | MODE(0x80)},	/* GP1_30 - LED */
//	{OFFSET(pincntl153), PULLDOWN_EN | MODE(0x80)},	/* GP2_18 */
//	{OFFSET(pincntl211), PULLDOWN_EN | MODE(0x80)},	/* GP3_3 - LED */
//	{OFFSET(pincntl212), PULLDOWN_EN | MODE(0x80)},	/* GP3_4 */
//	{OFFSET(pincntl228), PULLUP_EN | MODE(0x80)},	/* GP3_20 - ETH RESET*/
//	{OFFSET(pincntl232), PULLDOWN_EN | MODE(0x80)},	/* GP1_10 - KEYPAD */
	{-1},
};

// CSIS doesn't use HDMI
static struct module_pin_mux hdmi_pin_mux[] = {
	{OFFSET(pincntl111), PULLUP_EN | MODE(0x10)},	/* HDMI_CEC */
	{OFFSET(pincntl112), PULLDOWN_EN | MODE(0x10)},	/* HDMI_HPDET*/
	{-1},
};

// CSIS board doesn't use MCA[0]
// DM388CSK uses MCA[0] for BT audio channel
static struct module_pin_mux mcasp0_pin_mux[] = {
	// DM388CSK uses MCA[0] for BT audio channel
	{OFFSET(pincntl17), PULLDOWN_EN | MODE(0x1)},	/* MCA0_ACLKX */
	{OFFSET(pincntl18), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AFSX */
	{OFFSET(pincntl19), PULLDOWN_EN | MODE(0x1)},	/* MCA0_ACLKR */
	{OFFSET(pincntl20), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AFSR */
	{OFFSET(pincntl21), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AXR0 */
	{OFFSET(pincntl22), PULLUP_EN | MODE(0x1)},	/* MCA0_AXR1 */

	// DM388CSK uses remainder MCA[0] for offboard audio channel
	{OFFSET(pincntl23), PULLUP_EN | MODE(0x1)},	/* MCA0_AXR2 */
	{OFFSET(pincntl24), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AXR3 */
	{OFFSET(pincntl25), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AXR4 */
	{OFFSET(pincntl26), PULLDOWN_EN | MODE(0x1)},	/* MCA0_AXR5 */
	{-1},
};

// TODO: verify use of CLKX, CLKR for CSIS
// CSIS board uses MCA[1] for audio codec
// DM388CSK uses MCA[0] for BT audio channel
static struct module_pin_mux mcasp1_pin_mux[] = {
	{OFFSET(pincntl15), PULLDOWN_EN | MODE(0x4)},	/* MCA1_AHCLKX */	/* CSIS AIC_MCLK */
	{OFFSET(pincntl31), PULLDOWN_EN | MODE(0x1)},	/* MCA1_AHCLKX */	/* CSIS AIC_BCLK */
	{OFFSET(pincntl32), PULLDOWN_EN | MODE(0x1)},	/* MCA1_AFSX */		/* CSIS AIC_WCLK */
	{OFFSET(pincntl33), PULLDOWN_EN | MODE(0x1)},	/* MCA1_AHCLKR */
	{OFFSET(pincntl34), PULLDOWN_EN | MODE(0x1)},	/* MCA1_AFSR */
	{OFFSET(pincntl35), PULLUP_EN | MODE(0x1)},	/* MCA1_AXR0 */		/* CSIS AIC_DIN */
	{OFFSET(pincntl36), PULLUP_EN | MODE(0x1)},	/* MCA1_AXR1 */		/* CSIS AIC_DOUT */
	{-1},
};

// Both boards use SD0 for WiFi
static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(pincntl8), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl9), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl10), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl11), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl12), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl13), PULLUP_EN | MODE(0x01)},
	{-1},
};

// CSIS doesn't use SD2
static struct module_pin_mux mmc2_pin_mux[] = {
	{OFFSET(pincntl113), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl114), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl115), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl116), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl117), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl118), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl119), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl120), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl121), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl126), PULLUP_EN | MODE(0x02)},
	{-1},
};

// CSIS uses SPI0 for Battery board comms
// DM388CSK uses SPI0 for Expansion
static struct module_pin_mux spi0_pin_mux[] = {
	{OFFSET(pincntl81), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl82), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl83), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl84), PULLUP_EN | MODE(0x01)},
	{-1},
};

// CSIS uses SPI1 for UI board comms
// DM388CSK uses SPI1 for piggyback termination board (N/A)
static struct module_pin_mux spi1_pin_mux[] = {
	{OFFSET(pincntl85), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl86), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl87), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl88), PULLUP_EN | MODE(0x01)},
	{-1},
};

static struct module_pin_mux usb0_pin_mux[] = {
	{OFFSET(pincntl270), PULLDOWN_EN | MODE(0x01)},	/* GP0[7] */
	{-1},
};

// DM388CSK uses this pin as GPIO output
static struct module_pin_mux usb1_pin_mux[] = {
	{OFFSET(pincntl14), PULLDOWN_EN | MODE(0x80)},/* USB1_DRVVBUS */
	{-1},
};

static struct module_pin_mux misc_pin_mux[] = {
	{OFFSET(pincntl7), PULLUP_EN | MODE(0x1)},	/* DEVOSC_WAKE */
	{OFFSET(pincntl259), PULLDOWN_EN | MODE(0x1)},	/* CLKIN32 */
	{OFFSET(pincntl260), PULLUP_EN | MODE(0x01)},	/* RESETn */
	{OFFSET(pincntl261), PULLUP_EN | MODE(0x01)},	/* NMIn */
	{OFFSET(pincntl262),  MODE(0x01)},		/* RSTOUT_WD_OUTn */
	{-1},
};

static struct module_pin_mux wlan_pin_mux[] = {
	// CSIS uses GP1_12 for WIFI_EN as GPIO output
//	{OFFSET(pincntl106), PULLDOWN_EN | MODE(0x80)},	/* GP2_6 - WLAN_EN*/
	{OFFSET(pincntl141), PULLDOWN_EN | MODE(0x80)},	/* GP1_12 - WIFI_EN */

	// CSIS uses GP1_11 for WIFI_IRQ as GPIO input w/interrupt
//	{OFFSET(pincntl107), PULLDOWN_EN | MODE(0x80)},	/* GP1_13 - WLAN_IRQ*/
	{OFFSET(pincntl140), PULLUP_EN | MODE(0x80)},	/* GP1_11 - WIFI_IRQ */

	{OFFSET(pincntl127), PULLUP_EN | MODE(0x10)},	/* GPMC_CLK = CLKOUT1 */
	{-1},
};

void enable_uart_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
	configure_module_pin_mux(uart1_pin_mux);
	configure_module_pin_mux(uart2_pin_mux);
}

void enable_mmc_pin_mux(void)
{
	configure_module_pin_mux(mmc0_pin_mux);
	configure_module_pin_mux(mmc1_pin_mux);
	configure_module_pin_mux(mmc2_pin_mux);
}

void enable_enet_pin_mux(void)
{
	configure_module_pin_mux(enet_pin_mux);
}

void enable_gpmc_pin_mux(void)
{
	configure_module_pin_mux(gpmc_pin_mux);
}

void enable_i2c_pin_mux(void)
{
	configure_module_pin_mux(i2c0_pin_mux);
	configure_module_pin_mux(i2c1_pin_mux);
	configure_module_pin_mux(i2c2_pin_mux);
}

void enable_gpio_pin_mux(void)
{
	configure_module_pin_mux(gpio_pin_mux);
}

void enable_hdmi_pin_mux(void)
{
	configure_module_pin_mux(hdmi_pin_mux);
}

void enable_mcasp_pin_mux(void)
{
	configure_module_pin_mux(mcasp0_pin_mux);
	configure_module_pin_mux(mcasp1_pin_mux);
}

void enable_usb_pin_mux(void)
{
	configure_module_pin_mux(usb0_pin_mux);
	configure_module_pin_mux(usb1_pin_mux);
}

void enable_spi_pin_mux(void)
{
	configure_module_pin_mux(spi0_pin_mux);
	configure_module_pin_mux(spi1_pin_mux);
}

void enable_wlan_pin_mux(void)
{
	configure_module_pin_mux(wlan_pin_mux);
}

void enable_misc_pin_mux(void)
{
	configure_module_pin_mux(misc_pin_mux);
}
