ifneq ($(KERNELRELEASE),)
obj-m	:= mod1.o mod2.o hello.o hello2.o # mynull.o myzero.o

else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

default:
	$(MAKE)	-C $(KDIR)	M=$(PWD) modules
endif

clean:
	rm -rf *.ko *.mod *.mod.* *.o modules.order .m* .M* .h*
	rm -rf Module.symvers