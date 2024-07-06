obj-m := timer.o
ccflags-y = -Wno-unused-function -Wno-unused-label -Wno-unused-variable

CONFIG_MODULE_SIG=n
KDIR := ../../../src/linux-5.15.4/

PWD := $(shell pwd)

all: timer.c
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
