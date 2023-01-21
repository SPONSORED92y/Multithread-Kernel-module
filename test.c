#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main()
{
    int proc_fd = open("/proc/my_entry", O_RDWR);
    write(proc_fd, "2", strlen("2"));
    char c[100];
    read(proc_fd, c, 100);
    printf("%s", c);
    close(proc_fd);
}