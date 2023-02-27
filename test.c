#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void my_itos(char *str, pthread_t a)
{
    char temp[50];
    int i1 = 0;
    int i2 = 0;
    int i3 = 0;
    int j = 0;
    for (; i1 < 50; i1++)
    {
        temp[i1] = '\0';
    }
    for (;; i2++)
    {
        if (a == 0)
        {
            break;
        }
        switch (a % 10)
        {
        case 0:
            temp[i2] = '0';
            break;
        case 1:
            temp[i2] = '1';
            break;
        case 2:
            temp[i2] = '2';
            break;
        case 3:
            temp[i2] = '3';
            break;
        case 4:
            temp[i2] = '4';
            break;
        case 5:
            temp[i2] = '5';
            break;
        case 6:
            temp[i2] = '6';
            break;
        case 7:
            temp[i2] = '7';
            break;
        case 8:
            temp[i2] = '8';
            break;
        case 9:
            temp[i2] = '9';
            break;
        }
        a /= 10;
    }

    for (; i3 < 50; i3++)
    {
        if (temp[49 - i3] != '\0')
        {
            str[j] = temp[49 - i3];
            j++;
        }
    }
    for (; j < 50; j++)
    {
        str[j] = '\0';
    }
    if (str[0] == '\0')
    {
        str[0] = '0';
        str[1] = '\0';
    }
    return;
}

int main()
{
    // char *in_word = "2";
    pthread_t n = 2;
    char pthread_self_str[50];
    my_itos(pthread_self_str, n);
    int proc_fd = open("/proc/my_entry", O_RDWR);
    if (proc_fd < 0)
    {
        perror("proc_fd entry");
    }
    write(proc_fd, pthread_self_str, strlen(pthread_self_str));
    char out_word[100];
    read(proc_fd, out_word, 100);
    printf("r:%s\n", out_word);

    close(proc_fd);
}