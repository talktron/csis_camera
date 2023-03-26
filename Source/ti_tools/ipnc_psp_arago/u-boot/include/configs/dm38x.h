/*
 * dm38x.h
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_DM38X_H
#define __CONFIG_DM38X_H

#define CONFIG_TI81XX
#define CONFIG_TI814X
#define CONFIG_SYS_NO_FLASH
#define CONFIG_OMAP
#define CONFIG_OMAP_COMMON
#define CONFIG_DM38X

#include <asm/arch/omap.h>

#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */
#define CONFIG_SYS_MALLOC_LEN		(1024 << 10)
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_CACHELINE_SIZE	64
#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs  */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG		/* for ramdisk support */
#define CONFIG_CONS_INDEX               1
/* commands to include */
#define CONFIG_CMD_ASKENV
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_VERSION_VARIABLE

#ifdef CONFIG_DDR_533_MHZ
#define	CONFIG_DDR_533_MHZ		1
#endif

#ifdef CONFIG_PHY_MMD_INDIRECT_WRITE
#define PHY_MMD_INDIRECT_WRITE          1
#endif

#define CONFIG_BOOTDELAY		1	/* negative for no autoboot */

#ifdef CONFIG_NAND
#define NANDARGS \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"nandargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"mem=${mem} " \
		"vram=${vram} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype}\0" \
	"nandroot=ubi0:rootfs rw ubi.mtd=NAND.file-system,2048\0" \
	"nandrootfstype=ubifs rootwait=1\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"nand read ${loadaddr} NAND.kernel; " \
		"nand read ${loaddtbaddr} NAND.u-boot-spl-os; " \
		"bootm ${loadaddr} - ${loaddtbaddr}\0"
#else
#define NANDARGS ""
#endif
#if defined(CONFIG_ENV_IS_IN_NAND)
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_OFFSET		0x00260000
#define CONFIG_SYS_ENV_SECT_SIZE	CONFIG_SYS_NAND_BLOCK_SIZE
#endif

#if defined(CONFIG_ENV_IS_IN_MMC)
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_OFFSET               (256 * 1024)
#define CONFIG_SYS_MMC_ENV_DEV          0
#endif

#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifndef CONFIG_SPL_BUILD

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x82000000\0" \
	"loaddtbaddr=0x84000000\0" \
	"fdt_high=0x84500000\0" \
	"bootfile=uImage\0" \
	"fdtfile=dm38x-csk.dtb\0" \
	"console=ttyS0,115200n8\0" \
	"optargs=cmemk.phys_start=0x85000000 cmemk.phys_end=0x89000000 cmemk.allowOverlap=1 notifyk.vpssm3_sva=0xBFD00000 earlyprintk\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rootwait rw\0" \
	"mmcrootfstype=\0" \
	"mem=80M\0" \
	"vram=4M\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"root=${mmcroot} " \
		"mem=${mem} " \
		"vram=${vram} " \
		"${optargs}\0 " \
	"loaduimage=fatload mmc ${mmcdev} ${loadaddr} ${bootfile}\0" \
	"loaddtbimage=fatload mmc ${mmcdev} ${loaddtbaddr} ${fdtfile}\0" \
	"mmcboot=mmc dev ${mmcdev}; if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"echo Booting from mmc ...; " \
			"run mmcargs; " \
			"if run loaduimage && run loaddtbimage; then " \
				"bootm ${loadaddr} - ${loaddtbaddr};" \
			"fi;" \
		"fi;\0" \
	NANDARGS
#endif

#define CONFIG_BOOTCOMMAND \
	"csk_ff_init 1;" \
	"run mmcboot;" \
	"run nandboot;"

/* Clock Defines */
#define V_OSCK			24000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

/* max number of command args */
#define CONFIG_SYS_MAXARGS		16

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		512

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START \
					+ PHYS_DRAM_1_SIZE - (8 << 12))

#define CONFIG_SYS_LOAD_ADDR		0x82000000	/* Default */

#define CONFIG_OMAP_GPIO
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_OMAP_HSMMC
#define CONFIG_DOS_PARTITION
#define CONFIG_SYS_MONITOR_LEN  (300 << 10)
/**
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS		1		/* 1 banks of DRAM */
#define PHYS_DRAM_1_SIZE		0x40000000	/* 1GB */
#define CONFIG_MAX_RAM_BANK_SIZE	(1024 << 20)	/* 1024MB */

#define CONFIG_SYS_SDRAM_BASE		0x80000000
#define CONFIG_SYS_INIT_SP_ADDR		(NON_SECURE_SRAM_END - \
					 GENERATED_GBL_DATA_SIZE)

/**
 * Platform/Board specific defs
 */
#define CONFIG_SYS_TIMERBASE		0x4802E000
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		(48000000)
#define CONFIG_SYS_NS16550_COM1		0x48020000	/* Base EVM has UART0 */

#define CONFIG_BAUDRATE			115200

/* CPU */
#define CONFIG_ARCH_CPU_INIT

#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_CONSOLE_INFO_QUIET

/* Defines for SPL */
/*
 * SPL related defines.  The Public RAM memory map the ROM defines the
 * area between 0x402F0400 and 0x4030B800 as a download area and
 * 0x4030B800 to 0x4030CE00 as a public stack area.  The ROM also
 * supports X-MODEM loading via UART, and we leverage this and then use
 * Y-MODEM to load u-boot.img, when booted over UART.
 */
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE            0x402F0400
#define CONFIG_SPL_MAX_SIZE             (0x4030B800 - CONFIG_SPL_TEXT_BASE)
#define CONFIG_SYS_SPL_ARGS_ADDR        (CONFIG_SYS_SDRAM_BASE + \
                                         (128 << 20))
#define CONFIG_SPL_BSS_START_ADDR	0x80000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000		/* 512 KB */

#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR 0x300 /* address 0x60000 */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS      0x200 /* 256 KB */
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME        "u-boot.img"
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT

#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBDISK_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x20000
#define CONFIG_SYS_SPI_U_BOOT_SIZE	0x40000
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/omap-common/u-boot-spl.lds"

#define CONFIG_SPL_BOARD_INIT

/*
 * 1MB into the SDRAM to allow for SPL's bss at the beginning of SDRAM
 * 64 bytes before this address should be set aside for u-boot.img's
 * header. That is 0x800FFFC0--0x80800000 should not be used for any
 * other needs.
 */
#define CONFIG_SYS_TEXT_BASE		0x80800000
#define CONFIG_SYS_SPL_MALLOC_START	0x80208000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000

/*
 * Since SPL did pll and ddr initialization for us,
 * we don't need to do it twice.
 */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

/* Unsupported features */
#undef CONFIG_USE_IRQ

/* Ethernet */
#define CONFIG_DRIVER_TI_CPSW
#define CONFIG_MII
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_NET_RETRY_COUNT         10
#define CONFIG_PHY_GIGE
#define CONFIG_PHYLIB
#define CONFIG_PHY_TI
/* Nand */
#ifdef CONFIG_NAND

#define CONFIG_SPL_NAND_SUPPORT

#ifdef CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_AM33XX_BCH
#endif

#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_ECC
#define CONFIG_CMD_NAND
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_NAND_OMAP_ELM

#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_BASE		0x08000000
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW

#define MTDIDS_DEFAULT			"nand0=nand.0"
#define MTDPARTS_DEFAULT		"mtdparts=nand.0:" \
					"128k(NAND.SPL)," \
					"4m(NAND.u-boot)," \
					"128k(NAND.u-boot-env)," \
					"6m(NAND.kernel)," \
					"192m(NAND.file-system)," \
					"256k(NAND.u-boot-spl-os)," \
					"12m(NAND.data)," \
					"-(NAND.reserved)"
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x00020000
#define CONFIG_MTD_DEBUG_VERBOSE	7
#define CONFIG_MTD_DEVICE		/* Required for mtdparts */
#define CONFIG_CMD_MTDPARTS
#endif
/* I2C IP block */
#define CONFIG_SYS_OMAP24_I2C_SPEED	100000
#define CONFIG_SYS_OMAP24_I2C_SLAVE	1
#define CONFIG_SYS_I2C_OMAP34XX
#define CONFIG_I2C
#define CONFIG_SYS_I2C
#endif	/* ! __CONFIG_DM385_IPNC_H */
