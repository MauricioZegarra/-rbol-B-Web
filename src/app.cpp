#include "B-Tree.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <comando> [<valor>]" << endl;
        cerr << "Comandos:  insert, remove" << endl;
        return 1;
    }

    BTree<string, 5> btree;
    btree.deserialize("btree.dat");  // Cargar el árbol desde el archivo

    string command = argv[1];

    if (command == "insert" && argc == 3) {
        string value = argv[2];
        btree.Insert(value);
        btree.serialize("btree.dat");  // Guardar el árbol en el archivo
    } 
    else if (command == "remove" && argc == 3) { 
        string value = argv[2];
        btree.Remove(value);
        btree.serialize("btree.dat");  // Guardar el árbol en el archivo
    }
    else{
        cerr << "Comando no reconocido o número incorrecto de argumentos." << endl;
        return 1;
    }

    btree.Print();  // Imprimir el árbol en preorden

    return 0; 
}
