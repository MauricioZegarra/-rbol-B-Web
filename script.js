// Elementos del archvo html
const maxGradoInput = document.getElementById('maxGrado');
const insertarValorInput = document.getElementById('insertarValor');
const eliminarValorInput = document.getElementById('eliminarValor');
const buscarValorInput = document.getElementById('buscarValor');
const botonInsertar = document.getElementById('botonInsertar');
const botonEliminar = document.getElementById('botonEliminar');
const botonBuscar = document.getElementById('botonBuscar');
const botonDibujar = document.getElementById('botonDibujar');
const botonAnimar = document.getElementById('botonAnimar');
const sliderVelocidad = document.getElementById('sliderVelocidad');
const menuToggle = document.getElementById('menuToggle');
const shareButton = document.getElementById('shareButton');

// Configuración del árbol B
let arbolB = {
    gradoMaximo: 3,
    nodos: []
};

// Funciones de manejo de eventos
function manejarInsercion() {
    const valor = parseInt(insertarValorInput.value);
    if (!isNaN(valor)) {
        // Aquí se llamaría a la función de inserción del backend C++
        console.log(`Insertando valor: ${valor}`);
        insertarValorInput.value = '';
        actualizarVisualizacionArbol();
    }
}

function manejarEliminacion() {
    const valor = parseInt(eliminarValorInput.value);
    if (!isNaN(valor)) {
        // Aquí se llamaría a la función de eliminación del backend C++
        console.log(`Eliminando valor: ${valor}`);
        eliminarValorInput.value = '';
        actualizarVisualizacionArbol();
    }
}

function manejarBusqueda() {
    const valor = parseInt(buscarValorInput.value);
    if (!isNaN(valor)) {
        // Aquí se llamaría a la función de búsqueda del backend C++
        console.log(`Buscando valor: ${valor}`);
        buscarValorInput.value = '';
        // Actualizar la visualización para mostrar el resultado de la búsqueda
    }
}

function manejarDibujarArbol() {
    // Aquí se llamaría a la función para obtener la estructura actual del árbol del backend C++
    console.log('Dibujando árbol');
    actualizarVisualizacionArbol();
}

function manejarAnimarArbol() {
    const velocidad = sliderVelocidad.value;
    console.log(`Iniciando animación con velocidad: ${velocidad}`);
    // ls lógica de la animación
}

function actualizarVisualizacionArbol() {
    const visualizacionArbol = document.getElementById('visualizacionArbol');
    // Aquí se implementaría la lógica para dibujar el árbol 
    visualizacionArbol.innerHTML = 'Arbolito generado...';
}

function alternarMenu() {
    const sidebar = document.getElementById('sidebar');
    sidebar.style.display = sidebar.style.display === 'none' ? 'block' : 'none';
}

function compartirArbol() {
    // Implementar la lógica para compartir el árbol
    console.log('Compartiendo árbol');
}

// Listeners de eventos
botonInsertar.addEventListener('click', manejarInsercion);
botonEliminar.addEventListener('click', manejarEliminacion);
botonBuscar.addEventListener('click', manejarBusqueda);
botonDibujar.addEventListener('click', manejarDibujarArbol);
botonAnimar.addEventListener('click', manejarAnimarArbol);
menuToggle.addEventListener('click', alternarMenu);
shareButton.addEventListener('click', compartirArbol);

// Inicialización
actualizarVisualizacionArbol();
