obj-m += led_drv_on.o
KERNEL_PATH=/opt/kernel/phytium-linux-kernel
all:
	make -C $(KERNEL_PATH) M=$(PWD) modules 
	$(CROSS_COMPILE)gcc -lpthread -o led_app led_app.c 
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
	rm -rf module.order led_app *.o

