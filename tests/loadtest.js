var WebSocket = require('ws');

const LobbyPort = 5000;
const Games = 600;
const PlayersPerGame = 8;
const GameDelay = 1000;
const MoveTimeout = 500;

function add_new_connection(port) {
    var ws = new WebSocket('ws://127.0.0.1:' + port);

    ws.on('open', () => {
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

        ws.send(JSON.stringify(wsdata));
        setTimeout(set_random_dir_every_second, Math.random() * MoveTimeout);
    };

    ws.on('message', (msg) => {
        let data = JSON.parse(msg);

        if(data.event == "gamesync") {
            set_random_dir_every_second();
            total_players++;
            console.log("total_players", total_players);
        }
    });


    ws.on('error', (msg) => {
        console.log('%s', msg);
    });
}

var total_players = 0;
for(var i=LobbyPort; i<=LobbyPort+Games; ++i) {
    setTimeout(function(i) {
        for(var j=0; j<PlayersPerGame; ++j) {
            add_new_connection(i);
        }
    }, GameDelay*(i-LobbyPort), i);
}
