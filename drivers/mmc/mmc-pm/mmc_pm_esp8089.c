/*
 * drivers/mmc/mmc-pm/mmc_pm_esp8089.c
 *
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * esp8089 sdio wifi power management API
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <plat/sys_config.h>

#include "mmc_pm.h"

#define esp8089_msg(...)    do {printk("[esp8089]: "__VA_ARGS__);} while(0)

static int esp8089_gpio_ctrl(char* name, int level)
{
	int i = 0, ret = 0;
	struct mmc_pm_ops *ops = &mmc_card_pm_ops;
	char* gpio_name[2] = {
		"esp8089_rst",
		"esp8089_chip_en",
	};

    for (i=0; i<2; i++) {
        if (strcmp(name, gpio_name[i])==0)
            break;
    }
    if (i==2) {
        esp8089_msg("No gpio %s for esp8089-wifi module\n", name);
        return -1;
    }

    ret = gpio_write_one_pin_value(ops->pio_hdle, level, name);
    if (ret) {
        esp8089_msg("Failed to set gpio %s to %d !\n", name, level);
        return -1;
    } else
		esp8089_msg("Succeed to set gpio %s to %d !\n", name, level);

    if (strcmp(name, "esp8089_chip_en") == 0) {
        esp8089_msg("ESP8089 SDIO Wifi Power %s !!\n", level ? "UP" : "Off");
    }

    return 0;
}

static int esp8089_get_io_value(char* name)
{
	int ret = -1;
	struct mmc_pm_ops *ops = &mmc_card_pm_ops;
	
    if (strcmp(name, "esp8089_wakeup")) {
        esp8089_msg("No gpio %s for ESP8089\n", name);
        return -1;
    }
	ret = gpio_read_one_pin_value(ops->pio_hdle, name);
	esp8089_msg("Succeed to get gpio %s value: %d !\n", name, ret);

	return ret;
}

void esp8089_gpio_init(void)
{
	struct mmc_pm_ops *ops = &mmc_card_pm_ops;

	esp8089_msg("exec esp8089_wifi_gpio_init...\n");
	ops->gpio_ctrl = esp8089_gpio_ctrl;
	ops->get_io_val = esp8089_get_io_value;
}
