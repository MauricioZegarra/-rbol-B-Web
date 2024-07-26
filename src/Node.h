#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

template <class T>
struct Node
{
    int NumbersOfKeys; // numero de claves actuales
    int position = -1; // para asignar valores en el lugar correcto
    T *keys;    // Arreglo de claves
    Node **childs;  // Arreglo de punteros a los hijos
    int Order; 
    // Constructor para inicializar un nodo
    Node(int order);
    // Insertar un valor en el árbol
    int Insert(T value);
    // Dividir un nodo
    Node *split(Node *node, T *value);
    void Print();
    void PrintUtil(int height, bool checkParent);
    int getHeight();    // Obtener la altura del árbol
    // Método para convertir el árbol en un archivo .dot
    void to_dot(ostringstream &oss, int &nodeCount) const;
    bool Search(T value) const;

    // Métodos para eliminar un valor del árbol
    bool Remove(T value);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    bool isLeaf() const;
};

// Implementación de los métodos de la estructura Node
template <class T>
Node<T>::Node(int order) : Order(order)
{
    this->NumbersOfKeys = 0;
    keys = new T[Order];
    childs = new Node *[Order + 1](); // Inicializa todos los punteros a nullptr
}
// Implementación del método insert
template <class T>
int Node<T>::Insert(T value)
{
    // Si el nodo es una hoja, insertar el valor en el nodo
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
        //Buscar el hijo en el que se debe insertar el valor
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
// Implementación del método split
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
// Implementación del método Print
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
// Implementación del método PrintUtil para imprimir el árbol
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
// Implementación del método getHeight
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
// Implementación del método to_dot
template <class T>
void Node<T>::to_dot(ostringstream &oss, int &nodeCount) const
{
    int currentNode = nodeCount++;

    // Crea un nodo con las claves
    oss << "node" << currentNode << "[label = \"<f0> |";
    for (int i = 0; i < this->NumbersOfKeys; i++)
    {
        oss << this->keys[i] << "|<f" << (i + 1) << "> ";
        if (i < this->NumbersOfKeys - 1)
            oss << "|";
    }
    oss << "\"];\n";

    // Crea los enlaces a los hijos
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
// Implementación del método Search
template <class T>
bool Node<T>::Search(T value) const
{
    int i = 0;
    while (i < NumbersOfKeys && value > keys[i])
        i++;

    if (i < NumbersOfKeys && keys[i] == value)
        return true;

    if (childs[i] == nullptr)
        return false;

    return childs[i]->Search(value);
}

//////////////////////////////////////////////////////////////////////////////////////////////

// Métodos para eliminar un valor del árbol
// Implementación del método Remove
template <class T>
bool Node<T>::Remove(T value) {
    int idx = 0;
    while (idx < NumbersOfKeys && keys[idx] < value) {
        idx++;
    }

    if (idx < NumbersOfKeys && keys[idx] == value) {
        // Caso 1: El valor está en el nodo actual
        if (childs[0] == nullptr) {
            // Caso 1a: El nodo actual es una hoja
            for (int i = idx + 1; i < NumbersOfKeys; i++) {
                keys[i - 1] = keys[i];
            }
            NumbersOfKeys--;
            return true;
        } else {
            // Caso 1b: El nodo actual no es una hoja
            if (childs[idx]->NumbersOfKeys >= (Order + 1) / 2) {
                // Caso 2a: El predecesor tiene al menos t llaves
                Node<T>* cur = childs[idx];
                while (!cur->isLeaf()) {
                    cur = cur->childs[cur->NumbersOfKeys];
                }
                T pred = cur->keys[cur->NumbersOfKeys - 1];
                keys[idx] = pred;
                return childs[idx]->Remove(pred);
            } else if (childs[idx + 1]->NumbersOfKeys >= (Order + 1) / 2) {
                // Caso 2b: El sucesor tiene al menos t llaves
                Node<T>* cur = childs[idx + 1];
                while (!cur->isLeaf()) {
                    cur = cur->childs[0];
                }
                T succ = cur->keys[0];
                keys[idx] = succ;
                return childs[idx + 1]->Remove(succ);
            } else {
                // Caso 2c: Ambos hijos tienen menos de t llaves
                merge(idx);
                return childs[idx]->Remove(value);
            }
        }
    } else {
        // Caso 3: El valor no está en el nodo actual
        if (childs[0] == nullptr) {
            // El valor no está en el árbol
            return false;
        }
        // Determinar si el hijo donde se supone que se encuentra el valor necesita ser rellenado
        bool flag = ((idx == NumbersOfKeys) ? true : false);
        // Si el hijo donde se supone que se encuentra el valor tiene menos de t llaves, rellenarlo
        if (childs[idx]->NumbersOfKeys < (Order + 1) / 2) {
            fill(idx);
        }

        if (flag && idx > NumbersOfKeys) {
            return childs[idx - 1]->Remove(value);
        } else {
            return childs[idx]->Remove(value);
        }
    }
}

// Métodos para implementar fill
template <class T>
void Node<T>::fill(int idx) {
    if (idx != 0 && childs[idx - 1]->NumbersOfKeys >= (Order + 1) / 2) {
        borrowFromPrev(idx);
    } else if (idx != NumbersOfKeys && childs[idx + 1]->NumbersOfKeys >= (Order + 1) / 2) {
        borrowFromNext(idx);
    } else {
        if (idx != NumbersOfKeys) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
}

// Métodos para implementar borrowFromPrev
template <class T>
void Node<T>::borrowFromPrev(int idx) {
    Node<T>* child = childs[idx];
    Node<T>* sibling = childs[idx - 1];

    for (int i = child->NumbersOfKeys - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->isLeaf()) {
        for (int i = child->NumbersOfKeys; i >= 0; --i) {
            child->childs[i + 1] = child->childs[i];
        }
    }

    child->keys[0] = keys[idx - 1];
    if (!child->isLeaf()) {
        child->childs[0] = sibling->childs[sibling->NumbersOfKeys];
    }

    keys[idx - 1] = sibling->keys[sibling->NumbersOfKeys - 1];

    child->NumbersOfKeys += 1;
    sibling->NumbersOfKeys -= 1;
}

// Métodos para implementar borrowFromNext
template <class T>
void Node<T>::borrowFromNext(int idx) {
    Node<T>* child = childs[idx];
    Node<T>* sibling = childs[idx + 1];

    child->keys[(child->NumbersOfKeys)] = keys[idx];

    if (!(child->isLeaf())) {
        child->childs[(child->NumbersOfKeys) + 1] = sibling->childs[0];
    }

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->NumbersOfKeys; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->isLeaf()) {
        for (int i = 1; i <= sibling->NumbersOfKeys; ++i) {
            sibling->childs[i - 1] = sibling->childs[i];
        }
    }

    child->NumbersOfKeys += 1;
    sibling->NumbersOfKeys -= 1;
}

// Métodos para implementar merge
template <class T>
void Node<T>::merge(int idx) {
    Node<T>* child = childs[idx];
    Node<T>* sibling = childs[idx + 1];

    child->keys[(child->NumbersOfKeys)] = keys[idx];

    for (int i = 0; i < sibling->NumbersOfKeys; ++i) {
        child->keys[i + child->NumbersOfKeys + 1] = sibling->keys[i];
    }

    if (!child->isLeaf()) {
        for (int i = 0; i <= sibling->NumbersOfKeys; ++i) {
            child->childs[i + child->NumbersOfKeys + 1] = sibling->childs[i];
        }
    }

    for (int i = idx + 1; i < NumbersOfKeys; ++i) {
        keys[i - 1] = keys[i];
    }

    for (int i = idx + 2; i <= NumbersOfKeys; ++i) {
        childs[i - 1] = childs[i];
    }

    child->NumbersOfKeys += sibling->NumbersOfKeys + 1;
    NumbersOfKeys--;
}

// Método verificar si un nodo es una hoja
template <class T>
bool Node<T>::isLeaf() const {
    return (childs[0] == nullptr);
}

