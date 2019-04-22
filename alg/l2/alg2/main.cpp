#include <iostream>
#include <stack>

using namespace std;


template<typename T>
struct Node
{
    Node(): right(NULL), left(NULL){}
    Node(const T &key): key(key),right(NULL), left(NULL){}
    Node(const T &key,Node<T> *right,Node<T> *left): key(key),right(right), left(left){}
    T key;
    Node<T> *right;
    Node<T> *left;
};

template<typename T>

class BinTree
{
public:
    BinTree(): root(NULL){}
    ~BinTree()
    {
        stack <Node<T>*>my_stack;
        Node<T> *tmp = root;
        my_stack.push(tmp);

        while (!my_stack.empty())
        {
            tmp = my_stack.top();
            my_stack.pop();

            if (tmp->right)
                my_stack.push(tmp->right);
            if (tmp->left)
                my_stack.push(tmp->left);
            delete tmp;
        }
    }

    bool Add(const T &key)
    {
        if (is_empty())
        {
            root = new Node<T>;
            root->key = key;
            return true;
        }

        Node<T> *tmp = root;
        Node<T> *prev = NULL;
        while (tmp != NULL)
        {
            prev = tmp;
            if (tmp->key <= key)
            {
                tmp = tmp->right;
            }
            else
            {
                tmp = tmp->left;
            }
        }
        if (prev->key <= key)
        {
            prev->right = new Node<T>(key);
        }
        else
        {
            prev->left = new Node<T>(key);
        }
        return true;
    }
    bool is_empty()
    {
        return root == NULL;
    }

    void print_pre_order()
    {
        stack <Node<T>*>my_stack;
        if (is_empty())
            return;

        Node<T> *tmp = root;
        my_stack.push(tmp);

        while (!my_stack.empty())
        {
            tmp = my_stack.top();
            my_stack.pop();
            std::cout << tmp->key << " " ;

            if (tmp->right)
                my_stack.push(tmp->right);
            if (tmp->left)
                my_stack.push(tmp->left);
        }
        cout << endl;
    }
private:
    Node<T> *root;
};

int main()
{
    int n;
    BinTree<int> tree;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int key;
        cin >> key;
        tree.Add(key);
    }
    tree.print_pre_order();
    return 0;
}
