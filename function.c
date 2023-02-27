//                 |
// custom function v
static void my_itos(char *str, unsigned long long int a)
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
    if (str[0] == '\0')
    {
        str[0] = '0';
        str[1] = '\0';
    }
    return;
}

static unsigned int my_sizeof(char *str)
{
    unsigned int len = 0;
    for (; str[len] != '\0'; len++)
        ;
    return len + 1;
}

static void my_strcat(char *rt, char *a, char *b, char *c)
{
    // format:<a>;<b>;<c>
    int i = 0;
    int j = 0;
    int k = 0;
    for (; i < my_sizeof(a) - 1; i++)
    {
        rt[i] = a[i];
    }
    rt[i] = ';';
    i++;
    for (; (j + i) < my_sizeof(a) + my_sizeof(b) - 1; j++)
    {
        rt[i + j] = b[j];
    }
    rt[i + j] = ';';
    j++;
    for (; (k + j + i) < my_sizeof(a) + my_sizeof(b) + my_sizeof(b) - 1; k++)
    {
        rt[i + j + k] = c[k];
    }
}

// custom function ^
//                 |