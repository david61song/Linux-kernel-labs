all: my_cdev_test.c
	gcc -static -o my_cdev_test my_cdev_test.c
	@# If you want to compile this program as explicitly 32-bit, use below command
	@# gcc -m32 -static -o my_cdev_test my_cdev_test.c

.PHONY: clean

clean:
	-rm -f *~ *.o my_cdev_test
