#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main()
{
    int proc = open("/proc/my_entry", O_RDWR);
    write(proc, "18", strlen("18"));
    close(proc);
}