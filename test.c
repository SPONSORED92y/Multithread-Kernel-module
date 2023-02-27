#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main()
{
    char *in_word = "write_in";

    int proc_fd = open("/proc/my_entry", O_RDWR);
    // write(proc_fd, in_word, strlen(in_word));
    char out_word[100];
    read(proc_fd, out_word, 100);
    printf("r:%s\n", out_word);

    close(proc_fd);
}