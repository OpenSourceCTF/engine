let WebSocket = require('ws');

let player_one_connected = false;
let voted_already = false;

let ws1 = new WebSocket('ws://127.0.0.1:5001');

ws1.on('open', () => {
    console.log('open');
    ws1.send(JSON.stringify({
        "request": "gamesync",
        "login_token": "nothing"
    }));
});

ws1.on('message', (msg) => {
    let data = JSON.parse(msg);

    if(data.event == "gamesync") {
        player_one_connected = true;
    }

    if(data.event == "vote_player") {
        console.log("vote_player(1)", data);
    }
});


ws1.on('error', (msg) => {
    console.log('%s', msg);
});

let ws2 = new WebSocket('ws://127.0.0.1:5001');

ws2.on('open', () => {
    console.log('open');
    ws2.send(JSON.stringify({
        "request": "gamesync",
        "login_token": "nothing"
    }));
});

ws2.on('message', (msg) => {
    let data = JSON.parse(msg);

    if(data.event == "gamesync") {
        const check_ready_to_vote = () => {
            if(player_one_connected && ! voted_already) {
                ws2.send(JSON.stringify({
                    "request": "vote_player",
                    "player_id": "player_id",
                    "reason": "too good"
                }));
                console.log('voting');

                voted_already = true;
            } else {
                setTimeout(check_ready_to_vote, 500);
            }
        };

        check_ready_to_vote();
    }

    if(data.event == "vote_player") {
        console.log("vote_player(2)", data);
    }
});


ws2.on('error', (msg) => {
    console.log('%s', msg);
});
