Para instalar WebAssembly, por favor consultar: https://emscripten.org/docs/getting_started/downloads.html



run server (en carpeta raíz): emrun --no_browser --port 8000 .



compile (en carpeta raíz): emcc src/tree.cpp -o arbol.js -s EXPORTED_FUNCTIONS='["_create_tree", "_Insert", "_delete_tree", "_to_dot", "_Print"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
