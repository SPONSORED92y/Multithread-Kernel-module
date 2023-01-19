#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

void n1();
/*void n2();
void n3();
void n4();
void n8();
void n16();
void n24();
void n32();*/

int main()
{
    int thread_num = 1;
    FILE *fp = fopen("Test_cases/Test_case_1/a.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
    }
    fscanf(fp, "%i", &a.row);
    fscanf(fp, "%i", &a.col);
    FILE *fp2 = fopen("Test_cases/Test_case_1/b.txt", "r");
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
    {
        a.m[i] = malloc(sizeof(int) * a.col);
    }

    for (int i = 0; i < b.row; i++)
    {
        b.m[i] = malloc(sizeof(int) * b.col);
    }

    for (int i = 0; i < c.row; i++)
    {
        c.m[i] = malloc(sizeof(int) * c.col);
    }
    for (int i = 0; i < c.row; i++)
    {
        for (int j = 0; j < c.col; j++)
        {
            c.m[i][j] = 0;
        }
    }
    // read m
    for (int i = 0; i < a.row; i++)
    {
        for (int j = 0; j < a.col; j++)
        {
            fscanf(fp, "%i", a.m[i] + j);
        }
    }

    for (int i = 0; i < b.row; i++)
    {
        for (int j = 0; j < b.col; j++)
        {
            fscanf(fp2, "%i", b.m[i] + j);
        }
    }
    fclose(fp);
    fclose(fp2);
    // start operatoion
    pthread_t *t;
    for (work.i = 0; work.i < c.row; work.i++)
    {
        for (work.j = 0; work.j < c.col; work.j++)
        {
        }
    }
    // write result
    FILE *fp3 = fopen("Test_cases/Test_case_1/r.txt", "w");
    for (int i = 0; i < c.row; i++)
    {
        for (int j = 0; j < c.col; j++)
        {
            fprintf(fp3, "%d ", c.m[i][j]);
        }
        fprintf(fp3, "\n");
    }
}
void n1()
{
    for (int i = 0; i < c.row; i++)
    {
        for (int j = 0; j < c.col; j++)
        {
            for (int k = 0; k < a.col; k++)
            {
                c.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
}