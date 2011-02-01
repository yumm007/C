obj-m		+= usb_bus.o driver.o

all:
	make -C /nfs_rootfs/linux-2.6.29/ M=`pwd` modules

clean:
	make -C /nfs_rootfs/linux-2.6.29/ M=`pwd` modules clean
	rm -rf modules.order

install:
	 make -C /nfs_rootfs/linux-2.6.29/ M=`pwd` modules_install INSTALL_MOD_PATH=/nfs_rootfs/nfsroot-29/
