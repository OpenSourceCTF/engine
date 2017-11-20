#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

#include <Box2D/Box2D.h>
#include <spdlog/spdlog.h>
#include <json/json.hpp>
#include <linenoise.hpp>

#include "settings.hpp"
#include "map.hpp"
#include "tp_map_importer.hpp"
#include "pms_map_importer.hpp"
#include "map_renderer.hpp"
#include "lobby_server.hpp"
#include "game.hpp"
#include "websocket_game_client.hpp"

std::thread renderer_thread;
bool close_renderer_window=false;
bool renderer_window_open=false;

int display_renderer(map& m)
{
    renderer_window_open = true;
    map_renderer renderer(m);

    int rcode = 0;

    if(renderer.open_window() != 0) {
        std::cerr << "error: open window failed" << std::endl;
        rcode = 1;
        goto exit;
    }

    while(! close_renderer_window && renderer.render() && renderer.get_input());
    renderer.close_window();

exit:
    renderer_window_open = false;
    close_renderer_window = false;
    return rcode;
}

int export_tp_map(
    const std::string & json_src,
    const std::string & png_src,
    const std::string & out_src
) {
    std::cout << "exporting " << json_src << "... " << std::flush;

    map m;
    tp_map_importer importer(m);

    if(importer.tp_import(json_src, png_src) != 0) {
        return EXIT_FAILURE;
    }

    std::ofstream out_f(out_src);
    out_f << nlohmann::json(m).dump(4);
    out_f.close();
    std::cout << "done!" << std::endl;

    return EXIT_SUCCESS;
}

int export_pms_map(
    const std::string & pms_src,
    const double scale_size,
    const std::string & out_src
) {
    std::cout << "exporting " << pms_src << "... " << std::flush;

    map m;
    pms_map_importer importer(m);

    if(importer.import(pms_src, scale_size) != 0) {
        return EXIT_FAILURE;
    }

    std::ofstream out_f(out_src);
    out_f << nlohmann::json(m).dump(4);
    out_f.close();
    std::cout << "done!" << std::endl;

    return EXIT_SUCCESS;
}

int render(const std::string & map_src)
{
    game g(0);

    if(! g.load_map(map_src)) {
        return EXIT_FAILURE;
    }

    g.spawn_phys_thread();

    ball* b = g.add_ball(new ball(ball_type::red));
    player* p = g.add_player(new player(&g, b, "player_id", true, "name", 100));
    b->set_player_ptr(p);
    b->add_to_world(g.world);
    g.respawn_ball(b);

    if(display_renderer(*(g.m)) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int serve()
{
    linenoise::SetCompletionCallback([](
        const char* ebuf,
        std::vector<std::string>& completions
    ) {
        const std::vector<std::string> cmds = {"exit", "quit", "help", "render", "stats", "log", "map"};
        const std::string edit(ebuf);

        for(auto && o : cmds) {
            if(edit[0] == o[0]) {
                completions.push_back(o);
            }
        }

        // todo: add completion for multiple characters
        // and handling for render N 
        // and handling for loading maps
    });

    lobby_server& lobby = lobby_server::get_instance();
    lobby.start_server();

    while(lobby.is_alive) {
        std::string line;

        bool quit = linenoise::Readline("$ ", line);

        if(quit) {
            if(renderer_window_open) {
                close_renderer_window = true;
                continue;
            } else {
                break;
            }
        }

        std::vector<std::string> iparts;
        {
            std::istringstream iss(line);
            for(std::string s; iss >> s;) {
                iparts.push_back(s);
            }
        }

        if(iparts.empty()) {
            continue;
        }

        const std::string cmd = iparts[0];


        if(cmd == "exit" || cmd == "quit") {
            lobby.is_alive = false;
            std::cout << "quitting..." << std::endl;
        } else if(cmd == "help") {
            std::cout
                << "available commands: \n\n"
                << "\thelp           (show this help)\n"
                << "\texit           (quits server)\n"
                << "\tquit           (quits server)\n"
                << "\trender GAME_ID (opens sfml debug window for game)\n"
                << "\tstats          (shows game/player stats)\n"
                << "\tlog LEVEL      (trace, debug, info, crit, error)\n"
                << "\tmap GAME_ID MAP(change map)\n"
                // todo add broadcast command so we can do something like broadcast 7 {"arbitrary_json":"test"} which will send data to all players
                // likewise for sending to specific player or specific team
                << std::endl;
        } else if(cmd == "render") {
            if(iparts.size() != 2) {
                std::cout
                    << "render requires two arguments"
                    << std::endl;
                continue;
            }

            const lobby_server& lobby = lobby_server::get_instance();

            if(lobby.games.size() == 0) {
                std::cerr << "error: no games currently running" << std::endl;
            }

            int game_id = 0;
            try {
                game_id = std::stoi(iparts[1]);
            } catch(std::invalid_argument& e) {
                std::cout
                    << "render requires an integer argument"
                    << std::endl;
                continue;
            }

            if(game_id < 0
            || static_cast<std::size_t>(game_id) >= lobby.games.size())
            {
                std::cout
                    << "game_id invalid"
                    << std::endl;
                continue;
            }

            if(! renderer_window_open) {
                renderer_thread = std::thread(
                    &display_renderer, std::ref(*(lobby.games[game_id].get()->m))
                );
                renderer_thread.detach();
            } else {
                std::cout << "close existing render window first" << std::endl;
            }
        } else if(cmd == "stats") {
            const lobby_server& lobby = lobby_server::get_instance();

            std::size_t total_players = 0;
            for(std::size_t i=0; i<lobby.games.size(); ++i) {
                const game& g = *(lobby.games[i]);
                const std::size_t g_players = g.m->balls.size();
                std::cout
                    << "game: " << i << "\t"
                    << "players: " << g_players << "\t"
                    << "red_score: " << g.red_points << "\t"
                    << "blue_score: "<< g.blue_points << "\t"
                    << "timestep: " << g.timestep
                    << "\n";
                total_players += g_players;
            }

            std::cout
                << "\n"
                << "total games:\t" << lobby.games.size() << "\n"
                << "total players:\t" << total_players
                << std::endl;
        } else if(cmd == "log") {
            if(iparts.size() != 2) {
                std::cout
                    << "log requires LEVEL argument"
                    << std::endl;
                continue;
            }

            const std::string level = iparts[1];
                 if(level == "trace") spdlog::set_level(spdlog::level::trace);
            else if(level == "debug") spdlog::set_level(spdlog::level::debug);
            else if(level == "info")  spdlog::set_level(spdlog::level::info);
            else if(level == "crit")  spdlog::set_level(spdlog::level::critical);
            else if(level == "error") spdlog::set_level(spdlog::level::err);
            else {
                std::cout << "invalid log argument" << std::endl;
                continue;
            }

            std::cout << "log level set to: " << level << std::endl;
        } else if(cmd == "map") {
            if(iparts.size() != 3) {
                std::cout
                    << "map requires GAME_ID and MAP argument"
                    << std::endl;
                continue;
            }

            if(renderer_window_open) {
                std::cout << "close existing render window first" << std::endl;
                continue;
            }

            lobby_server& lobby = lobby_server::get_instance();

            int game_id = 0;
            try {
                game_id = std::stoi(iparts[1]);
            } catch(std::invalid_argument& e) {
                std::cout
                    << "render requires an integer argument"
                    << std::endl;
                continue;
            }

            if(game_id < 0
            || static_cast<std::size_t>(game_id) >= lobby.games.size())
            {
                std::cout
                    << "game_id invalid"
                    << std::endl;
                continue;
            }

            game* g = lobby.games[game_id].get();

            const std::string map_src = iparts[2];

            g->load_map(map_src);
        } else {
            std::cout
                << "unrecognized command (try help)"
                << std::endl;
        }

        linenoise::AddHistory(line.c_str());
    }

    return 0;
}

int play(const std::string & uri)
{
    // todo
    return EXIT_SUCCESS;
}


int main(int argc, char ** argv)
{
    settings::get_instance(); // to load settings up front

    if(argc < 2) {
        std::cerr << "usage: ./tagos [export|exportpms|render|serve|play] [PARAMS]" << std::endl;
        std::cerr << "./tagos export tp_maps/Head.json tp_maps/Head.png maps/head.json" << std::endl;
        std::cerr << "./tagos exportpms example.PMS 0.003 maps/example.json" << std::endl;
        std::cerr << "./tagos render maps/head.json" << std::endl;
        std::cerr << "./tagos serve" << std::endl;
        // std::cerr << "./tagos play 127.0.0.1 5001" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        spdlog::stdout_logger_mt("game"); // initialize game log
        spdlog::set_error_handler([](const std::string& msg) {
            std::cerr << "error: spdlog handler: " << msg << std::endl;
        });
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "error: log init failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::string mode(argv[1]);
    if(mode == "export") {
        if(argc != 5) {
            std::cerr
                << "error: export needs 3 args" << std::endl
                << "ex: export IN_JSON IN_PNG OUT_JSON" << std::endl;
            return EXIT_FAILURE;
        }

        const std::string json_src(argv[2]);
        const std::string png_src(argv[3]);
        const std::string out_src(argv[4]);

        return export_tp_map(json_src, png_src, out_src);
    } else if(mode == "exportpms") {
        if(argc != 5) {
            std::cerr
                << "error: exportpms needs 3 args" << std::endl
                << "ex: exportpms IN_PMS SCALE_SIZE OUT_JSON" << std::endl;
            return EXIT_FAILURE;
        }

        const std::string pms_src(argv[2]);
        const double      scale_size = std::stod(argv[3]);
        const std::string out_src(argv[4]);

        return export_pms_map(pms_src, scale_size, out_src);
    } else if(mode == "render") {
        if(argc != 3) {
            std::cerr
                << "error: render needs 1 arg" << std::endl
                << "ex: render MAP" << std::endl;
            return EXIT_FAILURE;
        }
        
        const std::string map_src(argv[2]);

        return render(map_src);
    } else if(mode == "serve") {
        if(argc != 2) {
            std::cerr
                << "error: serve does not take arguments" << std::endl
                << "ex: serve" << std::endl;
            return EXIT_FAILURE;
        }

        return serve();
    } else if(mode == "play") {
        if(argc != 3) {
            std::cerr
                << "error: play needs uri arg" << std::endl
                << "ex: play ws://127.0.0.1:5001" << std::endl;
            return EXIT_FAILURE;
        }

        const std::string uri(argv[2]);

        return play(uri);
    } else {
        std::cerr << "error: invalid mode: " << mode << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
