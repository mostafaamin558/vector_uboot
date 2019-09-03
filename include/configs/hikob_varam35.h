/*
 * hikob_varam35.h - Default configuration for  Variscite AM35xx board.
 *
 * Author: Antoine Fraboulet <antoine.fraboulet@hikob.com>
 *
 * Based on omap3_evm_config.h
 *
 * Copyright (C) 2010 HIKOB
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H


/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA8	 1	/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		 1	/* in a TI OMAP core          */
#define CONFIG_OMAP34XX          1      /* which is a 34XX            */
#define CONFIG_OMAP3_VARAM35	 1	/* working with VARAM35       */
#define CONFIG_OMAP_COMMON       1
#define CONFIG_SYS_GENERIC_BOARD 1     

/* External Memory Interface Module */
#define CONFIG_EMIF4                    /* The chip has EMIF4 controller */

#include <asm/arch/cpu.h>		/* get chip and board defs */
#include <asm/arch/omap3.h>

/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/*
 * Size of malloc() pool
 */
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (128 << 10))
/*
 * DDR related
 */
#define CONFIG_SYS_CS0_SIZE		(256 * 1024 * 1024)

/*
 * Hardware drivers
 */

/*
 * OMAP GPIO configuration
 */
#define CONFIG_OMAP_GPIO

/*
 * NS16550 Configuration
 */
#define V_NS16550_CLK			48000000	/* 48MHz (APLL96/2) */

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		V_NS16550_CLK

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		OMAP34XX_UART3
#define CONFIG_SERIAL3			3	/* UART3 on AM3517 EVM */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}
#define CONFIG_MMC			1
#define CONFIG_GENERIC_MMC		1
#define CONFIG_OMAP_HSMMC		1
#define CONFIG_DOS_PARTITION		1

/*
 * USB configuration
 * Enable CONFIG_MUSB_HOST for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_GADGET for Device functionalities.
 */
#define CONFIG_USB_MUSB_AM35X
#define CONFIG_MUSB_HOST
#define CONFIG_MUSB_PIO_ONLY

#ifdef CONFIG_USB_MUSB_AM35X

#ifdef CONFIG_MUSB_HOST
#define CONFIG_CMD_USB

#define CONFIG_USB_STORAGE
#define CONGIG_CMD_STORAGE
#define CONFIG_CMD_FAT

#ifdef CONFIG_USB_KEYBOARD
#define CONFIG_SYS_USB_EVENT_POLL
#define CONFIG_PREBOOT "usb start"
#endif /* CONFIG_USB_KEYBOARD */

#endif /* CONFIG_MUSB_HOST */

#ifdef CONFIG_MUSB_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#endif /* CONFIG_MUSB_GADGET */

#endif /* CONFIG_USB_MUSB_AM35X */

/* commands to include */
#include <config_cmd_default.h>

#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/

#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_MMC		/* MMC support			*/
#define CONFIG_CMD_NAND		/* NAND support			*/
#define CONFIG_CMD_DHCP

#undef CONFIG_CMD_PING
#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/

#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_I2C
#define CONFIG_SYS_OMAP24_I2C_SPEED	100000
#define CONFIG_SYS_OMAP24_I2C_SLAVE	1
#define CONFIG_SYS_I2C_OMAP34XX
#define CONFIG_I2C_MULTI_BUS

/*
 * Ethernet
 */
#define CONFIG_DRIVER_TI_EMAC
#define CONFIG_DRIVER_TI_EMAC_USE_RMII
#define CONFIG_MII
#define CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_NET_RETRY_COUNT		10

/*
 * Board NAND Info.
 */
#define CONFIG_SYS_NAND_ADDR		NAND_BASE	/* physical address */
							/* to access nand */
#define CONFIG_SYS_NAND_BASE		NAND_BASE	/* physical address */
							/* to access */
							/* nand at CS0 */

#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of */
							/* NAND devices */
#define CONFIG_JFFS2_NAND
/* nand device jffs2 lives on */
#define CONFIG_JFFS2_DEV		"nand0"
/* start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x680000
#define CONFIG_JFFS2_PART_SIZE		0xf980000	/* sz of jffs2 part */

/* Environment information */
/* with 1 link is not initialized
 * Worked with 2, let 3 for security */
#define CONFIG_BOOTDELAY	3

/* only cancel autoboot if enter or start is pressed */
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_STOP_STR " "
#define CONFIG_AUTOBOOT_STOP_STR2 "\n"
#define CONFIG_AUTOBOOT_PROMPT \
        "Hit 'Space' or 'Enter' to stop autoboot within %ds\r\n", bootdelay

#define CONFIG_BOOTFILE		"uImage"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"console=ttyO2,115200n8\0" \
	"ethaddr=AA:BB:CC:DD:EE:FF\0" \
	"the_best=clochette\0"

#define CONFIG_BOOTCOMMAND \
	"bootp; setenv bootargs mem=256M console=${console} "\
	"root=/dev/nfs ip=dhcp "\
	"nfsroot=${serverip}:/lely/images/${ipaddr}/image " \
	"rw rootwait eth=${ethaddr}; bootm"

#define CONFIG_AUTO_COMPLETE	1
/*
 * Miscellaneous configurable options
 */
#define V_PROMPT			"HKB_AM35_2014 # "

#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT		V_PROMPT
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		32	/* max number of command */
						/* args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)
/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + \
					0x01F00000) /* 31MB */

#define CONFIG_SYS_LOAD_ADDR		(OMAP34XX_SDRC_CS0) /* default load */
								/* address */

/*
 * AM3517 has 12 GP timers, they can be driven by the system clock
 * (12/13/16.8/19.2/38.4MHz) or by 32KHz clock. We use 13MHz (V_SCLK).
 * This rate is divided by a local divisor.
 */
#define CONFIG_SYS_TIMERBASE		OMAP34XX_GPT2
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	2	/* CS1 may or may not be populated */
#define PHYS_SDRAM_1		OMAP34XX_SDRC_CS0
#define PHYS_SDRAM_2		OMAP34XX_SDRC_CS1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* **** PISMO SUPPORT *** */
#define CONFIG_SYS_MAX_FLASH_SECT	520	/* max number of sectors */
						/* on one chip */
#define CONFIG_SYS_MAX_FLASH_BANKS	2	/* max number of flash banks */
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* Reserve 2 sectors */

#if defined(CONFIG_CMD_NAND)
#define CONFIG_SYS_FLASH_BASE		NAND_BASE
#endif

/* Monitor at start of flash */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_FLASH_BASE

#define CONFIG_ENV_IS_IN_NAND		1
/*
 * Default u-boot 2014.10 starts at 0x260000 with 0x20000
 * configuration size.
 * #define SMNAND_ENV_OFFSET		0x260000 
 * #define CONFIG_SYS_ENV_SECT_SIZE	(128 << 10)
 */
#define SMNAND_ENV_OFFSET		0x240000      /* environment starts here */
#define CONFIG_SYS_ENV_SECT_SIZE	(128 << 10)   /* 128 KiB */
 
#define CONFIG_ENV_OFFSET		SMNAND_ENV_OFFSET
#define CONFIG_ENV_ADDR			SMNAND_ENV_OFFSET

/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

/* Flash banks JFFS2 should use */
#define CONFIG_SYS_MAX_MTD_BANKS	(CONFIG_SYS_MAX_FLASH_BANKS + \
					CONFIG_SYS_MAX_NAND_DEVICE)
#define CONFIG_SYS_JFFS2_MEM_NAND
/* use flash_info[2] */
#define CONFIG_SYS_JFFS2_FIRST_BANK	CONFIG_SYS_MAX_FLASH_BANKS
#define CONFIG_SYS_JFFS2_NUM_BANKS	1

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_INIT_RAM_ADDR	0x4020f800
#define CONFIG_SYS_INIT_RAM_SIZE	0x800
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					 CONFIG_SYS_INIT_RAM_SIZE - \
					 GENERATED_GBL_DATA_SIZE)

/* Defines for SPL */
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_NAND_SIMPLE
#define CONFIG_SPL_TEXT_BASE		0x40200800
#define CONFIG_SPL_MAX_SIZE		(54 * 1024)	/* 8 KB for stack */
#define CONFIG_SPL_STACK		LOW_LEVEL_SRAM_STACK

#define CONFIG_SPL_BSS_START_ADDR	0x80000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000		/* 512 KB */

#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300 /* address 0x60000 */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x200 /* 256 KB */
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME	"u-boot.img"

#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBDISK_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_ECC
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/omap-common/u-boot-spl.lds"

/* NAND boot config : device related config */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT  1

#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000

/* ==================================================
 * NAND Configuration : driver related config
 * ================================================== */

#define CONFIG_NAND_OMAP_GPMC           1
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCSIZE		512
#define GPMC_NAND_ECC_LP_x16_LAYOUT     1
	
#define HKB_BCH8_SW_DEFAULT             1
#if HKB_BCH8_SW_DEFAULT == 1

#define CONFIG_BCH                      1
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW_DETECTION_SW
#undef  CONFIG_SPL_NAND_AM33XX_BCH

/*
 * Used only in SPL mode
 * taken from /include/configs/tricorder.h 
 * http://u-boot.10912.n7.nabble.com/PATCH-v1-0-2-mtd-nand-omap-booting-from-NAND-using-u-boot-td169557.html
 * http://lists.infradead.org/pipermail/linux-mtd/2013-December/050945.html
 * http://e2e.ti.com/support/dsp/davinci_digital_media_processors/f/717/t/159068 
 */
#define CONFIG_SYS_NAND_ECCBYTES	13

#define CONFIG_SYS_NAND_ECCPOS          { 2,  3,  4,  5,  6,  7,  8,  9, 10, 11, \
					 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, \
					 23, 24, 25, 26, 27, 28, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, \
					 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, \
					 55, 56}

/* this one taken from linux-3.9.6 */
#define LEGACY_CONFIG_SYS_NAND_ECCPOS  { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, \
					 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, \
					 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, \
					 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, \
					 62, 63 }


/* taken from /include/configs/am335x_evm.h 
 * this mode needs ELM (BCH8_CODE_HW)
 */
#define HKB_ELM_CONFIG_SYS_NAND_ECCPOS  { 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }
#define HKB_ELM_CONFIG_SYS_NAND_ECCBYTES 14


/* ===================== */
#else
/* ===================== */

#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_HAM1_CODE_HW
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9,	\
						10, 11, 12, 13}
#define CONFIG_SYS_NAND_ECCBYTES	3

#endif
/* ==================================================
 * NAND Configuration
 * ================================================== */

/*
 * 1MB into the SDRAM to allow for SPL's bss at the beginning of SDRAM
 * 64 bytes before this address should be set aside for u-boot.img's
 * header. That is 0x800FFFC0--0x80100000 should not be used for any
 * other needs.
 */
#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_SPL_MALLOC_START	0x80208000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000

#endif /* __CONFIG_H */
