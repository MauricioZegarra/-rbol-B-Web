#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Node protoType Functions
template <class T, int Order>
struct Node {
    int NumbersOfKeys;  // number of the actual keys
    int position = -1;  // to allocate value in the appropriate place
    T* keys;
    Node** childs;

    Node();
    int Insert(T value);
    Node* split(Node* node, T* value);
    void Print();
    void PrintUtil(int height, bool checkParent);
    int getHeight();
    ~Node();

    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
    //Funciones para eliminar
    int FindKey(T value);
    bool RemoveFromLeaf(int idx);
    bool RemoveFromNonLeaf(int idx);
    T GetPredecessor(int idx);
    T GetSuccessor(int idx);
    void Fill(int idx);
    void BorrowFromPrev(int idx);
    void BorrowFromNext(int idx);
    void Merge(int idx);
    bool Remove(T value); 
};

/////////////////////////////////////////////////////////////

// Node implementation
template <class T, int Order>
Node<T, Order>::Node() {
    this->NumbersOfKeys = 0;
    keys = new T[Order];
    childs = new Node*[Order + 1]();  // Initialize with nullptrs
}

template <class T, int Order>
int Node<T, Order>::Insert(T value) {
    // if the node is leaf
    if (this->childs[0] == nullptr) {
        this->keys[++this->position] = value;
        ++this->NumbersOfKeys;
        // arrange the keys array after putting new value in node
        for (int i = this->position; i > 0; i--) {
            if (this->keys[i] < this->keys[i - 1])
                std::swap(this->keys[i], this->keys[i - 1]);
        }
    } else {  // if the node is not leaf
        // count to get place of child to put the value in it
        int i = 0;
        for (; i < this->NumbersOfKeys && value > this->keys[i];) {
            i++;
        }
        // Check if the child is full to split it
        int check = this->childs[i]->Insert(value);
        // if node full
        if (check) {
            T mid;
            int TEMP = i;
            Node<T, Order>* newNode = split(this->childs[i], &mid);  // Splitted Node to store the values and child that greater than the midValue
            // allocate midValue in correct place
            for (; i < this->NumbersOfKeys && mid > this->keys[i];) {
                i++;
            }

            for (int j = this->NumbersOfKeys; j > i; j--) this->keys[j] = this->keys[j - 1];
            this->keys[i] = mid;

            ++this->NumbersOfKeys;
            ++this->position;

            // allocate newNode Splitted in the correct place
            int k;
            for (k = this->NumbersOfKeys; k > TEMP + 1; k--) this->childs[k] = this->childs[k - 1];
            this->childs[k] = newNode;
        }
    }
    if (this->NumbersOfKeys == Order) return 1;  // to split it
    else return 0;
}

template <class T, int Order>
Node<T, Order>* Node<T, Order>::split(Node* node, T* med)  // mid to store value of mid and use it in insert func
{
    int NumberOfKeys = node->NumbersOfKeys;
    Node<T, Order>* newNode = new Node<T, Order>();
    int midValue = NumberOfKeys / 2;
    *med = node->keys[midValue];
    int i;
    // take the values after mid value
    for (i = midValue + 1; i < NumberOfKeys; ++i) {
        newNode->keys[++newNode->position] = node->keys[i];
        newNode->childs[newNode->position] = node->childs[i];
        ++newNode->NumbersOfKeys;
        --node->position;
        --node->NumbersOfKeys;
        node->childs[i] = nullptr;
    }
    newNode->childs[newNode->position + 1] = node->childs[i];
    node->childs[i] = nullptr;

    --node->NumbersOfKeys;  // because we take mid value...
    --node->position;
    return newNode;
}

template <class T, int Order>
void Node<T, Order>::Print() {
    int height = this->getHeight();  // number of levels  -> log (n)
    for (int i = 1; i <= height; ++i) {  // 50 levels maximum
        // O(n)
        if (i == 1) PrintUtil(i, true);
        else PrintUtil(i, false);
        cout << endl;
    }
    cout << endl;
}

template <class T, int Order>
void Node<T, Order>::PrintUtil(int height, bool checkRoot) {
    // to print all values in the level
    if (height == 1 || checkRoot) {
        for (int i = 0; i < this->NumbersOfKeys; i++) {
            if (i == 0) cout << "|";
            cout << this->keys[i];
            if (i != this->NumbersOfKeys - 1) cout << "|";
            if (i == this->NumbersOfKeys - 1) cout << "|" << " ";
        }
    } else {
        for (int i = 0; i <= this->NumbersOfKeys; i++) {
            if (this->childs[i]) {
                this->childs[i]->PrintUtil(height - 1, false);
            }
        }
    }
}

template <class T, int Order>
int Node<T, Order>::getHeight() {
    int COUNT = 1;
    Node<T, Order>* Current = this;  // current point to root
    while (true) {
        // is leaf
        if (Current->childs[0] == nullptr) {
            return COUNT;
        }
        Current = Current->childs[0];
        COUNT++;
    }
}

// Deallocation
template <class T, int Order>
Node<T, Order>::~Node() {
    delete[] keys;
    for (int i = 0; i <= this->NumbersOfKeys; ++i)
        delete this->childs[i];
    delete[] childs;
}

// Serialización
template <class T, int Order>
void Node<T, Order>::serialize(ofstream& out) const {
    out << NumbersOfKeys << ' ' << position << ' ';
    for (int i = 0; i < NumbersOfKeys; ++i) {
        out << keys[i] << ' ';
    }
    for (int i = 0; i <= NumbersOfKeys; ++i) {
        if (childs[i]) {
            out << 1 << ' ';  // indicates that the child exists
            childs[i]->serialize(out);
        } else {
            out << 0 << ' ';  // indicates that the child is nullptr
        }
    }
}

template <class T, int Order>
void Node<T, Order>::deserialize(ifstream& in) {
    in >> NumbersOfKeys >> position;
    for (int i = 0; i < NumbersOfKeys; ++i) {
        in >> keys[i];
    }
    for (int i = 0; i <= NumbersOfKeys; ++i) {
        int childExists;
        in >> childExists;
        if (childExists) {
            childs[i] = new Node<T, Order>();
            childs[i]->deserialize(in);
        } else {
            childs[i] = nullptr;
        }
    }
}
template <class T, int Order>
bool Node<T, Order>::Remove(T value) {
    int idx = FindKey(value);

    // The key to be removed is present in this node
    if (idx < NumbersOfKeys && keys[idx] == value) {
        // If the node is a leaf node
        if (childs[0] == nullptr)
            return RemoveFromLeaf(idx);
        else
            return RemoveFromNonLeaf(idx);
    } else {
        // If the node is a non-leaf node
        if (childs[0] == nullptr) {
            std::cout << "The key " << value << " is not present in the tree\n";
            return false;
        }

        // The key to be removed is present in the subtree rooted with this node
        bool flag = ((idx == NumbersOfKeys) ? true : false);

        if (childs[idx]->NumbersOfKeys < (Order + 1) / 2)
            Fill(idx);

        if (flag && idx > NumbersOfKeys)
            return childs[idx - 1]->Remove(value);
        else
            return childs[idx]->Remove(value);
    }
    return false;
}

// Additional methods for handling various cases...

template <class T, int Order>
bool Node<T, Order>::RemoveFromLeaf(int idx) {
    for (int i = idx + 1; i < NumbersOfKeys; ++i)
        keys[i - 1] = keys[i];
    NumbersOfKeys--;
    return true;
}

template <class T, int Order>
bool Node<T, Order>::RemoveFromNonLeaf(int idx) {
    T k = keys[idx];

    if (childs[idx]->NumbersOfKeys >= (Order + 1) / 2) {
        T pred = GetPredecessor(idx);
        keys[idx] = pred;
        return childs[idx]->Remove(pred);
    } else if (childs[idx + 1]->NumbersOfKeys >= (Order + 1) / 2) {
        T succ = GetSuccessor(idx);
        keys[idx] = succ;
        return childs[idx + 1]->Remove(succ);
    } else {
        Merge(idx);
        return childs[idx]->Remove(k);
    }
}

template <class T, int Order>
T Node<T, Order>::GetPredecessor(int idx) {
    Node* cur = childs[idx];
    while (!(cur->childs[0] == nullptr))
        cur = cur->childs[cur->NumbersOfKeys];
    return cur->keys[cur->NumbersOfKeys - 1];
}

template <class T, int Order>
T Node<T, Order>::GetSuccessor(int idx) {
    Node* cur = childs[idx + 1];
    while (!(cur->childs[0] == nullptr))
        cur = cur->childs[0];
    return cur->keys[0];
}

template <class T, int Order>
void Node<T, Order>::Fill(int idx) {
    if (idx != 0 && childs[idx - 1]->NumbersOfKeys >= (Order + 1) / 2)
        BorrowFromPrev(idx);
    else if (idx != NumbersOfKeys && childs[idx + 1]->NumbersOfKeys >= (Order + 1) / 2)
        BorrowFromNext(idx);
    else {
        if (idx != NumbersOfKeys)
            Merge(idx);
        else
            Merge(idx - 1);
    }
    return;
}

template <class T, int Order>
void Node<T, Order>::BorrowFromPrev(int idx) {
    Node* child = childs[idx];
    Node* sibling = childs[idx - 1];

    for (int i = child->NumbersOfKeys - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!(child->childs[0] == nullptr)) {
        for (int i = child->NumbersOfKeys; i >= 0; --i)
            child->childs[i + 1] = child->childs[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!(child->childs[0] == nullptr))
        child->childs[0] = sibling->childs[sibling->NumbersOfKeys];

    keys[idx - 1] = sibling->keys[sibling->NumbersOfKeys - 1];

    child->NumbersOfKeys += 1;
    sibling->NumbersOfKeys -= 1;

    return;
}

template <class T, int Order>
void Node<T, Order>::BorrowFromNext(int idx) {
    Node* child = childs[idx];
    Node* sibling = childs[idx + 1];

    child->keys[child->NumbersOfKeys] = keys[idx];

    if (!(child->childs[0] == nullptr))
        child->childs[child->NumbersOfKeys + 1] = sibling->childs[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->NumbersOfKeys; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!(sibling->childs[0] == nullptr)) {
        for (int i = 1; i <= sibling->NumbersOfKeys; ++i)
            sibling->childs[i - 1] = sibling->childs[i];
    }

    child->NumbersOfKeys += 1;
    sibling->NumbersOfKeys -= 1;

    return;
}

template <class T, int Order>
void Node<T, Order>::Merge(int idx) {
    Node* child = childs[idx];
    Node* sibling = childs[idx + 1];

    child->keys[(Order - 1) / 2 - 1] = keys[idx];

    for (int i = 0; i < sibling->NumbersOfKeys; ++i)
        child->keys[i + (Order + 1) / 2] = sibling->keys[i];

    if (!(child->childs[0] == nullptr)) {
        for (int i = 0; i <= sibling->NumbersOfKeys; ++i)
            child->childs[i + (Order + 1) / 2] = sibling->childs[i];
    }

    for (int i = idx + 1; i < NumbersOfKeys; ++i)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= NumbersOfKeys; ++i)
        childs[i - 1] = childs[i];

    child->NumbersOfKeys += sibling->NumbersOfKeys + 1;
    NumbersOfKeys--;

    delete sibling;
    return;
}

template <class T, int Order>
int Node<T, Order>::FindKey(T value) {
    int idx = 0;
    while (idx < NumbersOfKeys && keys[idx] < value)
        ++idx;
    return idx;
}


/////////////////////////////////////////////////////////////

// BTree protoType Function
template <class T, int Order>
class BTree {
private:
    Node<T, Order>* Root;
    int count = 0;  // to count number of elements

public:
    BTree();
    void Insert(T value);
    bool Remove(T value);
    void Print() const;
    ~BTree();

    void serialize(const string& filename) const;
    void deserialize(const string& filename);
};

/////////////////////////////////////////////////////////////

// BTree implementation
template <class T, int Order>
BTree<T, Order>::BTree() {
    this->Root = nullptr;
}

template <class T, int Order>
bool BTree<T, Order>::Remove(T value) {
    if (!Root) {
        std::cout << "The tree is empty\n";
        return false;
    }

    bool result = Root->Remove(value);

    if (Root->NumbersOfKeys == 0) {
        Node<T, Order>* tmp = Root;
        if (Root->childs[0])
            Root = Root->childs[0];
        else
            Root = nullptr;
        delete tmp;
    }
    return result;
}

template <class T, int Order>
void BTree<T, Order>::Insert(T value) {
    count++;
    // if Tree is empty
    if (this->Root == nullptr) {
        this->Root = new Node<T, Order>();
        this->Root->keys[++this->Root->position] = value;
        this->Root->NumbersOfKeys = 1;
    } else {  // if tree not empty
        int check = Root->Insert(value);
        if (check) {
            T mid;
            Node<T, Order>* splittedNode = this->Root->split(this->Root, &mid);
            Node<T, Order>* newNode = new Node<T, Order>();
            newNode->keys[++newNode->position] = mid;
            newNode->NumbersOfKeys = 1;
            newNode->childs[0] = Root;
            newNode->childs[1] = splittedNode;
            this->Root = newNode;
        }
    }
}

template <class T, int Order>
void BTree<T, Order>::Print() const {
    if (Root != nullptr)
        Root->Print();
    else
        cout << "The B-Tree is Empty" << endl;
}

template <class T, int Order>
BTree<T, Order>::~BTree() {
    delete Root;
}

// Serialización
template <class T, int Order>
void BTree<T, Order>::serialize(const string& filename) const {
    ofstream out(filename);
    if (out.is_open()) {
        if (Root) {
            out << 1 << ' ';  // indicates that the root exists
            Root->serialize(out);
        } else {
            out << 0;  // indicates that the tree is empty
        }
        out.close();
    } else {
        cerr << "Unable to open file for writing: " << filename << endl;
    }
}

// Deserialización
template <class T, int Order>
void BTree<T, Order>::deserialize(const string& filename) {
    ifstream in(filename);
    if (in.is_open()) {
        int rootExists;
        in >> rootExists;
        if (rootExists) {
            Root = new Node<T, Order>();
            Root->deserialize(in);
        }
        in.close();
    } else {
        cerr << "Unable to open file for reading: " << filename << endl;
    }
}

#endif // BTREE_H
