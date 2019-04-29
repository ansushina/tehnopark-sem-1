#include <iostream>
using namespace std;
template <typename Key>
class AvlTree
{
    struct Node
    {
        Node(const Key &key) : key(key), height(1), children(0), left(NULL), right(NULL) {}
        Key key;
        size_t height;
        size_t children;
        Node *left;
        Node *right;
    };

public:
    AvlTree() : root(NULL) {}
    ~AvlTree()
    {
        destroyTree(root);
    }

public:
    size_t Add_with_position(const Key &key)
    {
        size_t pos = 0;
        root = addInternal(root, key, pos);
        return pos;
    }
    int Remove(const Key &key)
    {
        root = removeInternal(root, key);
    }
    void RemoveByNumver(size_t number)
    {
        Key key = root->key;
        size_t pos = 0;
        root = removeInternal_2(root,pos, number);
    }

private:
    Node *root;

    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *addInternal(Node *node, const Key &key, size_t &pos)
    {
        if (!node)
            return new Node(key);
        node->children++;
        if (node->key <= key)
        {
            node->right = addInternal(node->right, key, pos);
        }
        else
        {
            pos++;
            if (node->right)
                pos += node->right->children+1;
            node->left = addInternal(node->left, key, pos);
        }

        return doBalance(node);
    }

    Node* removeInternal_2(Node *node, size_t &pos, size_t number)
    {
        if (!node)
            return NULL;
        size_t now_pos = pos;
        if (node->right)
        {
            now_pos += node->right->children + 1;
        }
        if (node->children)
            node->children--;
        if (now_pos > number)
            node->right = removeInternal_2(node->right, pos, number);
        else if (now_pos < number)
        {
            pos = now_pos+1;
            node->left = removeInternal_2(node->left, pos, number);
        }
        else
        {
            Node *left = node->left;
            Node *right = node->right;
            size_t children = node->children;

            delete node;

            if (!right)
                return left;

            Node *min = findMin(right);
            min->right = removeMin(right);


            //Node *min = find_and_removeMin(right);
            min->left = left;
            min->children = children;

            return doBalance(min);
        }

        return doBalance(node);
    }

    Node* find_and_removeMin(Node *right)
    {
        Node *node = right;
        if (!node->left)
        {
            return node;
        }
        Node *tmp;
        node->children--;
        if (!node->left->left)
        {
            tmp = node->left;
            node->left = node->left->right;
            node = doBalance(node);
            tmp->right = node;
            return tmp;
        }

        tmp = find_and_removeMin(node->left);
        node = doBalance(node);
        tmp->right = node;
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
        node->children--;
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
        tmp->children += 1;
        if (node->left)
             tmp->children += node->left->children + 1;
        node->children--;
        if (tmp->right)
            node->children -= tmp->right->children + 1;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* rotateRight(Node *node)
    {
        Node *tmp = node->left;
        tmp->children++;
        if (node->right)
            tmp->children += node->right->children + 1;
        node->children--;
        if (tmp->left)
            node->children -= tmp->left->children + 1;
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
            pos = tree.Add_with_position(key);
            cout << pos << endl;
        }
        else if (op == 2)
        {
            tree.RemoveByNumver(key);
        }
    }
    return 0;
}
