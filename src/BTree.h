#include "Node.h"   // Incluyendo la clase Node
// Clase plantilla para un árbol B  
template <class T>
class BTree
{
private:
    Node<T> *Root;  // Puntero a la raíz del árbol
    int count = 0;  // para contar el número de elementos del árbol
    int Order;     // Orden del árbol B

public:
    BTree(int order);       // Constructor       
    void Insert(T value);
    void Print() const;
    ~BTree();               // Destructor

    void to_dot(ostringstream &oss) const;  // Función para generar el archivo .dot
    bool Search(T value) const;         // Función para buscar un valor
    void Clear(Node<T> *node);          // Función para liberar la memoria
    bool Remove(T value);               // Función para eliminar un valor
};
// Inicialización de árbol B 
template <class T>
BTree<T>::BTree(int order) : Order(order + 1)
{
    this->Root = nullptr;
}

template <class T>
void BTree<T>::Insert(T value)
{
    count++;
    if (this->Root == nullptr)  // Si el árbol está vacío, se crea un nuevo nodo
    {
        this->Root = new Node<T>(this->Order); // Pasar el orden al crear un nuevo nodo
        this->Root->keys[++this->Root->position] = value;   // Inserción
        this->Root->NumbersOfKeys = 1;  // Establecer el número de claves en 1
    }
    else
    {
        int check = Root->Insert(value);
        if (check)  // Si el nodo está lleno, se divide
        {
            T mid;
            Node<T> *splittedNode = this->Root->split(this->Root, &mid);
            Node<T> *newNode = new Node<T>(this->Order); // Pasar el orden al crear un nuevo nodo
            newNode->keys[++newNode->position] = mid;   // Inserción
            newNode->NumbersOfKeys = 1; // Establecer el número de claves en 1
            newNode->childs[0] = Root;  // Asigna el nodo raiz al hijo izquierdo
            newNode->childs[1] = splittedNode;  // Asigna el nodo dividido al hijo derecho
            this->Root = newNode;   // Establecer el nuevo nodo como raíz
        }
    }
}
// Función para imprimir el árbol
template <class T>
void BTree<T>::Print() const
{
    if (this->Root)
        this->Root->Print();
}
// Destructor
template <class T>
BTree<T>::~BTree()
{
    Clear(this->Root);
}
// Función para liberar la memoria
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
// Función para generar el archivo .dot
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
// Función para buscar un valor
template <class T>
bool BTree<T>::Search(T value) const
{
    if (!Root)
        return false;

    return Root->Search(value);
}
// Función para eliminar un valor
template <class T>
bool BTree<T>::Remove(T value) {
    if (!Root) {
        return false;
    }

    Root->Remove(value);
    // Si el nodo raíz está vacío, se elimina
    if (Root->NumbersOfKeys == 0) {
        Node<T>* tmp = Root;
        if (Root->childs[0]) {
            Root = Root->childs[0];
        } else {
            Root = nullptr;
        }
        delete tmp;
    }

    return true;
}
