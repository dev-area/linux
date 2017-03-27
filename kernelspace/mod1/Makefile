
obj-m	:= simple.o

KERNELDIR ?= /opt/armsystem/linux-2.6.32/
PWD       := $(shell pwd)

all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

