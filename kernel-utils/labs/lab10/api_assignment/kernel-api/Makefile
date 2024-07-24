obj-m := list.o

CONFIG_MODULE_SIG=n
KDIR := /home/song/Linux-kernel-labs/kernel-utils/src/linux-6.9.7

PWD := $(shell pwd)

all: list.c
	make -C $(KDIR) M=$(PWD) modules
	@cp list.ko ../checker/

clean:
	make -C $(KDIR) M=$(PWD) clean
	@rm ../checker/*.ko
