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
	{ 0x1292e7d3, "module_layout" },
	{ 0x1976aa06, "param_ops_bool" },
	{ 0xc847772c, "usb_deregister" },
	{ 0xad8435e3, "usb_register_driver" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x2af6eb65, "usb_control_msg" },
	{ 0x274de7cb, "__mutex_init" },
	{ 0xb93739e7, "input_event" },
	{ 0xc5c1c825, "usb_autopm_put_interface" },
	{ 0x4d688ba9, "input_free_device" },
	{ 0xd93d6140, "class_create_file" },
	{ 0x52e79da5, "__class_create" },
	{ 0xbc58e5fb, "usb_autopm_get_interface" },
	{ 0xe113c7b2, "usb_alloc_urb" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x3daa9dee, "usb_alloc_coherent" },
	{ 0xe2bccfed, "input_register_device" },
	{ 0xf753c120, "input_set_abs_params" },
	{ 0x28318305, "snprintf" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x5792f848, "strlcpy" },
	{ 0x10a30667, "input_allocate_device" },
	{ 0xef5cc865, "kmem_cache_alloc_trace" },
	{ 0x570eaaf0, "kmalloc_caches" },
	{ 0x273ef62f, "dev_err" },
	{ 0xa1d0fd00, "_dev_info" },
	{ 0x7f26ebfb, "class_destroy" },
	{ 0x36d0cdf5, "class_remove_file" },
	{ 0x37a0cba, "kfree" },
	{ 0xc4a48996, "usb_free_coherent" },
	{ 0xc8830225, "usb_free_urb" },
	{ 0x9785dd17, "input_unregister_device" },
	{ 0x4ee7021c, "dev_set_drvdata" },
	{ 0xe8f6705b, "usb_kill_urb" },
	{ 0xe7180a8b, "mutex_unlock" },
	{ 0x9a2fd821, "mutex_lock" },
	{ 0xf576ae39, "dev_get_drvdata" },
	{ 0x5002560, "usb_submit_urb" },
	{ 0x7d11c268, "jiffies" },
	{ 0x27e1a049, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v040Bp2230d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "2E5AC8E8C637D9F60D25431");
