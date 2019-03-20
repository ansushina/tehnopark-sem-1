#include <iostream>
#include<stdio.h>

using namespace std;

#define OK 0
#define ERR_IO 1
#define ERR_MEMORY 2
#define BYTE 255

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
    if (scanf("%d", &n) != 1 || n <= 0 || n > 1000000)
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

/**
  Сортировка подсчетом для k-ого байта
 * @brief CountingSort
 * @param a [in] - массива
 * @param n [in] - длина массива
 * @param k [in] - номер байта, по которому сортируем
 */
void CountingSort(unsigned long long int *a, int n, int k)
{

    int c[BYTE + 1] = {0};
    for (int i = 0; i < n; i++)
    {
        ++c[(a[i]>>(k*8))&BYTE];
    }
    for (int i = 1; i < BYTE+1; i++)
    {
        c[i] += c[i-1];
    }
    unsigned long long int *b = new unsigned long long int[n];
    for (int i = n - 1; i >= 0; i--)
    {
        b[--c[(a[i]>>(k*8))&BYTE]] = a[i];
    }
    for (int i = 0; i < n; i++)
    {
        a[i] = b[i];
    }
    delete[] b;
}

/**
  Подсчет количества байтов
 * @brief count_num_len
 * @param num[in] - число
 * @return количество байтов
 */
int count_num_len(unsigned long long int num)
{
    int i = 0;
    while(num)
    {
        num >>= 8;
        i++;
    }
    return i;
}

/**
  Побайтовая сортировка LSD
 * @brief LSD
 * @param a[in] - массив
 * @param n[in] - длина массива
 */
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
{
    int n = 0;
    unsigned long long int *a = nullptr;

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
