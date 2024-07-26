#include "BTree.h"

extern "C"
{
    // Crear un árbol B con determinado orden
    BTree<int> *create_tree(int order)
    {
        return new BTree<int>(order);
    }
    // Imprimir el árbol B
    void Print(BTree<int> *tree)
    {
        tree->Print();
    }
    // Función de inserción de un valor en el árbol B
    void Insert(BTree<int> *tree, int value)
    {
        tree->Insert(value);
    }
    // Eliminación y liberación de memoria del árbol B
    void delete_tree(BTree<int> *tree)
    {
        tree->~BTree();
    }
    // Función para convertir el árbol B en un archivo .dot
    const char *to_dot(BTree<int> *tree)
    {
        ostringstream oss;
        tree->to_dot(oss);
        return strdup(oss.str().c_str());   // Convierte la salida a una cadena C
    }
    // Dado un valor, eliminar un nodo del árbol B
    bool remove_node(BTree<int> *tree, int value)
    {
        return tree->Remove(value);
    }
    // Dado un valor, buscar un nodo en el árbol B
    bool search_node(BTree<int> *tree, int value)
    {
        return tree->Search(value);
    }
}
