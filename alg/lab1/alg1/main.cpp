/*
Сушина АД АПО-13

1_1. Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
Необходимо найти первую пару индексов i0 и j0, i0 ≤ j0, такую что
A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
Время работы - O(n).
n ≤ 100000.

*/

//#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//using namespace std;

#define OK 0
#define ERR_IO 1
#define ERR_MEMORY 2

/**
  Считывание данных из stdin
 * @brief read_all
 * @param a_mas[out] - массив A
 * @param b_mas[out] - массив B
 * @param n[out] - длина массивов
 * @return код ошибки
 */
int read_all(int **a_mas, int **b_mas, int &n)
{
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        return ERR_IO;
    }
    int *a = (int *)malloc(n * sizeof(int));
    if (!a)
    {
        return ERR_MEMORY;
    }
    int *b = (int *)malloc(n * sizeof(int));
    if (!b)
    {
        free(a);
        return ERR_MEMORY;
    }

    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", a + i) != 1)
        {
            free(a);
            free(b);
            return ERR_IO;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", b + i) != 1)
        {
            free(a);
            free(b);
            return ERR_IO;
        }
    }
    *a_mas = a;
    *b_mas = b;
    return OK;
}

/**
  Поиск индексов наибольней пары, где i<j
 * @brief process
 * @param a[in] - массив А
 * @param b[in] - массив В
 * @param n[in] - длина массивов
 * @param imax[out] - искомый индекс массива А
 * @param jmax[out] - искомый индекс массива В
 */
void process(int *a, int *b, int n, int &imax, int &jmax)
{
    int amax = 0;
    for (int i = 0; i < n; i++)
    {
        if (a[i] > a[amax])
        {
            amax = i;
        }
        if (a[amax] + b[i] > a[imax] + b[jmax])
        {
            imax = amax;
            jmax = i;
        }

    }
}

int main()
{
    int n;
    int *a, *b;

    int rc = read_all(&a, &b, n);
    if (rc == OK)
    {
        int imax = 0, jmax = 0;
        process(a, b, n, imax, jmax);
        //std::cout << imax << " " << jmax << std::endl;
		printf("%d %d", imax, jmax);
        free(a);
        free(b);
    }
    else
    {
        printf("[error]");
        return 0;
    }


    return 0;
}
