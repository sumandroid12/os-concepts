obj-m += mod_collatz.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) $(INC) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) $(INC) clean
