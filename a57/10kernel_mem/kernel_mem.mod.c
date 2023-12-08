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
	{ 0x4302d0eb, "free_pages" },
	{ 0x37a0cba, "kfree" },
	{ 0x999e8297, "vfree" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x4cab91b3, "kmem_cache_alloc" },
	{ 0xa668535a, "kmalloc_caches" },
	{ 0x7c32d0f0, "printk" },
	{ 0xe4bbc1dd, "kimage_voffset" },
	{ 0x9688de8b, "memstart_addr" },
	{ 0x93fca811, "__get_free_pages" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

