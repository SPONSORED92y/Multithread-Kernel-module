#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#define THREAD_NUM 4

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
    FILE *fp = fopen("../Test_cases/Test_case_1/a.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
    }
    fscanf(fp, "%i", &a.row);
    fscanf(fp, "%i", &a.col);
    FILE *fp2 = fopen("../Test_cases/Test_case_1/b.txt", "r");
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
    FILE *fp3 = fopen("../Test_cases/Test_case_1/r.txt", "w");
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
            int proc = open("\proc\entry", O_RDWR);
            unsigned long long utime = 77;
            unsigned long nvcsw = 77;
            unsigned long nivcsw = 77;
            if (proc < 0)
            {
                perror("proc entry");
            }
            if (fprintf(proc, "%ld", pthread_self()) < 0)
            {
                perror("write to proc");
            }
            if (fscanf(proc, "%lld", utime) < 0)
            {
                perror("fscanf(utime)");
            }
            if (fscanf(proc, "%ld", nvcsw) < 0)
            {
                perror("fscanf(nvcsw)");
            }
            if (fscanf(proc, "%ld", nivcsw) < 0)
            {
                perror("fscanf(nivcsw)");
            }
            close(proc);
            printf("\tThreadID:%ld Time:%lld(ms) context switch times:%ld", pthread_self(), utime, nvcsw + nivcsw);
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
