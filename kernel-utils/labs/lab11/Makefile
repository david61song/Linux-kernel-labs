ccflags-y = -Wno-unused-function -Wno-unused-label -Wno-unused-variable -pg -g -Og
obj-m = my_cdev.o

CONFIG_MODULE_SIG=n
KDIR := /home/song/Linux-kernel-labs/kernel-utils/src/linux-6.9.7
PWD := $(shell pwd)


all: my_cdev.c
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean


