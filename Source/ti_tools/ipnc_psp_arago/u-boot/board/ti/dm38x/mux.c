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

static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(pincntl70), PULLUP_EN | MODE(0x01)},	/* UART0_RXD */
	{OFFSET(pincntl71), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl72), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl73), PULLUP_EN | MODE(0x01)},
	{-1},
};

static struct module_pin_mux mmc1_pin_mux[] = {
	{OFFSET(pincntl1), PULLUP_EN | MODE(0x01)},	/* SD1_CLK */
	{OFFSET(pincntl2), PULLUP_EN | MODE(0x01)},	/* SD1_CMD */
	{OFFSET(pincntl3), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[0] */
	{OFFSET(pincntl4), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[1] */
	{OFFSET(pincntl5), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[2] */
	{OFFSET(pincntl6), PULLUP_EN | MODE(0x01)},	/* SD1_DAT[3] */
	{OFFSET(pincntl80), PULLUP_EN | MODE(0x02)},	/* SD1_SDCD */
	{-1},
};

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
	{OFFSET(pincntl125), PULLUP_EN | MODE(0x01)},	/* GPMC_CS3 */
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

static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(pincntl263), PULLUP_EN | MODE(0x01)},	/* I2C_SCL */
	{OFFSET(pincntl264), PULLUP_EN | MODE(0x01)},	/* I2C_SDA */
};

static struct module_pin_mux i2c1_pin_mux[] = {
	{OFFSET(pincntl78), PULLUP_EN | MODE(0x01)},	/* I2C_SCL */
	{OFFSET(pincntl79), PULLUP_EN | MODE(0x01)},	/* I2C_SDA */
};

static struct module_pin_mux i2c2_pin_mux[] = {
	{OFFSET(pincntl74), PULLUP_EN | MODE(0x20)},	/* I2C_SCL */
	{OFFSET(pincntl75), PULLUP_EN | MODE(0x20)},	/* I2C_SDA */
};

static struct module_pin_mux gpio_pin_mux[] = {
	{OFFSET(pincntl16), PULLDOWN_EN | MODE(0x80)},	/* GP0_9 - LED */
	{OFFSET(pincntl41), PULLUP_EN | MODE(0x80)},	/* GP0_12 - KEYPAD */
	{OFFSET(pincntl42), PULLUP_EN | MODE(0x80)},	/* GP0_13 - KEYPAD */
	{OFFSET(pincntl105), PULLDOWN_EN | MODE(0x80)},	/* GP2_5 - BT_EN */
	{OFFSET(pincntl108), PULLDOWN_EN | MODE(0x80)},	/* GP1_14 */
	{OFFSET(pincntl109), PULLUP_EN | MODE(0x80)},	/* GP1_15 PMIC_SLEEP*/
	{OFFSET(pincntl110), PULLDOWN_EN | MODE(0x80)},	/* GP1_16 -
							CAM/UART MUX_SEL*/
	{OFFSET(pincntl123), PULLUP_EN | MODE(0x80)},	/* GP1_24 - LED */
	{OFFSET(pincntl124), PULLUP_EN | MODE(0x80)},	/* GP1_25 - LED */
	{OFFSET(pincntl132), PULLDOWN_EN | MODE(0x80)},	/* GP1_30 - LED */
	{OFFSET(pincntl153), PULLDOWN_EN | MODE(0x80)},	/* GP2_18 */
	{OFFSET(pincntl211), PULLDOWN_EN | MODE(0x80)},	/* GP3_3 - LED */
	{OFFSET(pincntl212), PULLDOWN_EN | MODE(0x80)},	/* GP3_4 */
	{OFFSET(pincntl228), PULLUP_EN | MODE(0x80)},	/* GP3_20 - ETH RESET*/
	{OFFSET(pincntl232), PULLDOWN_EN | MODE(0x80)},	/* GP1_10 - KEYPAD */
	{-1},
};

static struct module_pin_mux hdmi_pin_mux[] = {
	{OFFSET(pincntl111), PULLUP_EN | MODE(0x10)},/* HDMI_CEC */
	{OFFSET(pincntl112), PULLDOWN_EN | MODE(0x10)},/* HDMI_HPDET*/
	{-1},
};


static struct module_pin_mux mcasp0_pin_mux[] = {
	{OFFSET(pincntl17), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl18), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl19), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl20), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl21), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl22), PULLUP_EN | MODE(0x1)},
	{OFFSET(pincntl23), PULLUP_EN | MODE(0x1)},
	{OFFSET(pincntl24), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl25), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl26), PULLDOWN_EN | MODE(0x1)},
	{-1},
};

static struct module_pin_mux mcasp1_pin_mux[] = {
	{OFFSET(pincntl15), PULLDOWN_EN | MODE(0x4)},
	{OFFSET(pincntl31), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl32), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl33), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl34), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl35), PULLUP_EN | MODE(0x1)},
	{OFFSET(pincntl36), PULLUP_EN | MODE(0x1)},
	{-1},
};

static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(pincntl8), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl9), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl10), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl11), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl12), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl13), PULLUP_EN | MODE(0x01)},
	{-1},
};

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

static struct module_pin_mux spi0_pin_mux[] = {
	{OFFSET(pincntl81), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl82), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl83), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl84), PULLUP_EN | MODE(0x01)},
	{-1},
};

static struct module_pin_mux spi1_pin_mux[] = {
	{OFFSET(pincntl85), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl86), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl87), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl88), PULLUP_EN | MODE(0x01)},
	{-1},
};

static struct module_pin_mux uart1_pin_mux[] = {
	{OFFSET(pincntl76), PULLUP_EN | MODE(0x04)},
	{OFFSET(pincntl77), PULLUP_EN | MODE(0x04)},
	{-1},
};

static struct module_pin_mux uart2_pin_mux[] = {
	{OFFSET(pincntl68), PULLUP_EN | MODE(0x02)},
	{OFFSET(pincntl69), PULLUP_EN | MODE(0x02)},
	{-1},
};

static struct module_pin_mux usb0_pin_mux[] = {
	{OFFSET(pincntl270), PULLDOWN_EN | MODE(0x01)},
	{-1},
};
static struct module_pin_mux usb1_pin_mux[] = {
	{OFFSET(pincntl14), PULLDOWN_EN | MODE(0x80)},/* USB DRV */
	{-1},
};

static struct module_pin_mux misc_pin_mux[] = {
	{OFFSET(pincntl7), PULLUP_EN | MODE(0x1)},
	{OFFSET(pincntl259), PULLDOWN_EN | MODE(0x1)},
	{OFFSET(pincntl260), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl261), PULLUP_EN | MODE(0x01)},
	{OFFSET(pincntl262),  MODE(0x01)},
	{-1},
};

static struct module_pin_mux wlan_pin_mux[] = {
	{OFFSET(pincntl106), PULLDOWN_EN | MODE(0x80)},	/* GP2_6 - WLAN_EN*/
	{OFFSET(pincntl107), PULLDOWN_EN | MODE(0x80)},	/* GP1_13 - WLAN_IRQ*/
	{OFFSET(pincntl127), PULLUP_EN | MODE(0x10)},
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
