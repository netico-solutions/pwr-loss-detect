#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x747fa1c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6ad3c2ed, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x58736c50, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x192fd160, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xc0f4adce, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x84017183, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xef520da5, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xe2caf96b, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x9b25554, __VMLINUX_SYMBOL_STR(gpiod_unexport) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x41fe713a, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x402e533b, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0x7cf87000, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0x51e8521f, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xe819baf8, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xb0093bc8, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B27EE353827BFDB3FE00D82");
