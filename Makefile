obj-m += pwr-loss-gpio.o

all:
	make -C $(URTU_KERNEL) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules

clean:
	make -C $(URTU_KERNEL) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- clean 
	
