#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
    void to_dot(std::ostringstream& oss, int& nodeCounter) const;  // Añadido
};

// Node implementation
template <class T, int Order>
Node<T, Order>::Node() {
    this->NumbersOfKeys = 0;
    keys = new T[Order];
    childs = new Node*[Order + 1]();  // Initialize with nullptrs
}

template <class T, int Order>
int Node<T, Order>::Insert(T value) {
    if (this->childs[0] == nullptr) {
        this->keys[++this->position] = value;
        ++this->NumbersOfKeys;
        for (int i = this->position; i > 0; i--) {
            if (this->keys[i] < this->keys[i - 1])
                std::swap(this->keys[i], this->keys[i - 1]);
        }
    } else {
        int i = 0;
        for (; i < this->NumbersOfKeys && value > this->keys[i];) {
            i++;
        }
        int check = this->childs[i]->Insert(value);
        if (check) {
            T mid;
            int TEMP = i;
            Node<T, Order>* newNode = split(this->childs[i], &mid);
            for (; i < this->NumbersOfKeys && mid > this->keys[i];) {
                i++;
            }

            for (int j = this->NumbersOfKeys; j > i; j--) this->keys[j] = this->keys[j - 1];
            this->keys[i] = mid;

            ++this->NumbersOfKeys;
            ++this->position;

            int k;
            for (k = this->NumbersOfKeys; k > TEMP + 1; k--) this->childs[k] = this->childs[k - 1];
            this->childs[k] = newNode;
        }
    }
    if (this->NumbersOfKeys == Order) return 1;
    else return 0;
}

template <class T, int Order>
Node<T, Order>* Node<T, Order>::split(Node* node, T* med) {
    int NumberOfKeys = node->NumbersOfKeys;
    Node<T, Order>* newNode = new Node<T, Order>();
    int midValue = NumberOfKeys / 2;
    *med = node->keys[midValue];
    int i;
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

    --node->NumbersOfKeys;
    --node->position;
    return newNode;
}

template <class T, int Order>
void Node<T, Order>::Print() {
    int height = this->getHeight();
    for (int i = 1; i <= height; ++i) {
        if (i == 1) PrintUtil(i, true);
        else PrintUtil(i, false);
        cout << endl;
    }
    cout << endl;
}

template <class T, int Order>
void Node<T, Order>::PrintUtil(int height, bool checkRoot) {
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
    Node<T, Order>* Current = this;
    while (true) {
        if (Current->childs[0] == nullptr) {
            return COUNT;
        }
        Current = Current->childs[0];
        COUNT++;
    }
}

template <class T, int Order>
Node<T, Order>::~Node() {
    delete[] keys;
    for (int i = 0; i <= this->NumbersOfKeys; ++i)
        delete this->childs[i];
    delete[] childs;
}

template <class T, int Order>
void Node<T, Order>::serialize(ofstream& out) const {
    out << NumbersOfKeys << ' ' << position << ' ';
    for (int i = 0; i < NumbersOfKeys; ++i) {
        out << keys[i] << ' ';
    }
    for (int i = 0; i <= NumbersOfKeys; ++i) {
        if (childs[i]) {
            out << 1 << ' ';
            childs[i]->serialize(out);
        } else {
            out << 0 << ' ';
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
void Node<T, Order>::to_dot(std::ostringstream& oss, int& nodeCounter) const {
    int currentId = nodeCounter++;
    oss << "node" << currentId << " [label=\"";
    for (int i = 0; i < NumbersOfKeys; ++i) {
        if (i > 0) oss << "|";
        oss << "<f" << i << ">" << keys[i];
    }
    oss << "\"];\n";
    
    for (int i = 0; i <= NumbersOfKeys; ++i) {
        if (childs[i]) {
            int childId = nodeCounter;
            childs[i]->to_dot(oss, nodeCounter);
            oss << "node" << currentId << ":f" << i << " -> node" << childId << ";\n";
        }
    }
}

template <class T, int Order>
class BTree {
private:
    Node<T, Order>* Root;
    int count = 0;  // to count number of elements

public:
    BTree();
    void Insert(T value);
    void Print() const;
    ~BTree();

    void serialize(const string& filename) const;
    void deserialize(const string& filename);
    void to_dot(std::ostringstream& oss) const;
};

template <class T, int Order>
BTree<T, Order>::BTree() {
    this->Root = nullptr;
}

template <class T, int Order>
void BTree<T, Order>::Insert(T value) {
    count++;
    if (this->Root == nullptr) {
        this->Root = new Node<T, Order>();
        this->Root->keys[++this->Root->position] = value;
        this->Root->NumbersOfKeys = 1;
    } else {
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
    if (this->Root) this->Root->Print();
}

template <class T, int Order>
BTree<T, Order>::~BTree() {
    delete this->Root;
}

template <class T, int Order>
void BTree<T, Order>::serialize(const string& filename) const {
    ofstream out(filename);
    Root->serialize(out);
    out.close();
}

template <class T, int Order>
void BTree<T, Order>::deserialize(const string& filename) {
    ifstream in(filename);
    Root = new Node<T, Order>();
    Root->deserialize(in);
    in.close();
}

template <class T, int Order>
void BTree<T, Order>::to_dot(std::ostringstream& oss) const {
    oss << "digraph G {\nnode [shape=record];\n";
    int nodeCounter = 0;
    if (Root) {
        Root->to_dot(oss, nodeCounter);
    }
    oss << "}\n";
}

extern "C" {
    BTree<int, 3>* create_tree() {
        return new BTree<int, 3>();
    }

    void Insert(BTree<int, 3>* tree, int value) {
        tree->Insert(value);
    }

    void delete_tree(BTree<int, 3>* tree) {
        delete tree;
    }

    const char* to_dot(BTree<int, 3>* tree) {
        std::ostringstream oss;
        tree->to_dot(oss);
        return strdup(oss.str().c_str());
    }
}

