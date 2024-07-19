run server: emrun --no_browser --port 8000 .
compile: emcc arbol.cpp -o arbol.js -s EXPORTED_FUNCTIONS='["_create_tree", "_Insert", "_delete_tree", "_to_dot", "_Print"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
