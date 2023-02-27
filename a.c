#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str1[4] = "123";
    char str2[4] = "111";
    long int sum;
    sum = strtol(str1, NULL, 10) + strtol(str2, NULL, 10);

    printf("%ld\n", sum);
}