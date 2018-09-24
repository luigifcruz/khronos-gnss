const express = require('express');
const path = require('path');
const http = require('http');

const app = new express();

app.get('/bundle.js', (req, res) => {
    req.url = req.url + '.gz';
    res.set('Content-Encoding', 'gzip');
    res.set('Content-Type', 'text/javascript');
    res.sendFile(path.join(__dirname, "dist/bundle.js.gz"));
})

app.get('/main.css', (req, res) => { 
    req.url = req.url + '.gz';
    res.set('Content-Encoding', 'gzip');
    res.set('Content-Type', 'text/css');  
    res.sendFile(path.join(__dirname, "dist/main.css.gz"));
})

app.get('/*', (req, res) => {   
    res.sendFile(path.join(__dirname, "dist/index.html"));
})

const server = http.createServer(app).listen(3002);
console.log(`Simulator started at port 3002...`);
