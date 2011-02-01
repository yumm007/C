obj-m	+= driver.o

KERN_DIR = /nfs_rootfs/linux-2.6.29

all:
	make -C $(KERN_DIR) M=`pwd` modules

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

