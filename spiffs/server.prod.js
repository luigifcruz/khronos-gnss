import express from 'express'
import path from 'path'
import http from 'http'

const app = new express();
const server = http.createServer(app).listen(3002);

app.get('/bundle.js', (req, res) => {   
    res.sendFile(path.join(__dirname, "dist/bundle.js"));
})

app.get('/style.css', (req, res) => {   
    res.sendFile(path.join(__dirname, "dist/style.css"));
})

app.get('/*', (req, res) => {   
    res.sendFile(path.join(__dirname, "dist/index.html"));
})

console.log(`Simulator started at port 3002...`);
