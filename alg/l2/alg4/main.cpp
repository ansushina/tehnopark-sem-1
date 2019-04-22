#include <iostream>
using namespace std;
template <typename Key>
class AvlTree
{
    struct Node
    {
        Node(const Key &key) : key(key), height(1), left(nullptr), right(nullptr) {}
        Key key;
        size_t height;
        Node *left;
        Node *right;
    };

public:
    AvlTree() : root(nullptr) {}
    ~AvlTree()
    {
        destroyTree(root);
    }

    bool Has(const Key &key) const
    {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->key == key)
            {
                return true;
            }
            else if (tmp->key < key)
            {
                tmp = tmp->right;
            }
            else
            {
                tmp = tmp->left;
            }
        }
        return false;
    }
    void Add(const Key &key)
    {
        root = addInternal(root, key);

    }
    void Remove(const Key &key)
    {
        root = removeInternal(root, key);
    }
    void RemoveByNumver( size_t number)
    {
        Key key = root->key;
        size_t pos = 0;
        find_key(root, key, pos, number);
        Remove(key);
    }
    size_t find_position(const Key &key)
    {
        size_t pos = 0;
        find(root, key, pos);
        return pos;
    }

private:

    Node *root;

    size_t find(Node *node, const Key &key, size_t &pos)
    {
        if (!node)
            return 0;
        if (node->key == key)
            return 1;
        if (find(node->right, key, pos))
            return 1;
        pos++;
        if (find(node->left, key, pos))
            return 1;
        return 0;
    }

    size_t find_key(Node *node, Key &key, size_t &pos, size_t number)
    {

        if (!node)
            return 0;
        if (find_key(node->right, key, pos,number))
            return 1;
        if (pos == number)
        {
            key = node->key;
            return 1;
        }
        pos++;
        if (find_key(node->left, key, pos,number))
            return 1;
        return 0;
    }

    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *addInternal(Node *node, const Key &key)
    {
        if (!node)
            return new Node(key);
        if (node->key <= key)
            node->right = addInternal(node->right, key);
        else
            node->left = addInternal(node->left, key);

        return doBalance(node);
    }

    Node* removeInternal(Node *node, const Key &key)
    {
        if (!node)
            return nullptr;
        if (node->key < key)
            node->right = removeInternal(node->right, key);
        else if (node->key > key)
            node->left = removeInternal(node->left, key);
        else
        {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;

            Node *min = findMin(right);
            min->right = removeMin(right);
            min->left = left;;
            //Node *min = find_and_removeMin(right,left);

            return doBalance(min);
        }

        return doBalance(node);
    }

    Node* find_and_removeMin(Node *right, Node *left)
    {
        Node *node = right;
        Node *tmp;

        if (!node->left->left)
        {
            tmp = node->left;
            node->left = node->left->right;
            node = doBalance(node);
            tmp->right = node;
            tmp->left = left;
            return tmp;
        }
        tmp = find_and_removeMin(node->left, node->left);
        node = doBalance(node);
        tmp->right = node;
        tmp->left = left;
        return tmp;

    }

    Node* findMin(Node *node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* removeMin(Node *node)
    {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node *doBalance(Node *node)
    {
        if (!node)
            return node;
        fixHeight(node);

        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }

    void fixHeight(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    int getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node)
    {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }
};

int main(int argc, const char * argv[]) {
    int n;
    AvlTree<int> tree;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int key, op, pos;
        cin >> op >> key;
        if (op == 1)
        {
            tree.Add(key);
            pos = tree.find_position(key);
            cout << pos << endl;
        }
        else if (op == 2)
        {
            tree.RemoveByNumver(key);
        }

        //bintree.Add(key);
    }
    return 0;
}
