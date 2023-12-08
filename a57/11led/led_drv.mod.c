#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2a1826b7, "module_layout" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xe11f6c6c, "class_destroy" },
	{ 0x50c3f86b, "device_destroy" },
	{ 0x6b4b2933, "__ioremap" },
	{ 0xa58e3cf4, "cpu_hwcaps" },
	{ 0x66cfa968, "cpu_hwcap_keys" },
	{ 0x6dfb912f, "arm64_const_caps_ready" },
	{ 0xb4364f80, "device_create" },
	{ 0xf3cd999d, "__class_create" },
	{ 0x655c1341, "__register_chrdev" },
	{ 0x7c32d0f0, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

