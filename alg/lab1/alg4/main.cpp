/*
 * Для сложения чисел используется старый компьютер.
 * Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
 *  Таким образом для нахождения суммы чисел 1,2,3 может потребоваться
 * разное время, в зависимости от порядка вычислений.
 * ((1+2)+3) -> 1+2 + 3+3 = 9
 * ((1+3)+2) -> 1+3 + 4+2 = 10
 * ((2+3)+1) -> 2+3 + 5+1 = 11
 * Требуется написать программу, которая определяет минимальное время,
 * достаточное для вычисления суммы заданного набора чисел.
*/
#include <iostream>
#include <stdio.h>

using namespace std;

class minHeap{
public:
    minHeap(): buffer(nullptr), buffer_size(0),size(0) {}
    ~minHeap()
    {
        delete [] buffer;
    }
    int top() const
    {
        if (isEmpty())
            return -1;
        return buffer[0];
    }

    void push(int num)
    {
        if (buffer_size == 0)
            resize();
        buffer[size] = num;
        siftUp(size);
        size++;
        if (size == buffer_size)
            resize();
    }
    int pop(int i)
    {
        if (i > size)
            return -1;
        if (isEmpty())
            return -1;
        int num = buffer[i];
        buffer[i] = buffer[size - 1];
        //printf("%d ===\n", buffer[size - 1]);
        size--;
        siftDown(i);
        return num;
    }
    bool isEmpty() const
    {
        return size == 0;
    }
private:
    void siftUp(int child)
    {
        int parent = (child-1)/2;
        while (buffer[parent] > buffer[child])
        {
            int swap = buffer[parent];
            buffer[parent] = buffer[child];
            buffer[child] = swap;
            child = parent;
            parent = (child-1)/2;
        }
    }
    void siftDown(int parent)
    {
        int child1 = 2*parent + 1;
        int child2 = 2*parent + 2;

        int child = buffer[child1] < buffer[child2] ? child1 : child2;
        while (buffer[parent] > buffer[child] && child < size)
        {
            int swap = buffer[parent];
            buffer[parent] = buffer[child];
            buffer[child] = swap;
            parent = child;
            child1 = 2*parent + 1;
            child2 = 2*parent + 2;

            child = buffer[child1] < buffer[child2] ? child1 : child2;
        }

    }
    int *buffer;
    int buffer_size;
    int size;
    void resize()
    {
        if (buffer_size == 0)
        {
            buffer_size = 2;
            buffer = new int[2];
            return;
        }

        int *buf = new int[buffer_size * 2];

        for (int i = 0; i < size; i++)
        {
            buf[i] = buffer[i];
        }
        delete [] buffer;
        buffer = buf;
        buffer_size *= 2;
    }
};

/**
  Чтение элементов и запись их в кучу
 * @brief read_all
 * @param heap [in,out] - куча
 * @param n [in,out] - количество элементов
 * @return
 */
int read_all(minHeap &heap, int &n)
{
    if (scanf("%d", &n) != 1 || n < 1)
       return 1;

    for (int i = 0; i < n; i++)
    {
        int num = 0;
        if (scanf("%d", &num) != 1)
        {
            return 1;
        }
        heap.push(num);
    }
    return 0;
}

/**
  Решение задачи: поиск минимального времени выполнения
 * @brief process
 * @param heap[in] - куча
 * @return минимальное времня выполнения(сумма)
 */
int process(minHeap &heap)
{
    int sum = 0;
    int min1 = 0, min2 = 0;
    while (1)
    {
        if (heap.isEmpty())
           break;
        min1 = heap.top();
        heap.pop(0);
        if (heap.isEmpty())
           break;
        min2 = heap.top();
        heap.pop(0);
        sum += min1 + min2;
        heap.push(min1+min2);
    }
    if (sum == 0)
    {
        sum += min1;
    }
    return sum;
}

int main()
{
    int mas_len = 0;
    minHeap myheap;
    if (read_all(myheap,mas_len))
    {
        printf("[error]");
        return 0;
    }
    int sum = process(myheap);
    printf("%d\n", sum);
    return 0;
}
