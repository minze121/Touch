
obj-m :=pixcir_hid_tangoc.o

KERNELDIR :=/lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	 rm -rf *.ko *.mod* *.order *vers *~ *.o

