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
	{ 0xb46bdc55, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdb68373d, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xbda90ac0, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x9b25554, __VMLINUX_SYMBOL_STR(gpiod_unexport) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x192fd160, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x57b6b864, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x84017183, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x2c5c9051, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xa970eebe, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0x29714e37, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x402e533b, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0x962de675, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0xee8e4fd, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x9c9ab3ed, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0x44e18189, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1457B895118211D7A6D9FD9");
