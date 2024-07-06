obj-m := mymmap.o
ccflags-y = -Wno-unused-function -Wno-unused-label -Wno-unused-variable

EXTRA_CFLAGS=$(ALLOC)
CONFIG_MODULE_SIG=n
KDIR := ../../../src/linux-5.15.4/

PWD := $(shell pwd)


all: mymmap.c
	@echo "extra_flags:$(EXTRA_CFLAGS)"
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
