#include <iostream>
#include<stdio.h>

using namespace std;

#define OK 0
#define ERR_IO 1
#define ERR_MEMORY 2
#define TEN 10

/**
  Печать массива
 * @brief print_mas
 * @param a[in] - массив
 * @param n[in] - длина массива
 */
void print_mas(unsigned long long int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%llu ", a[i]);
    }
    printf("\n");
}

int my_pow(int num, int k)
{
    if (k == 0)
        return 1;
    int pr = num;
    for (int i = 0; i < k - 1; i++)
    {
        num *= pr;
    }
    return num;
}
/**
  Считывание данных из stdin
 * @brief read_all
 * @param a_mas[out] - массив A
 * @param b_mas[out] - массив B
 * @param n[out] - длина массивов
 * @return код ошибки
 */
int read_all(unsigned long long int **mas, int &n)
{
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        return ERR_IO;
    }
    unsigned long long int *a = new unsigned long long int[n];
    if (!a)
    {
        return ERR_MEMORY;
    }
    for (int i = 0; i < n; i++)
    {
        if (scanf("%llu", a + i) != 1)
        {
            delete[] a;
            return ERR_IO;
        }
    }
    *mas = a;
    return OK;
}


void CountingSort(unsigned long long int *a, int n, int k)
{

    int c[TEN] = {0};
    for (int i = 0; i < n; i++)
    {
        ++c[(a[i] % my_pow(10, k+1)/ my_pow(10,k))];
    }
    for (int i = 1; i < TEN; i++)
    {
        c[i] += c[i-1];
    }
    unsigned long long int *b = new unsigned long long int[n];
    for (int i = n - 1; i >= 0; i--)
    {
        b[--c[(a[i] % my_pow(10, k+1)/ my_pow(10,k))]] = a[i];
    }
    for (int i = 0; i < n; i++)
        a[i] = b[i];

    //memcpy(a, b ,n * sizeof(int));//странно, но эта функция неправильно пересзаписывает
    delete[] b;
}

int count_num_len(unsigned long long int num)
{
    int i = 0;
    while (num/10 != 0)
    {
        num /= 10;
        i++;
    }
    i++;
    return i;
}

void LSD(unsigned long long int *a, int n)
{
    unsigned long long int max = a[0];
    for (int i = 0; i < n; i++)
    {
        if (a[i] > max)
            max = a[i];
    }
    int max_len = count_num_len(max);

    for (int i = 0; i < max_len; i++)
    {
        CountingSort(a, n, i);
    }
}

int main()
{;
    int n = 0;
    unsigned long long int *a = NULL;

    int rc = read_all(&a, n);
    if (rc != OK)
    {
        printf("[error]");
        return OK;
    }
    LSD(a, n);
    print_mas(a, n);
    delete[] a;
    return OK;
}
