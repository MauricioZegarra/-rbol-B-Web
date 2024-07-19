
        Module.onRuntimeInitialized = () => {
            const createTree = Module.cwrap('create_tree', 'number', []);
            const insert = Module.cwrap('Insert', null, ['number', 'number']);
            const remove = Module.cwrap('Remove', null, ['number', 'number']); // Asegúrate de que la función remove esté implementada si la necesitas
            const print = Module.cwrap('Print', 'number', ['number']);
            const deleteTree = Module.cwrap('delete_tree', 'number', ['number']);
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

    document.getElementById('delete').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        deleteTree(tree);
        print(tree);
        tree = null;
        console.log('Tree deleted');
        document.getElementById('tree-container').innerHTML = '';
    });
    function visualizeTree() {
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
