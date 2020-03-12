KDIR = /lib/modules/`uname -r`/build
XTRA_CFLAGS = -Wall -g

obj-m+=skarabvethdriver.o
skarabvethdriver-objs := libxdma.o skarabveth.o


all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
