obj-m := prime_kmod.o
CONFIG_MODULE_SIG=n
KDIR := /home/song/Linux-kernel-labs/kernel-utils/src/linux-6.9.7
PWD := $(shell pwd)

# Add -pg flag for kernel module compilation
EXTRA_CFLAGS += -pg

all : kmod syscall userspace

syscall: prime_syscall.c prime_syscall_test.c
	gcc -o prime_syscall prime_syscall.c prime_syscall_test.c -pg

userspace: prime_user.c
	gcc -o prime_user prime_user.c -pg

kmod: prime_kmod.c # add lkp2.c if necessary
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f prime_syscall
	rm -f prime_user
