ccflags-y=-std=gnu99
obj-m := ki2.o
KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc test2.c -o test2
	gcc stove.c -o stove
	gcc	coache.c -o coache	


clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm test2 stove coache

