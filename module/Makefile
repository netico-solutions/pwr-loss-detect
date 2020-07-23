obj-m += pwr-loss-gpio.o

all:
	make -C $(URTU_KERNEL) M=$(PWD) ARCH=arm CROSS_COMPILE=/opt/gcc-linaro-4.9-2015.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- modules

clean:
	make -C $(URTU_KERNEL) M=$(PWD) ARCH=arm CROSS_COMPILE=/opt/gcc-linaro-4.9-2015.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- clean 
	
