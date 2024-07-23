#include "BTree.h"

extern "C"
{
    BTree<int> *create_tree(int order)
    {
        return new BTree<int>(order);
    }
    void Print(BTree<int> *tree)
    {
        tree->Print();
    }

    void Insert(BTree<int> *tree, int value)
    {
        tree->Insert(value);
    }

    void delete_tree(BTree<int> *tree)
    {
        delete tree;
    }

    const char *to_dot(BTree<int> *tree)
    {
        ostringstream oss;
        tree->to_dot(oss);
        return strdup(oss.str().c_str());
    }

    bool remove_node(BTree<int> *tree, int value)
    {
        return tree->Remove(value);
    }

    bool search_node(BTree<int> *tree, int value)
    {
        return tree->Search(value);
    }
}