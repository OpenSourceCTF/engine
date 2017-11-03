var WebSocket = require('ws');
var ws = new WebSocket('ws://127.0.0.1:5000');

ws.on('open', () => {
    console.log('request: games');
    ws.send(JSON.stringify({"request": "games"}));
});

ws.on('message', (msg) => {
    console.log('received: %s', msg);
});


ws.on('error', (msg) => {
    console.log('%s', msg);
});
