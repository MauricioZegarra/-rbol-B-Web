Module.onRuntimeInitialized = () => {
    const createTree = Module.cwrap('create_tree', 'number', ['number']);
    const insert = Module.cwrap('Insert', null, ['number', 'number']);
    const deleteTree = Module.cwrap('delete_tree', null, ['number']);
    const toDot = Module.cwrap('to_dot', 'number', ['number']);
    const freeString = Module.cwrap('free_string', null, ['number']);

    let tree = null;

    document.getElementById('create').addEventListener('click', () => {
        const order = parseInt(document.getElementById('order').value) || 2;
        if (tree) {
            deleteTree(tree);
        }
        tree = createTree(order);
        console.log(`Created B-Tree with order: ${order}`);
        visualizeTree();
    });

    document.getElementById('insert').addEventListener('click', () => {
        if (!tree) {
            alert('Create a tree first!');
            return;
        }
        const key = parseInt(document.getElementById('key').value);
        insert(tree, key);
        console.log(`Inserted key: ${key}`);
        visualizeTree();
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
    });

    function visualizeTree() {
        if (!tree) {
            return;
        }
        const dotPtr = toDot(tree);
        if (dotPtr) {
            const dot = Module.UTF8ToString(dotPtr);
            freeString(dotPtr);
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
    }

    window.addEventListener('unload', () => {
        if (tree) {
            deleteTree(tree);
        }
    });
};