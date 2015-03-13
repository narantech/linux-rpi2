/*
 * Set LED GPIO to Input "Trigger"
 *
 * Copyright 2015 Phil Elwell <phil@raspberrypi.org>
 *
 * Based on Nick Forbes's ledtrig-default-on.c.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include "../leds.h"

/* This is a hack to get at the private 'gpio' member */

struct gpio_led_data {
	struct led_classdev cdev;
	unsigned gpio;
};

static void input_trig_activate(struct led_classdev *led_cdev)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);
	if (gpio_is_valid(led_dat->gpio))
		gpio_direction_input(led_dat->gpio);
}

static void input_trig_deactivate(struct led_classdev *led_cdev)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);
	if (gpio_is_valid(led_dat->gpio))
		gpio_direction_output(led_dat->gpio, 0);
}

static struct led_trigger input_led_trigger = {
	.name     = "input",
	.activate = input_trig_activate,
	.deactivate = input_trig_deactivate,
};

static int __init input_trig_init(void)
{
	return led_trigger_register(&input_led_trigger);
}

static void __exit input_trig_exit(void)
{
	led_trigger_unregister(&input_led_trigger);
}

module_init(input_trig_init);
module_exit(input_trig_exit);

MODULE_AUTHOR("Phil Elwell <phil@raspberrypi.org>");
MODULE_DESCRIPTION("Set LED GPIO to Input \"trigger\"");
MODULE_LICENSE("GPL");
