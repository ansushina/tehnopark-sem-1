#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
const size_t DEFAULT_SIZE=8;
const size_t RESIZE_FACTOR=2;
const double MAX_ALPHA=0.75;

size_t Hash(const std::string s, size_t i, size_t size)
{
    if (i == 0)
    {
         size_t hash = 0;
         for(size_t i = 0; i < s.size(); i++)
         {
            hash = hash*73 + s[i];
         }
         return hash % size;
    }
    else
    {
        return (Hash(s, i-1, size) + i) % size;
    }
}

template <typename T>
class HashTable
{
public:
    HashTable(size_t size = DEFAULT_SIZE):
        table(size, NULL), deleted(size, false), size(0) {}

    ~HashTable()
    {

        for(size_t i = 0; i < table.size(); i++)
        {
            if (table[i])
                delete table[i];
        }
    }
    bool Add(const T &key)
    {
        size_t hash;
        T *node = NULL;
        for (size_t i = 0; i < table.size(); i++)
        {
            hash = Hash(key,i ,table.size());
            node = table[hash];
            if (!node)
                break;
            if (*node == key)
                break;
        }
        if (node)
            return false;
        if (deleted[hash])
            deleted[hash] = false;
        table[hash] = new T;
        *table[hash] = key;
        size++;

        if (((double)size)/table.size() >= MAX_ALPHA)
            grow();
        return true;
    }
    bool Delete(const T &key)
    {
        size_t hash;
        T *node = NULL;

        for (size_t i = 0; i < table.size(); i++)
        {
            hash = Hash(key,i ,table.size());
            node = table[hash];
            if (!node && deleted[hash] == false)
                break;
            if (node && *node == key)
                break;
        }
        if (!node)
            return false;

        deleted[hash] = true;
        table[hash] = NULL;
        delete node;
        size--;
        return true;
    }
    bool Has(const T &key)
    {
        size_t hash;
        T *node = NULL;

        for (size_t i = 0; i < table.size(); i++)
        {
            hash = Hash(key,i ,table.size());
            node = table[hash];
            if (!node && deleted[hash] == false)
                break;
            if (node && *node == key)
                break;
        }
        return node != NULL;
    }
private:
    std::vector<T*> table;
    std::vector<bool> deleted;
    size_t size;

    void grow()
    {
        std::vector<T*> newTable(table.size() * RESIZE_FACTOR, NULL);
        std::vector<bool> newdel(table.size() * RESIZE_FACTOR, false);
        for (size_t i = 0; i < table.size(); i++)
        {
            T *key = table[i];
            if (key != NULL)
            {
                size_t hash;
                T *node = NULL;
                for (size_t j = 0; j < table.size(); j++)
                {
                    hash = Hash(*key,j ,table.size() * RESIZE_FACTOR);
                    node = newTable[hash];
                    if (!node)
                        break;
                }
                newTable[hash] = table[i];
             }
        }
        table.clear();
        deleted.clear();
        for(size_t i = 0; i < newTable.size(); i++)
        {
            table.push_back(newTable[i]);
            deleted.push_back(false);
        }
    }
};

int main()
{
    HashTable<std::string> h;

    char op;
    std::string key;
    while (std::cin >> op >>key)
    {
        if (op == '+')
        {
            std::cout << (h.Add(key) ? "OK" : "FAIL") << std::endl;
        }
        if (op == '-')
        {
            std::cout << (h.Delete(key) ? "OK":"FAIL") << std::endl;
        }
        if (op == '?')
        {
            std::cout << (h.Has(key)?"OK":"FAIL")<<std::endl;
        }
    }
    return 0;
}
