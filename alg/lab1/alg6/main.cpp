#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

int partition(int *arr, int l, int r)
{
    //for (int i = l; i <= r; i++)
      //  printf("%d |", arr[i]);
   // printf("\n");
    int pivot_index = l + rand() % (r-l + 1);
    int num = arr[l];
    arr[l] = arr[pivot_index];
    arr[pivot_index] = num;
    int pivot = arr[l];
    //printf("%d ===\n", pivot);
   // for (int i = l; i <= r; i++)
   //     printf("%d ", arr[i]);
  //  printf("\n");

    int i = r;
    int j = r;

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
      // printf("swap %d %d\n", i+1, j);
      // for (int i = l; i <= r; i++)
     //      printf("%d ", arr[i]);
     //  printf("\n");
       //j--;
    }
  int buf = arr[i];
  arr[i] = pivot;
  arr[l] = buf;
 // for (int i = l; i <= r; i++)
 //     printf("%d ", arr[i]);
//  printf("\n");
//  printf("return %d\n", i);
  return i;
}
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
    //cout << "Hello World!" << endl;
    srand(time(nullptr));
    //int arr[] = {5,2,8,1,9,22};

    int n = 0;
    int k = 0;
    int *arr = read_mas(n, k);

    std::cout << kth_statistic(arr,k,n) << std::endl;
    return 0;
}
