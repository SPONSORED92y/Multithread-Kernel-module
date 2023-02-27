make clean
make && sudo rmmod my_procfs2.ko && sudo insmod my_procfs2.ko && gcc test.c && ./a.out && sudo journalctl -an 70 | grep -a kernel