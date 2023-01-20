obj-m += procfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=/mnt/c/src/OS_HW2_Multithread-Kernel-module modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=/mnt/c/src/OS_HW2_Multithread-Kernel-module clean