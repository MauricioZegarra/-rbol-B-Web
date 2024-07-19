Module.onRuntimeInitialized = () => {
    const createTree = Module.cwrap('create_tree', 'number', ['number']);
    const insert = Module.cwrap('Insert', null, ['number', 'number']);
    const remove = Module.cwrap('remove_node', null, ['number', 'number']);
    const search = Module.cwrap('search_node', 'boolean', ['number', 'number']);
    const print = Module.cwrap('Print', null, ['number']);
    const deleteTree = Module.cwrap('delete_tree', null, ['number']);
    const toDot = Module.cwrap('to_dot', 'string', ['number']);

    let tree = null;

    document.getElementById('create').addEventListener('click', () => {
        const order = parseInt(document.getElementById('maxGrado').value) || 2;
        if (tree) {
            deleteTree(tree);
        }
        tree = createTree(order);
        console.log(`Created B-Tree with order: ${order}`);
        visualizeTree();
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
    });

    document.getElementById('botonBuscar').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        const key = parseInt(document.getElementById('buscarValor').value);
        const result = search(tree, key);
        document.getElementById('msg').innerText = result ? `Key ${key} found in the tree.` : `Key ${key} not found in the tree.`;
        console.log(`Searched for key: ${key}, found: ${result}`);
    });

    document.getElementById('delete').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        deleteTree(tree);
        tree = null;
        console.log('Tree deleted');
        document.getElementById('tree-container').innerHTML = '';
        document.getElementById('search-result').innerHTML = '';
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