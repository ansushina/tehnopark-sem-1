//#include <iostream>

#include <stdio.h>

//using namespace std;

class Deque
{

public:
    Deque();
    ~Deque;
    void pushBack();
    void pushFront();
    int popBack();
    int popFront();
    bool IsEmpty() const;

private:
    int *head, *tail;
    int *mas;
    size_t size;
}

int main()
{
    //cout << "Hello World!" << endl;
    return 0;
}
