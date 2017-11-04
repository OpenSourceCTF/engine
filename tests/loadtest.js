var WebSocket = require('ws');

function add_new_connection(port) {
    var ws = new WebSocket('ws://127.0.0.1:' + port);

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

        setTimeout(set_random_dir_every_second, Math.random() * 2000);
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
}

for(var i=5001; i<=5008; ++i) {
    for(var j=0; j<8; ++j) {
        add_new_connection(i);
    }
}
