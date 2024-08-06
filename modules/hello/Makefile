CFLAGS=--sysroot $(SYSROOT) -g -Wall -static
LDLIBS=-lrt

CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LAR = $(CROSS_COMPILE)ar
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
RANLIB = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip

obj-m	:= hello.o

PWD	:= $(shell pwd)

default:
	$(MAKE)	-C $(KERNELDIR)	M=$(PWD) modules	CC=${CC}

clean:
	rm -rf *.ko *.mod *.mod.* *.o modules.order .m* .M* .h* .*.o.d .*.cmd Module.symvers
