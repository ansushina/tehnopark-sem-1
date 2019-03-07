#include <iostream>

#include <stdio.h>
#include <stdlib.h>

//using namespace std;

class Deque
{
public:
    Deque() : head (0), tail (0), mas (nullptr), size (0) {}
    ~Deque()
    {
        delete[] mas;
    }
    void pushBack(int i)
    {
        if (mas == nullptr)
        {
             resize();
        }

        mas[tail] = i;
        tail = (tail+1)%size;
        if (tail == head)
        {
            resize();
        }
       /* for (size_t var = 0; var < size; ++var)
        {
            printf("%d ", mas[var]);

        }
        printf("\n");*/
    }
    void pushFront(int i)
    {
        if (mas == nullptr)
        {
             resize();
        }

        head = (head-1)%size;
        if (tail == head)
        {
            resize();
        }

        mas[head] = i;
        /*for (size_t var = 0; var < size; ++var)
        {
            printf("%d ", mas[var]);

        }
        printf("\n");*/
    }
    int popBack()
    {
        if (IsEmpty())
            return -1;
        tail = (tail-1)%size;
        int el = mas[tail];

        return el;

        /*for (size_t var = 0; var < size; ++var)
        {
            printf("%d ", mas[var]);

        }
        printf("\n");*/
    }
    int popFront()
    {
        if (IsEmpty())
            return -1;
        int el = mas[head];
        head = (head+1)%size;

        return el;

    }
    bool IsEmpty() const
    {
        return (head == tail);
    }

protected:

private:
    size_t head, tail;
    int *mas;
    size_t size;
    void resize()
    {
        if (size == 0)
        {
            size = 2;
            mas = new int[2];
            //mas = (int *)malloc(2*sizeof(int));
            return;
        }


        int *buf = new int[size*2];

        size_t count = 0;
        if (head >= tail)
        {
            for (size_t i = head; i < size; i++)
            {
                buf[count] = mas[i];
                count++;
            }
            for (size_t i = 0;i < tail; i++)
            {
                buf[count] = mas[i];
                count++;
            }
        }
        else
        {
            for (size_t i = head; i < tail; i++)
            {
                buf[count] = mas[i];
                count++;
            }
        }

        head = 0;
        tail = count;
        delete[] mas;
        mas = buf;
        size *= 2;

    }
};

int main()
{
    Deque deque;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        int op = 0, val = 0;
        std::cin >> op >> val;

        switch (op)
        {
            case 3:
            {
                deque.pushBack(val);
                break;
            }
            case 4:
            {
                int  qValue = deque.popBack();
                if (qValue != val)
                {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
                break;
            }
            case 2:
            {
                int  qValue = deque.popFront();
                if (qValue != val)
                {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
                break;
            }
            case 1:
            {
                deque.pushFront(val);
                break;
            }

        }
    }
    std::cout << "YES" << std::endl;
   // ~queue();
    return 0;
}
