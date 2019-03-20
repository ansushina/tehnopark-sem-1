#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;
/**
  Ставит элемент массива на его место в отстортированном
 * @brief partition
 * @param arr [in] - массив
 * @param l[in] - левый конец отрезка
 * @param r[in] - правый конец отрезка
 * @return индекс элемента
 */
int partition(int *arr, int l, int r)
{
    int pivot_index = l + rand() % (r-l + 1); //рандомный индекс
    //ставим в начало
    int num = arr[l];
    arr[l] = arr[pivot_index];
    arr[pivot_index] = num;

    int pivot = arr[l];

    int i = r; // указывает на последний меньше Pivot
    int j = r; // указывает на начао просмотренного массива
    while (arr[j] > pivot && j >= l)
    {
        j--;
        i--;
    }

    while (j > l)
    {
       while (arr[j] <= pivot && j > l)
       {
           j--;
       }
       if (j == l || i == l)
           break;
       int buf = arr[j];
       arr[j] = arr[i];
       arr[i] = buf;
       i--;
    }
    int buf = arr[i];
    arr[i] = pivot;
    arr[l] = buf;
    return i;
}

/**
  Ищет k-ую статистику
 * @brief kth_statistic
 * @param arr[in] - массив
 * @param k[in] - индекс статистики
 * @param len[in] - длина массива
 * @return k-aя статистика
 */
int kth_statistic(int * arr, int k, int len)
{
    int n = partition(arr,0,len-1);
    int l = 0,r = len-1;
    if (n == k)
        return arr[n];
    while (1)
    {
        if (n == k)
            return arr[n];
        else if (n > k)
        {
            r = n;
        }
        else
        {
            l = n;
        }
        n = partition(arr,l,r);
    }
}

/**
  Чтение массива
 * @brief read_mas
 * @param n[out] - длина массива
 * @param k[out] - номер статистики
 * @return массив
 */
int *read_mas(int &n, int &k)
{
    if (scanf("%d %d", &n, &k) != 2 || n < 1 || k < 0 || k > n)
       return NULL;
    int *mas = new int[n];
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &mas[i]) != 1)
        {
            delete [] mas;
            return NULL;
        }
    }
    return mas;
}

int main()
{
    srand(time(nullptr));

    int n = 0;
    int k = 0;
    int *arr = read_mas(n, k);

    std::cout << kth_statistic(arr,k,n) << std::endl;
    delete [] arr;
    return 0;
}
