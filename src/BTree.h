#include "Node.h"

template <class T>
class BTree
{
private:
    Node<T> *Root;
    int count = 0; // to count number of elements
    int Order;     // Added to store the order of the tree

public:
    BTree(int order);
    void Insert(T value);
    void Print() const;
    ~BTree();

    void to_dot(ostringstream &oss) const;
    bool Remove(T value);
    bool Search(T value) const;
    void Clear(Node<T> *node);
};

template <class T>
BTree<T>::BTree(int order) : Order(order)
{
    this->Root = nullptr;
}

template <class T>
void BTree<T>::Insert(T value)
{
    count++;
    if (this->Root == nullptr)
    {
        this->Root = new Node<T>(this->Order); // Pass the order when creating a new node
        this->Root->keys[++this->Root->position] = value;
        this->Root->NumbersOfKeys = 1;
    }
    else
    {
        int check = Root->Insert(value);
        if (check)
        {
            T mid;
            Node<T> *splittedNode = this->Root->split(this->Root, &mid);
            Node<T> *newNode = new Node<T>(this->Order); // Pass the order when creating a new node
            newNode->keys[++newNode->position] = mid;
            newNode->NumbersOfKeys = 1;
            newNode->childs[0] = Root;
            newNode->childs[1] = splittedNode;
            this->Root = newNode;
        }
    }
}

template <class T>
void BTree<T>::Print() const
{
    if (this->Root)
        this->Root->Print();
}

template <class T>
BTree<T>::~BTree()
{
    Clear(Root);
}

template <class T>
void BTree<T>::Clear(Node<T> *node)
{
    if (node)
    {
        for (int i = 0; i <= node->NumbersOfKeys; i++)
        {
            Clear(node->childs[i]);
        }
        delete node;
    }
}

template <class T>
void BTree<T>::to_dot(ostringstream &oss) const
{
    oss << "digraph g {\n";
    oss << "node [shape = record,height=.1];\n";
    int nodeCount = 0;
    if (Root)
    {
        Root->to_dot(oss, nodeCount);
    }
    oss << "}\n";
}

template <class T>
bool BTree<T>::Remove(T value)
{
    if (!Root)
        return false;

    bool result = Root->Remove(value);

    if (Root->NumbersOfKeys == 0)
    {
        Node<T> *temp = Root;
        if (Root->childs[0] == nullptr)
            Root = nullptr;
        else
            Root = Root->childs[0];
        delete temp;
    }

    return result;
}

template <class T>
bool BTree<T>::Search(T value) const
{
    if (!Root)
        return false;

    return Root->Search(value);
}