Module.onRuntimeInitialized = () => {
    const createTree = Module.cwrap('create_tree', 'number', ['number']);
    const insert = Module.cwrap('Insert', null, ['number', 'number']);
    const remove = Module.cwrap('remove_node', null, ['number', 'number']);
    const search = Module.cwrap('search_node', 'boolean', ['number', 'number']);
    const print = Module.cwrap('Print', null, ['number']);
    const deleteTree = Module.cwrap('delete_tree', null, ['number']);
    const toDot = Module.cwrap('to_dot', 'string', ['number']);

    let tree = null;

    function limpiar() {
        document.getElementById('key').value = '';
        document.getElementById('eliminarValor').value = '';
        document.getElementById('buscarValor').value = '';
    }

    document.getElementById('create').addEventListener('click', () => {
        const order = parseInt(document.getElementById('maxGrado').value) || 2;
        if (tree) {
            deleteTree(tree);
        }
        tree = createTree(order);
        console.log(`Created B-Tree with order: ${order}`);
        visualizeTree();

        limpiar();
    });

    document.getElementById('botonInsertar').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        const key = parseInt(document.getElementById('key').value);
        insert(tree, key);
        print(tree);
        console.log(`Inserted key: ${key}`);
        visualizeTree();

        limpiar();
    });

    document.getElementById('botonEliminar').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        const key = parseInt(document.getElementById('eliminarValor').value);
        remove(tree, key);
        print(tree);
        console.log(`Removed key: ${key}`);
        visualizeTree();

        limpiar();
    });

    document.getElementById('botonBuscar').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        const key = parseInt(document.getElementById('buscarValor').value);
        const result = search(tree, key);

        document.getElementById('msg').innerText = result ? `Valor ${key} encontrado en el arbol.` : `Valor ${key} no encontrado en el arbol.`;
        console.log(`Searched for key: ${key}, found: ${result}`);

        limpiar();
    });

    document.getElementById('delete').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        tree = null;
        console.log('Tree deleted');
        document.getElementById('tree-container').innerHTML = '';
        document.getElementById('search-result').innerHTML = '';

        limpiar();
    });

    function visualizeTree() {
        if (!tree) return;
        const dot = toDot(tree);
        console.log('DOT Output:', dot);  // Añadir esta línea para depuración
        const viz = new Viz();
        viz.renderSVGElement(dot)
            .then(element => {
                const treeContainer = document.getElementById('tree-container');
                treeContainer.innerHTML = '';
                treeContainer.appendChild(element);
            })
            .catch(error => {
                console.error(error);
            });
    }

    window.addEventListener('unload', () => {
        if (tree) {
            deleteTree(tree);
        }
    });
};
