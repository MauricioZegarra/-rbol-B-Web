#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

template <class T>
struct Node
{
    int NumbersOfKeys; // number of the actual keys
    int position = -1; // to allocate value in the appropriate place
    T *keys;
    Node **childs;
    int Order; // Added to store the order dynamically

    Node(int order);
    int Insert(T value);
    Node *split(Node *node, T *value);
    void Print();
    void PrintUtil(int height, bool checkParent);
    int getHeight();
    ~Node();

    void to_dot(ostringstream &oss, int &nodeCount) const;
    bool Remove(T value);
    bool Search(T value) const;
};

// Node implementation
template <class T>
Node<T>::Node(int order) : Order(order)
{
    this->NumbersOfKeys = 0;
    keys = new T[Order];
    childs = new Node *[Order + 1](); // Initialize with null ptrs
}

template <class T>
int Node<T>::Insert(T value)
{
    if (this->childs[0] == nullptr)
    {
        this->keys[++this->position] = value;
        ++this->NumbersOfKeys;
        for (int i = this->position; i > 0; i--)
        {
            if (this->keys[i] < this->keys[i - 1])
                swap(this->keys[i], this->keys[i - 1]);
        }
    }
    else
    {
        int i = 0;
        for (; i < this->NumbersOfKeys && value > this->keys[i];)
        {
            i++;
        }
        int check = this->childs[i]->Insert(value);
        if (check)
        {
            T mid;
            int TEMP = i;
            Node<T> *newNode = split(this->childs[i], &mid);
            for (; i < this->NumbersOfKeys && mid > this->keys[i];)
            {
                i++;
            }

            for (int j = this->NumbersOfKeys; j > i; j--)
                this->keys[j] = this->keys[j - 1];
            this->keys[i] = mid;

            ++this->NumbersOfKeys;
            ++this->position;

            int k;
            for (k = this->NumbersOfKeys; k > TEMP + 1; k--)
                this->childs[k] = this->childs[k - 1];
            this->childs[k] = newNode;
        }
    }
    if (this->NumbersOfKeys == this->Order)
        return 1;
    else
        return 0;
}

template <class T>
Node<T> *Node<T>::split(Node *node, T *med)
{
    int NumberOfKeys = node->NumbersOfKeys;
    Node<T> *newNode = new Node<T>(Order); // Pass the order when creating a new node
    int midValue = NumberOfKeys / 2;
    *med = node->keys[midValue];
    int i;
    for (i = midValue + 1; i < NumberOfKeys; ++i)
    {
        newNode->keys[++newNode->position] = node->keys[i];
        newNode->childs[newNode->position] = node->childs[i];
        ++newNode->NumbersOfKeys;
        --node->position;
        --node->NumbersOfKeys;
        node->childs[i] = nullptr;
    }
    newNode->childs[newNode->position + 1] = node->childs[i];
    node->childs[i] = nullptr;

    --node->NumbersOfKeys;
    --node->position;
    return newNode;
}

template <class T>
void Node<T>::Print()
{
    int height = this->getHeight();
    for (int i = 1; i <= height; ++i)
    {
        if (i == 1)
            PrintUtil(i, true);
        else
            PrintUtil(i, false);
        cout << endl;
    }
    cout << endl;
}

template <class T>
void Node<T>::PrintUtil(int height, bool checkRoot)
{
    if (height == 1 || checkRoot)
    {
        for (int i = 0; i < this->NumbersOfKeys; i++)
        {
            if (i == 0)
                cout << "|";
            cout << this->keys[i];
            if (i != this->NumbersOfKeys - 1)
                cout << "|";
            if (i == this->NumbersOfKeys - 1)
                cout << "|" << " ";
        }
    }
    else
    {
        for (int i = 0; i <= this->NumbersOfKeys; i++)
        {
            if (this->childs[i])
            {
                this->childs[i]->PrintUtil(height - 1, false);
            }
        }
    }
}

template <class T>
int Node<T>::getHeight()
{
    int COUNT = 1;
    Node<T> *Current = this;
    while (true)
    {
        if (Current->childs[0] == nullptr)
        {
            return COUNT;
        }
        Current = Current->childs[0];
        COUNT++;
    }
}

template <class T>
Node<T>::~Node()
{
    delete[] keys;
    for (int i = 0; i <= this->NumbersOfKeys; ++i)
        delete this->childs[i];
    delete[] childs;
}

template <class T>
void Node<T>::to_dot(ostringstream &oss, int &nodeCount) const
{
    int currentNode = nodeCount++;

    // Create node
    oss << "node" << currentNode << "[label = \"<f0> |";
    for (int i = 0; i < this->NumbersOfKeys; i++)
    {
        oss << this->keys[i] << "|<f" << (i + 1) << "> ";
        if (i < this->NumbersOfKeys - 1)
            oss << "|";
    }
    oss << "\"];\n";

    // Connect children
    for (int i = 0; i <= this->NumbersOfKeys; i++)
    {
        if (this->childs[i])
        {
            int childNode = nodeCount;
            this->childs[i]->to_dot(oss, nodeCount);
            oss << "\"node" << currentNode << "\":f" << i << " -> \"node" << childNode << "\"\n";
        }
    }
}

template <class T>
bool Node<T>::Search(T value) const
{
    int i = 0;
    while (i < NumbersOfKeys && value > keys[i])
        i++;

    if (keys[i] == value)
        return true;

    if (childs[i] == nullptr)
        return false;

    return childs[i]->Search(value);
}

template <class T>
bool Node<T>::Remove(T value)
{
    int i = 0;
    while (i < NumbersOfKeys && keys[i] < value)
        i++;

    if (i < NumbersOfKeys && keys[i] == value)
    {
        if (childs[i] == nullptr)
        {
            for (int j = i + 1; j < NumbersOfKeys; ++j)
                keys[j - 1] = keys[j];
            NumbersOfKeys--;
            position--;
            return true;
        }
        else
        {
            // Handle the case when the node is not a leaf
            // Replace the value with the largest value in the left subtree
            Node<T> *predecessor = childs[i];
            while (predecessor->childs[predecessor->NumbersOfKeys] != nullptr)
                predecessor = predecessor->childs[predecessor->NumbersOfKeys];

            T predecessorValue = predecessor->keys[predecessor->NumbersOfKeys - 1];
            keys[i] = predecessorValue;
            return childs[i]->Remove(predecessorValue);
        }
    }
    else
    {
        if (childs[i] == nullptr)
            return false;
        else
            return childs[i]->Remove(value);
    }
}