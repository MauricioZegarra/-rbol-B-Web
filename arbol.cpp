#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

template <class T>
struct Node {
    int NumbersOfKeys;
    T* keys;
    Node<T>** childs;
    int order;

    Node(int order) : NumbersOfKeys(0), order(order) {
        keys = new T[order - 1];
        childs = new Node<T>*[order];
        for (int i = 0; i < order; ++i) {
            childs[i] = nullptr;
        }
    }

    ~Node() {
        delete[] keys;
        for (int i = 0; i < order; ++i) {
            delete childs[i];
        }
        delete[] childs;
    }

    int Insert(T value) {
        int i = NumbersOfKeys - 1;
        if (childs[0] == nullptr) {
            while (i >= 0 && keys[i] > value) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = value;
            NumbersOfKeys++;
            return (NumbersOfKeys == order - 1) ? 1 : 0;
        }
        while (i >= 0 && keys[i] > value) {
            i--;
        }
        i++;
        if (childs[i]->Insert(value)) {
            return splitChild(i);
        }
        return 0;
    }

    int splitChild(int index) {
        Node<T>* child = childs[index];
        Node<T>* newChild = new Node<T>(order);
        int mid = (order - 1) / 2;

        newChild->NumbersOfKeys = order - 1 - mid - 1;
        for (int i = 0; i < newChild->NumbersOfKeys; i++) {
            newChild->keys[i] = child->keys[mid + 1 + i];
        }

        if (child->childs[0] != nullptr) {
            for (int i = 0; i <= newChild->NumbersOfKeys; i++) {
                newChild->childs[i] = child->childs[mid + 1 + i];
            }
        }

        child->NumbersOfKeys = mid;

        for (int i = NumbersOfKeys; i > index; i--) {
            childs[i + 1] = childs[i];
        }
        childs[index + 1] = newChild;

        for (int i = NumbersOfKeys - 1; i >= index; i--) {
            keys[i + 1] = keys[i];
        }
        keys[index] = child->keys[mid];
        NumbersOfKeys++;

        return (NumbersOfKeys == order - 1) ? 1 : 0;
    }

    void Print() {
        for (int i = 0; i < NumbersOfKeys; i++) {
            if (childs[0]) {
                childs[i]->Print();
            }
            std::cout << " " << keys[i];
        }
        if (childs[0]) {
            childs[NumbersOfKeys]->Print();
        }
    }

    void PrintUtil(int height, bool checkParent) {
        if (checkParent) {
            std::cout << "\nHeight " << height << ": ";
        }
        for (int i = 0; i < NumbersOfKeys; i++) {
            std::cout << keys[i] << " ";
        }
        if (childs[0]) {
            for (int i = 0; i <= NumbersOfKeys; i++) {
                childs[i]->PrintUtil(height + 1, false);
            }
        }
    }

    int getHeight() {
        if (childs[0]) {
            return 1 + childs[0]->getHeight();
        }
        return 0;
    }

    void serialize(std::ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&NumbersOfKeys), sizeof(NumbersOfKeys));
        out.write(reinterpret_cast<const char*>(keys), NumbersOfKeys * sizeof(T));
        for (int i = 0; i <= NumbersOfKeys; i++) {
            if (childs[i]) {
                bool childExists = true;
                out.write(reinterpret_cast<const char*>(&childExists), sizeof(childExists));
                childs[i]->serialize(out);
            } else {
                bool childExists = false;
                out.write(reinterpret_cast<const char*>(&childExists), sizeof(childExists));
            }
        }
    }

    void deserialize(std::ifstream& in) {
        in.read(reinterpret_cast<char*>(&NumbersOfKeys), sizeof(NumbersOfKeys));
        in.read(reinterpret_cast<char*>(keys), NumbersOfKeys * sizeof(T));
        for (int i = 0; i <= NumbersOfKeys; i++) {
            bool childExists;
            in.read(reinterpret_cast<char*>(&childExists), sizeof(childExists));
            if (childExists) {
                childs[i] = new Node<T>(order);
                childs[i]->deserialize(in);
            } else {
                childs[i] = nullptr;
            }
        }
    }

    void to_dot(std::ostringstream& oss, int& nodeCounter) const {
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
};

template <class T>
class BTree {
private:
    Node<T>* Root;
    int order;

public:
    BTree(int customOrder) : Root(nullptr), order(customOrder) {}

    void Insert(T value) {
        if (Root == nullptr) {
            Root = new Node<T>(order);
            Root->keys[0] = value;
            Root->NumbersOfKeys = 1;
        } else if (Root->NumbersOfKeys == order - 1) {
            Node<T>* newRoot = new Node<T>(order);
            newRoot->childs[0] = Root;
            newRoot->splitChild(0);
            int i = (newRoot->keys[0] < value) ? 1 : 0;
            newRoot->childs[i]->Insert(value);
            Root = newRoot;
        } else {
            Root->Insert(value);
        }
    }

    void Print() const {
        if (Root) {
            Root->Print();
        }
    }

    ~BTree() {
        delete Root;
    }

    void serialize(const string& filename) const {
        ofstream out(filename, ios::binary);
        if (Root) {
            Root->serialize(out);
        }
    }

    void deserialize(const string& filename) {
        ifstream in(filename, ios::binary);
        if (in) {
            Root = new Node<T>(order);
            Root->deserialize(in);
        }
    }

    void to_dot(std::ostringstream& oss) const {
        int nodeCounter = 0;
        oss << "digraph G {\n";
        if (Root) {
            Root->to_dot(oss, nodeCounter);
        }
        oss << "}\n";
    }
};

extern "C" {
    BTree<int>* create_tree(int order) {
        return new BTree<int>(order);
    }

    void Insert(BTree<int>* tree, int value) {
        if (tree) {
            tree->Insert(value);
        }
    }

    void delete_tree(BTree<int>* tree) {
        delete tree;
    }

    char* to_dot(BTree<int>* tree) {
        if (!tree) return nullptr;
        std::ostringstream oss;
        tree->to_dot(oss);
        std::string str = oss.str();
        char* cstr = new char[str.length() + 1];
        std::strcpy(cstr, str.c_str());
        return cstr;
    }

    void free_string(char* str) {
        delete[] str;
    }
}