obj-m := list_example.o

CONFIG_MODULE_SIG=n
KDIR := /your/linux/source/path

PWD := $(shell pwd)

all: list_example.c
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
