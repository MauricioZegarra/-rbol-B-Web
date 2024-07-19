const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const app = express();
const port = 3000;

// Serve static files from the "public" directory
app.use(express.static('public'));

// Set the path to the AVL executable
const executable = path.join(__dirname, 'app.exe');

app.post('/insert/:value', (req, res) => {
    const value = req.params.value;
    // Colocar la ruta del ejecutable entre comillas
    exec(`"${executable}" insert ${value}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${stderr}`);
            res.status(500).send(stderr);
        } else {
            console.log(`Output: ${stdout}`);
            res.send(stdout); // Envía el preorden después de insertar
        }
    });
});
app.post('/remove/:value', (req, res) => {
    const value = req.params.value;
    // Colocar la ruta del ejecutable entre comillas
    exec(`"${executable}" remove ${value}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${stderr}`);
            res.status(500).send(stderr);
        } else {
            console.log(`Output: ${stdout}`);
            res.send(stdout); // Envía el preorden después de borrar
        }
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
