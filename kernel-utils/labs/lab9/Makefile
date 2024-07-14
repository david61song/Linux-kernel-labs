obj-m := simple_module.o
# obj-m += lkp2.o # add multiple files if necessary

CONFIG_MODULE_SIG=n
KDIR := /home/song/kernel-labs/kernel-utils/src/linux-6.9.7
# KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: simple_module.c # add lkp2.c if necessary
    make -C $(KDIR) M=$(PWD) modules

clean:
    make -C $(KDIR) M=$(PWD) clean
