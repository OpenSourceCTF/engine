#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>

#include <Box2D/Box2D.h>

#include "libs/json.hpp"
#include "map.hpp"
#include "tp_map_importer.hpp"
#include "map_renderer.hpp"
#include "settings.hpp"
#include "server_lobby.hpp"
#include "game.hpp"

int display_renderer(map& m)
{
    map_renderer renderer(m);

    if(renderer.open_window() != 0) {
        std::cerr << "error: open window failed" << std::endl;
        return 1;
    }

    while(renderer.render() && renderer.get_input());
    renderer.close_window();

    return 0;
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

int render(const std::string & map_src)
{
    std::ifstream t(map_src);
    std::stringstream buf;
    buf << t.rdbuf();


    map m = nlohmann::json::parse(buf.str());
    game g(0, &m);
    g.spawn_thread();

    if(display_renderer(*(g.m)) != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int serve()
{
    server_lobby& lobby = server_lobby::get_instance();
    lobby.start_server();

    while(lobby.is_alive) {
        std::cout << std::endl << "$ " << std::flush;
        std::string line;
        std::getline(std::cin, line);

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


        if(cmd == "quit") {
            lobby.is_alive = false;
            std::cout << "quitting..." << std::endl;
        } else if(cmd == "help") {
            std::cout
                << "available commands: \n\n"
                << "\thelp           (show this help)\n" 
                << "\tquit           (quits server)\n" 
                << "\trender GAME_ID (opens sfml debug window for game)\n"
                << "\tstats          (shows game/player stats)\n"
                << std::endl;
        } else if(cmd == "render") {
            if(iparts.size() != 2) {
                std::cout
                    << "render requires two arguments"
                    << std::endl;
                continue;
            }

            const server_lobby& lobby = server_lobby::get_instance();

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

            display_renderer(*(lobby.games[game_id].get()->m));
        } else if(cmd == "stats") {
            const server_lobby& lobby = server_lobby::get_instance();

            std::size_t total_players = 0;
            for(std::size_t i=0; i<lobby.games.size(); ++i) {
                const game& g = *(lobby.games[i]);
                const std::size_t g_players = g.m->balls.size();
                std::cout
                    << "game: " << i << "\t"
                    << "players: " << g_players
                    << "\n";
                total_players += g_players;
            }

            std::cout
                << "\n"
                << "total games:\t" << lobby.games.size() << "\n"
                << "total players:\t" << total_players
                << std::endl;
        } else {
            std::cout
                << "unrecognized command (try help)"
                << std::endl;
        }
    }

    return 0;
}

int main(int argc, char ** argv)
{
    settings::get_instance(); // to load settings up front

    if(argc < 2) {
        std::cerr << "usage: ./tagos [export|render|serve] [PARAMS]" << std::endl;
        std::cerr << "./tagos export tp_maps/Head.json tp_maps/Head.png maps/head.json" << std::endl;
        std::cerr << "./tagos render maps/head.json" << std::endl;
        std::cerr << "./tagos serve" << std::endl;
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
    } else {
        std::cerr << "error: invalid mode: " << mode << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
