#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define THREAD_NUM 1
void my_itos(char *str, pthread_t a);
struct DATA
{
    int **m;
    int row;
    int col;
} a, b, c;

struct WORK
{
    int i;
    int j;
} work;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
void *worker_thread(void *);

int main()
{
    FILE *fp = fopen("./Test_cases/Test_case_1/a.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
    }
    fscanf(fp, "%i", &a.row);
    fscanf(fp, "%i", &a.col);
    FILE *fp2 = fopen("./Test_cases/Test_case_1/b.txt", "r");
    if (fp2 == NULL)
    {
        perror("fopen");
    }
    fscanf(fp2, "%i", &b.row);
    fscanf(fp2, "%i", &b.col);
    c.row = a.row;
    c.col = b.col;
    //  init matrix a b c
    a.m = malloc(sizeof(int *) * a.row);
    b.m = malloc(sizeof(char *) * b.row);
    c.m = malloc(sizeof(char *) * c.row);
    for (int i = 0; i < a.row; i++)
        a.m[i] = malloc(sizeof(int) * a.col);

    for (int i = 0; i < b.row; i++)
        b.m[i] = malloc(sizeof(int) * b.col);

    for (int i = 0; i < c.row; i++)
        c.m[i] = malloc(sizeof(int) * c.col);
    for (int i = 0; i < c.row; i++)
        for (int j = 0; j < c.col; j++)
            c.m[i][j] = 0;
    // read m
    for (int i = 0; i < a.row; i++)
        for (int j = 0; j < a.col; j++)
            fscanf(fp, "%i", a.m[i] + j);
    for (int i = 0; i < b.row; i++)
        for (int j = 0; j < b.col; j++)
            fscanf(fp2, "%i", b.m[i] + j);
    fclose(fp);
    fclose(fp2);
    // start operatoion
    printf("PID:%d\n", getpid());
    pthread_t t[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_create(t + i, NULL, worker_thread, NULL);
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(t[i], NULL);
    }

    // write result
    FILE *fp3 = fopen("./Test_cases/Test_case_1/r.txt", "w");
    if (fp3 == NULL)
    {
        perror("fopen");
    }
    for (int i = 0; i < c.row; i++)
    {
        for (int j = 0; j < c.col; j++)
        {
            fprintf(fp3, "%d ", c.m[i][j]);
        }
        fprintf(fp3, "\n");
    }
}

void *worker_thread(void *ptr)
{
    while (1)
    {
        pthread_mutex_lock(&mutex1);
        if (work.i == -1)
        {
            pthread_mutex_unlock(&mutex1);
            // unsigned long long utime = 77;
            // unsigned long nvcsw = 77;
            // unsigned long nivcsw = 77;
            char *utime;
            char *nvcsw;
            char *nivcsw;
            char pthread_self_str[50];
            my_itos(pthread_self_str, pthread_self());
            printf("worker PID:%s\n", pthread_self_str);
            int proc_fd = open("/proc/my_entry", O_RDWR);
            if (proc_fd < 0)
            {
                perror("proc_fd entry");
            }
            sleep(1);
            if (write(proc_fd, pthread_self_str, strlen(pthread_self_str)) < 0)
            {
                perror("write to proc_fd");
            }
            sleep(1);

            char out_word[100];
            if (read(proc_fd, out_word, 100) < 0)
            {
                perror("write to proc_fd");
            }
            sleep(1);
            if (close(proc_fd) != 0)
            {
                perror("closing proc_fd");
            }
            printf("%s\n", out_word);
            utime = strtok(out_word, ";");
            nvcsw = strtok(NULL, ";");
            nivcsw = strtok(NULL, ";");
            printf("%s\n", utime);
            printf("%s\n", nvcsw);
            printf("%s\n", nivcsw);
            printf("\tThreadID:%ld Time:%s(ms) context switch times:%ld\n", pthread_self(), utime, strtol(nvcsw, NULL, 10) + strtol(nivcsw, NULL, 10));
            pthread_exit(NULL);
        }
        int i, j;
        i = work.i;
        j = work.j;
        work.j++;
        if (work.j == c.col)
        {
            work.j = 0;
            work.i++;
            if (work.i == c.row) // done operation
            {
                work.i = -1;
                // work.j = -1;
            }
        }
        pthread_mutex_unlock(&mutex1);
        for (int k = 0; k < a.col; k++)
        {
            c.m[i][j] += a.m[i][k] * b.m[k][j];
        }
    }
}

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