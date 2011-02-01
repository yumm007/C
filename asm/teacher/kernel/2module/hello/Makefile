obj-m		+= hello.o

all:
	make -C /nfs_rootfs/linux-2.6.29/ M=`pwd` modules

clean:
	make -C /nfs_rootfs/linux-2.6.29/ M=`pwd` modules clean
	rm -rf modules.order
