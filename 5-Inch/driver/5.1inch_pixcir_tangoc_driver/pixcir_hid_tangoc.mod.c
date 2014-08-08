#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0x46f584b7, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x1976aa06, __VMLINUX_SYMBOL_STR(param_ops_bool) },
	{ 0x9b3f3aba, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x5631f43d, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x464f8ad9, __VMLINUX_SYMBOL_STR(usb_autopm_put_interface) },
	{ 0x4e010cf3, __VMLINUX_SYMBOL_STR(input_free_device) },
	{ 0xffdd49ee, __VMLINUX_SYMBOL_STR(class_create_file) },
	{ 0x1cba218d, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x35c1b77, __VMLINUX_SYMBOL_STR(usb_autopm_get_interface) },
	{ 0x8d745737, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xceda89ad, __VMLINUX_SYMBOL_STR(usb_alloc_coherent) },
	{ 0x592e0602, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0xaab3fbae, __VMLINUX_SYMBOL_STR(input_set_abs_params) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xf9c0b663, __VMLINUX_SYMBOL_STR(strlcat) },
	{ 0x5792f848, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x63bab763, __VMLINUX_SYMBOL_STR(input_allocate_device) },
	{ 0xa71f417d, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x380da879, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x16209b87, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0xb223ac0b, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x5f807ccb, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xfc279e4f, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x8c363fbc, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x70d0f033, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x8b439c61, __VMLINUX_SYMBOL_STR(class_remove_file) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xe63359bf, __VMLINUX_SYMBOL_STR(usb_free_coherent) },
	{ 0x4caf5590, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0xe8fe5144, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0x68f6c083, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x2f2f5dcb, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xe2539977, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x908c715f, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x25eb3b76, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6bb9d97b, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v040Bp2230d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "96430FF1BE0483A42529A75");
