

obj-m	:= simproc.o


KERNELDIR ?= /home/developer/armsystem/kernel/linux-3.10
PWD       := $(shell pwd)
all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

