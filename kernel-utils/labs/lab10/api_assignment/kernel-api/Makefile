obj-m := list.o

CONFIG_MODULE_SIG=n
KDIR := /your/linux/source/path

PWD := $(shell pwd)

all: list.c
	make -C $(KDIR) M=$(PWD) modules
	@cp list.ko ../checker/

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm ../checker/*.ko
