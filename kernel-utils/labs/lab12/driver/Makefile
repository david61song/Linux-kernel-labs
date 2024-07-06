obj-m = driver.o

CONFIG_MODULE_SIG=n
KDIR := ../../../src/linux-5.15.4/

PWD := $(shell pwd)

all: driver test

driver: driver.c driver.h
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm -f ioctl_test

test: simple_ioctl.c ioctl_test.c driver.h
	gcc -O0 -static -o ioctl_test ioctl_test.c
	gcc -O0 -static -o simple_test simple_ioctl.c
