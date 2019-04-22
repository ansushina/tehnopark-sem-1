#include <iostream>
#include <stack>

using namespace std;

template<typename T>
struct Node
{
    Node(): right(NULL), left(NULL){}
    Node(const T &key): key(key),right(NULL), left(NULL){}
    Node(const T &key, int priority): key(key), priority(priority),right(NULL), left(NULL){}
    Node(const T &key,Node<T> *right,Node<T> *left): key(key),right(right), left(left){}
    T key;
    int priority;
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
    size_t tree_len()
    {
        if (is_empty())
            return 0;

        stack <Node<T>*>my_stack;
        stack <size_t>count_stack;
        Node<T> *tmp = root;
        size_t count = 0;
        size_t max = 0;

        while (!my_stack.empty() || tmp != NULL)
        {
            if (tmp != NULL)
            {
                my_stack.push(tmp);
                tmp = tmp->left;
                count_stack.push(count);
                count ++;

            }
            else
            {
                tmp = my_stack.top();
                my_stack.pop();
                count = count_stack.top();
                count_stack.pop();
                count++;
                tmp = tmp->right;

            }
            if (count > max)
                max = count;
        }
        return max;
    }
private:
    Node<T> *root;
};


template<typename T>
class DekartTree
{
public:
    DekartTree(): root(NULL){}
    ~DekartTree()
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
    bool Add(const T &key, int priority)
    {
        if (is_empty())
        {
            root = new Node<T>;
            root->key = key;
            root->priority = priority;
            return true;
        }

        Node<T> *tmp = root;
        Node<T> *prev = NULL;
        while (tmp != NULL && tmp->priority >= priority)
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
        if (tmp == NULL)
        {
            if (prev->key <= key)
            {
                prev->right = new Node<T>(key,priority);
            }
            else
            {
                prev->left = new Node<T>(key,priority);
            }
            return true;
        }

        // нашли позицию
        // нужно разбить на два поддерева
        Node<T> *left = NULL, *right = NULL;
        Split(tmp, key, left, right);

        tmp = new Node<T>(key, priority);
        if (!prev)
        {
            root = tmp;
        }
        else
        {
            if (prev->key <= key)
            {
                prev->right = tmp;
            }
            else
            {
                prev->left = tmp;
            }
        }
        tmp->left = left;
        tmp->right = right;

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
    size_t tree_len()
    {
        if (is_empty())
            return 0;

        stack <Node<T>*>my_stack;
        stack <size_t>count_stack;
        Node<T> *tmp = root;
        size_t count = 0;
        size_t max = 0;

        while (!my_stack.empty() || tmp != NULL)
        {
            if (tmp != NULL)
            {
                my_stack.push(tmp);
                tmp = tmp->left;
                count_stack.push(count);
                count ++;

            }
            else
            {
                tmp = my_stack.top();
                my_stack.pop();
                count = count_stack.top();
                count_stack.pop();
                count++;
                tmp = tmp->right;

            }
            if (count > max)
                max = count;
        }
        return max;
    }
private:
    Node<T> *root;
    void Split(Node<T> *node, const T &key, Node<T> *&left, Node<T> *&right)
    {
        if (!node)
        {
            left = NULL;
            right = NULL;
            return;
        }
        if (node->key <= key)
        {
            Node<T> *tmpleft, *tmpright;
            Split(node->right, key, tmpleft, tmpright);
            left = node;
            left->right = tmpleft;
            right = tmpright;
        }
        else
        {
            Node<T> *tmpleft, *tmpright;
            Split(node->left, key, tmpleft, tmpright);
            right = node;
            right->left= tmpright;
            left = tmpleft;
        }

    }

};
int main()
{
    int n;
    DekartTree<int> tree;
    BinTree<int> bintree;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int key, priority;
        cin >> key >> priority;
        tree.Add(key, priority);
        bintree.Add(key);
    }
    cout<< bintree.tree_len() - tree.tree_len()<<endl;
    return 0;
}
