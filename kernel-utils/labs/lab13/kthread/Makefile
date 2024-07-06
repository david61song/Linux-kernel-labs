ccflags-y = -Wno-unused-function -Wno-unused-label -Wno-unused-variable
obj-m        = kthread.o

CONFIG_MODULE_SIG=n
KDIR := ../../../src/linux-5.15.4/

PWD := $(shell pwd)

all: kthread.c
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
