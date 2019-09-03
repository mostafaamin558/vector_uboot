/*
 * am35xx.c - board file for HIKOB Variscite SOM AM35xx family of devices.
 *
 * Author: Antoine Fraboulet <antoine.fraboulet@hikob.com>
 *
 * Based on ti/evm/evm.c
 *          logicpd/am3517evm/am3517evm.c
 *
 * Copyright (C) 2014
 * HIKOB - http://www.hikob.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/omap_musb.h>
#include <asm/arch/am35x_def.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/musb.h>
#include <asm/mach-types.h>
#include <asm/arch/emac_defs.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/musb.h>
#include <i2c.h>
#include <netdev.h>
#include "am35xx.h"

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

#define HKB_RELEASE               5
#define HKB_WATCHDOG_PERIOD       300
#define HKB_ENABLE_PMIC_FIT_PATCH 1

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

DECLARE_GLOBAL_DATA_PTR;

#define AM3517_IP_SW_RESET	0x48002598
#define CPGMACSS_SW_RST		(1 << 1)

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3517EVM;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}


/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

#ifdef CONFIG_USB_MUSB_AM35X
static struct musb_hdrc_config musb_config = {
	.multipoint     = 1,
	.dyn_fifo       = 1,
	.num_eps        = 16,
	.ram_bits       = 12,
};

static struct omap_musb_board_data musb_board_data = {
	.set_phy_power		= am35x_musb_phy_power,
	.clear_irq		= am35x_musb_clear_irq,
	.reset			= am35x_musb_reset,
};

static struct musb_hdrc_platform_data musb_plat = {
#if defined(CONFIG_MUSB_HOST)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_MUSB_GADGET)
	.mode		= MUSB_PERIPHERAL,
#else
#error "Please define either CONFIG_MUSB_HOST or CONFIG_MUSB_GADGET"
#endif
	.config         = &musb_config,
	.power          = 250,
	.platform_ops	= &am35x_ops,
	.board_data	= &musb_board_data,
};

static void am3517_evm_musb_init(void)
{
	/*
	 * Set up USB clock/mode in the DEVCONF2 register.
	 * USB2.0 PHY reference clock is 13 MHz
	 */
	clrsetbits_le32(&am35x_scm_general_regs->devconf2,
			CONF2_REFFREQ | CONF2_OTGMODE | CONF2_PHY_GPIOMODE,
			CONF2_REFFREQ_13MHZ | CONF2_SESENDEN |
			CONF2_VBDTCTEN | CONF2_DATPOL);

	musb_register(&musb_plat, &musb_board_data,
			(void *)AM35XX_IPSS_USBOTGSS_BASE);
}
#else
#define am3517_evm_musb_init() do {} while (0)
#endif

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

/*
 * WD2 Watchdog Timer settings, reboots platform if
 *     nothing happens
 *
 * see: sprugr0b.pdf page 1670
 *
 */

// from arch/arm/cpu/armv7/omap-common/hwinit-common.c:155
void wait_for_command_complete(struct watchdog *wd_base);

#define WD2_REG_WCLR 0x24 /* rw : prescaler          */
#define WD2_REG_WLDR 0x2c /* rw : timer load value   */
#define WD2_REG_WTGR 0x30 /* rw : trigger reload     */
#define WD2_REG_WWPS 0x34 /* r  : write posting bits */
#define WD2_REG_WSPR 0x48 /* rw : start - stop       */

#define PTV 0
#define WLDR_VAL(secs)    (0xffffffff - ((secs) * (32768/(1<<PTV))) + 1)

#define WATCHDOG_PERIOD_S HKB_WATCHDOG_PERIOD
#define WATCHDOG_PERIOD   WLDR_VAL(WATCHDOG_PERIOD_S)

void hkb_watchdog_start(void)
{
	struct watchdog *wd2_base = (struct watchdog *)WD2_BASE;
	struct prcm *prcm_base = (struct prcm *)PRCM_BASE;

	/* PRCM */

	// PRCM: enable fclk and iclk
	setbits_le32(&prcm_base->fclken_wkup, 0x20);
	setbits_le32(&prcm_base->iclken_wkup, 0x20);
	wait_on_value(ST_WDT2, 0x20, &prcm_base->idlest_wkup, 5);

	/* WD2 MCU */
	// xxP,PTV,xx = 0x20 | 0x1C
	// set WCLR.PRE = 1   (prescaler enable)
	//     WCLR.PTV = PTV (prescaler = pow(2,PTV))
	setbits_le32(WD2_BASE + WD2_REG_WCLR, ((1<< 5) | (PTV << 2)) & 0x3C);
	wait_for_command_complete(wd2_base);

	// load register
	writel(WATCHDOG_PERIOD, WD2_BASE + WD2_REG_WLDR);
	wait_for_command_complete(wd2_base);

	// start watchdog BBBB+4444
	writel(0xBBBB, &wd2_base->wspr);
	wait_for_command_complete(wd2_base);
	writel(0x4444, &wd2_base->wspr);
	wait_for_command_complete(wd2_base);

	// trigger a reload
	writel(0xbeef, WD2_BASE + WD2_REG_WTGR);
	wait_for_command_complete(wd2_base);

	printf("==   M3 + A8 watchdogs (%d)\n",WATCHDOG_PERIOD_S);
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void hkb_pmic_patch(void)
{
#if HKB_ENABLE_PMIC_FIT_PATCH == 1
#ifdef CONFIG_SYS_I2C_OMAP34XX
#define TPS_BUS  0
#define TPS_ADDR 0x48
	//
	// Proper initialisation of i2c1 for TPS operation.
	//
	int ret = i2c_set_bus_num(TPS_BUS);
	if (ret != 0) {
		printf("== I2C Cannot set bus 0 (ret %d)\n",ret);
		return;
	}

	if (i2c_probe(TPS_ADDR) != 0) {
		printf("== I2C Cannot probe TPS PMIC\n");
		return;
	}

	//
	// begin patch power source FIT
	//
	unsigned char tps65023_con_ctrl_value;
	unsigned char tps65023_con_ctrl_value_verif;
	i2c_read(TPS_ADDR, 0x04, 1, &tps65023_con_ctrl_value, 1);
	debug("== hkb i2c TPS, reg[4] = 0x%02x\n",tps65023_con_ctrl_value);

	tps65023_con_ctrl_value|=0x07;
	i2c_write(TPS_ADDR, 0x04, 1, &tps65023_con_ctrl_value, 1);

	i2c_read(TPS_ADDR, 0x04, 1, &tps65023_con_ctrl_value_verif, 1);
	debug("== hkb i2c TPS, reg[4] = 0x%02x\n",tps65023_con_ctrl_value);
	if (tps65023_con_ctrl_value_verif != tps65023_con_ctrl_value) {
		printf("==\n");
		printf("== TPS : cannot set PMIC in right mode\n");
		printf("==\n");
	}
	printf("==   TPS65023 PMIC configuration\n");
#endif
#endif
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

void hkb_eth_waitloop(int n)
{
	volatile unsigned int ctr;
	ctr  = 0;
	do {
		udelay(1000);
		ctr++;
	} while (ctr < n);
}

void hkb_eth_phy_init(void)
{
	u32 reset;

	gpio_direction_output(30, 0);

	// activate PHY reset
	gpio_set_value(30, 0);
	hkb_eth_waitloop(300);

	// deactivate PHY reset
	gpio_set_value(30, 1);
	hkb_eth_waitloop(300);

	// ensure that the module is out of reset
	reset = readl(AM3517_IP_SW_RESET);
	reset &= (~CPGMACSS_SW_RST);
	writel(reset,AM3517_IP_SW_RESET);
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

/*
 * Routine: misc_init_r
 * Description: Init i2c, ethernet, etc... (done here so udelay works)
 */
int misc_init_r(void)
{
	dieid_num_r();
	printf("== HIKOB release : %d\n",HKB_RELEASE);

	// ============
	// Watchdog
	// ============
	hkb_watchdog_start();

	// ============
	// GPIO
	// ============
	// OMAP GPIO, taken from u-boot 2009 variscite port
	gpio_direction_input(2);   //
	gpio_direction_input(3);   //
	gpio_direction_input(4);   //
	gpio_direction_input(5);   //
	gpio_direction_input(6);   //
	gpio_direction_input(7);   //
	gpio_direction_input(8);   //
	gpio_direction_input(30);  // Ethernet ~RESET
	//gpio_direction_input(130); // M3 reset

	// ============
	// I2C & PMIC
	// ============
#ifdef CONFIG_SYS_I2C_OMAP34XX
	i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);
#endif
        hkb_pmic_patch();

	// ============
	// USB init
	// ============
	am3517_evm_musb_init();

	// ============
	// Ethernet
	// ============
	hkb_eth_phy_init();

	// ============
	//
	// ============
	return 0;
}

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_HIKOB_AM35XX();
}

#if defined(CONFIG_GENERIC_MMC) && !defined(CONFIG_SPL_BUILD)
int board_mmc_init(bd_t *bis)
{
	return omap_mmc_init(0, 0, 0, -1, -1);
}
#endif

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */

#if defined(CONFIG_USB_ETHER) && defined(CONFIG_MUSB_GADGET)
int board_eth_init(bd_t *bis)
{
	int rv, n = 0;

	rv = cpu_eth_init(bis);
	if (rv > 0)
		n += rv;

	rv = usb_eth_initialize(bis);
	if (rv > 0)
		n += rv;

	return n;
}
#endif

/* ************************************************** */
/* ************************************************** */
/* ************************************************** */
