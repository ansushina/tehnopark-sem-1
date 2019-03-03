/*
Сушина АПО-13
2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента
 массива A, равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет,
выведите n. Время работы поиска k для каждого элемента B[i]: O(log(k)).
n, m ≤ 10000.
Формат входных данных.
В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.
*/


//#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERR_IO 1
#define ERR_MEMORY 2

//using namespace std;

/**
  Считывание данных из stdin
 * @brief read_all
 * @param a_mas[out] - массив A
 * @param b_mas[out] - массив B
 * @param n[out] - длина массивов
 * @return код ошибки
 */
int read_all(int **a_mas, int **b_mas, int &n, int &m)
{
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m <= 0)
    {
        return ERR_IO;
    }
    int *a = (int *)malloc(n * sizeof(int));
    if (!a)
    {
        return ERR_MEMORY;
    }
    int *b = (int *)malloc(m * sizeof(int));
    if (!b)
    {
        free(a);
        return ERR_MEMORY;
    }
    int rc = OK;
    // правильность ввода гарантируется?
    for (int i = 0; i < n && !rc; i++)
    {
        if (scanf("%d", a + i) != 1 || !(i == 0 || a[i-1] <= a[i]))
        {
            rc = ERR_IO;
        }
    }
    for (int i = 0; i < m && !rc; i++)
    {
        if (scanf("%d", b + i) != 1)
        {
            rc = ERR_IO;
        }
    }
    if (rc != OK)
    {
        free(a);
        free(b);
        return rc;
    }

    *a_mas = a;
    *b_mas = b;
    return rc;
}

/**
  Поиск двух позиций, между которыми находится нужная
  сложность log k
 * @brief find_pos
 * @param a[in] - массив
 * @param n[in] - длина массива
 * @param el[in] - элемент
 * @param beg[out] - начало интервала
 * @param end[out] - конец интервала
 */
void find_pos(int *a, int n, int el, int &beg, int &end)
{
    // если число меньше нулегого элемента, то нулевой - искомый
    if (el <= a[0])
    {
        beg = 0; end = 0;
    }
    int i = 1;
    // будем рассматривать позиции увеличивая каждый раз на 2
    // это гарантируется сложность logk
    while (el > a[i] && i < n)
    {
       i *= 2;
    }
    //если все хорошо
    if (i < n)
    {
        beg = i/2;
        end = i+1;
    }
    else
    {
        //если нет подходящего элемента
        if (el > a[n-1])
        {
            beg = n;
        }
        else
        {
            beg = i/2;
        }
        end = n;
    }

}

/**
  Бинарный поиск подходящей позии на отрезке [beg,end)
 * @brief bin_find
 * @param a[in] - массив
 * @param el[in] - элемент
 * @param beg[in] - начало интервала
 * @param end[in] - конец интервала
 * @return Возвращает индекс подходящего элемента
 */
int bin_find(int *a, int el, int beg, int end)
{
    int mid;
    while (beg < end)
    {
        mid = (beg + end)/2;
        if (a[mid] < el)
            beg = mid + 1;
        else
            end = mid;
    }
    return beg;
}

/**
  Создание массива индексов минимальных элементов больше b[i]
 * @brief process
 * @param a[in] - массив, по которому ищем индексов
 * @param n[in] - длина массива А
 * @param b[in] - массив, для элементов которого ищем индексы
 * @param m[in] -длина массива В
 * @param new_mas[out] - новый массив индексов длиной м
 * @return код ошибки
 */
int process(int *a, int n, int *b, int m, int **new_mas)
{
    int *mas = (int *)malloc(m * sizeof(int));
    if (!mas)
        return ERR_MEMORY;
    int beg, end;

    for (int i = 0; i < m; i++)
    {
        find_pos(a, n, b[i], beg, end);
        mas[i] = bin_find(a, b[i], beg, end);
    }
    *new_mas = mas;
    return OK;
}

/**
  Печать массива
 * @brief print_mas
 * @param a[in] - массив
 * @param n[in] - длина массива
 */
void print_mas(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ",a[i]);
    }
    printf("\n");
}

int main()
{
    //cout << "Hello World!" << endl;
    int n, m;
    int *a, *b;
    int *answer;

    int rc = read_all(&a, &b, n, m);
    if (rc == OK)
    {
        rc = process(a, n, b, m, &answer);
        if (rc == OK)
        {
            print_mas(answer, m);
            free(answer);
        }
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
