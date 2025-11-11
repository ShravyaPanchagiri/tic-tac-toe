obj-m += tictactoe.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


all:
	@echo "Building kernel module...."
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	@echo "Building user test program..."
	gcc test_user.c -o test_user

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f test_user