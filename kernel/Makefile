obj-m := hello.o
KERNELDIR :=/home/yu/linux-2.6.22.6

default:
	make -C ${KERNELDIR} M=${shell pwd} modules
clean:
	make -C ${KERNELDIR} M=${shell pwd} clean
	rm Module.symvers

