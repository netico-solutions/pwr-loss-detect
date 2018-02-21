obj-m += pwr-loss-gpio.o

all:
		make -C $(URTU_KERNEL) M=$(PWD)

clean:
		make -C $(URTU_KERNEL) M=$(PWD) clean 
