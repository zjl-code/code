obj-m += kernel_mem.o
KERNEL_PATH=/opt/kernel/phytium-linux-kernel
all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
