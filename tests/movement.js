var WebSocket = require('ws');
var ws = new WebSocket('ws://127.0.0.1:5001');

ws.on('open', () => {
    console.log('open');
    ws.send(JSON.stringify({
        "request": "gamesync",
        "login_token": "nothing"
    }));
});

const set_random_dir_every_second = () => {
    const rdir = () => Math.random() < 0.25 ? -1 : Math.random() < 0.75 ? 0 : 1
    const wsdata = {
        "request": "movement",
        "xdir": rdir(),
        "ydir": rdir()
    };

    console.log('sending: ', wsdata);
    ws.send(JSON.stringify(wsdata));

    setTimeout(set_random_dir_every_second, 1000);
};

ws.on('message', (msg) => {
    let data = JSON.parse(msg);

    if(data.event == "gamesync") {
        set_random_dir_every_second();
    }
});


ws.on('error', (msg) => {
    console.log('%s', msg);
});
