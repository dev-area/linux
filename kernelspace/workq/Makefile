obj-m	:= int2.o


KERNELDIR ?= /opt/i386system/kernel/linux-2.6.20/
PWD       := $(shell pwd)

all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules




clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

