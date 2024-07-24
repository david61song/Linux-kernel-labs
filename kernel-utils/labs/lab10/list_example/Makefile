obj-m := list_example.o

CONFIG_MODULE_SIG=n
KDIR := /home/song/Linux-kernel-labs/kernel-utils/src/linux-6.9.7

PWD := $(shell pwd)

all: list_example.c
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
