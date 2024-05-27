CFLAGS=--sysroot $(SYSROOT) -g -Wall -static
LDLIBS=-lrt

CC = ~/Dev/ESY/buildroot/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc-12.3.0.br_real
AS = $(CROSS_COMPILE)as
LAR = $(CROSS_COMPILE)ar
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
RANLIB = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip

obj-m	:= led_one.o

PWD	:= $(shell pwd)

default:
	$(MAKE)	-C $(KDIR)	M=$(PWD) modules	CC=${CC}

clean:
	rm -rf *.ko *.mod *.mod.* *.o modules.order .m* .M* .h* .*.o.d .*.cmd
	rm -rf Module.symvers