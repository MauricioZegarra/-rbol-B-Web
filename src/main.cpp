#include <iostream>
#include "BTree.h" // Asegúrate de que este archivo incluye Node.h

using namespace std;

int main() {
    int order;
    cout << "Enter the order of the B-Tree: ";
    cin >> order;

    BTree<int> btree(order);

    cout << "Inserting values: 10, 20, 5, 6, 12, 30, 7, 17" << endl;
    btree.Insert(10);
    btree.Insert(20);
    btree.Insert(5);
    btree.Insert(6);
    btree.Insert(12);
    btree.Insert(30);
    btree.Insert(7);
    btree.Insert(17);

    cout << "B-Tree after insertions:" << endl;
    btree.Print();

    cout << "Searching for value 6: " << (btree.Search(6) ? "Found" : "Not Found") << endl;
    cout << "Searching for value 7: " << (btree.Search(7) ? "Found" : "Not Found") << endl;
    cout << "Searching for value 10: " << (btree.Search(10) ? "Found" : "Not Found") << endl;
    cout << "Searching for value 15: " << (btree.Search(15) ? "Found" : "Not Found") << endl;
    cout << "Searching for value 22: " << (btree.Search(22) ? "Found" : "Not Found") << endl;

    cout << "Removing value 6" << endl;
    btree.Remove(6);
    cout << "B-Tree after removing 6:" << endl;
    btree.Print();

    cout << "Removing value 15 (non-existent)" << endl;
    btree.Remove(15);
    cout << "B-Tree after attempting to remove 15:" << endl;
    btree.Print();

    cout << "Removing value 7" << endl;
    btree.Remove(7);
    cout << "B-Tree after removing 7:" << endl;
    btree.Print();

    cout << "Removing value 10" << endl;
    btree.Remove(10);
    cout << "B-Tree after removing 10:" << endl;
    btree.Print();

    return 0;
}
