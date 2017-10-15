#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <thread>

#include <Box2D/Box2D.h>

#include "libs/json.hpp"
#include "settings.hpp"
#include "map.hpp"
#include "tp_map_importer.hpp"
#include "map_renderer.hpp"
#include "websocket_server.hpp"

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
    map_renderer renderer(m);

    if(renderer.open_window() != 0) {
        return EXIT_FAILURE;
    }

    b2World * world = m.init_world();
    while(renderer.render() && renderer.get_input()) {
        world->Step(1/60.0, 8, 3);
    }

    renderer.close_window();
    return EXIT_SUCCESS;
}

int serve(const std::string & map_src)
{
    std::thread srv_thread(start_server, 5000);
    srv_thread.detach();

    return render(map_src);
}

int main(int argc, char ** argv)
{
    settings::get_instance(); // to load settings up front

    if(argc < 2) {
        std::cerr << "usage: ./tagos [export|render|serve] [PARAMS]" << std::endl;
        std::cerr << "./tagos export tp_maps/Head.json tp_maps/Head.png maps/head.json" << std::endl;
        std::cerr << "./tagos render maps/head.json" << std::endl;
        std::cerr << "./tagos serve maps/head.json" << std::endl;
        return EXIT_FAILURE;
    }

    std::string mode(argv[1]);
    if(mode == "export") {
        if(argc != 5) {
            std::cerr << "error: export needs 3 args" << std::endl;
            std::cerr << "export IN_JSON IN_PNG OUT_JSON" << std::endl;
            return EXIT_FAILURE;
        }

        const std::string json_src(argv[2]);
        const std::string png_src(argv[3]);
        const std::string out_src(argv[4]);

        return export_tp_map(json_src, png_src, out_src);
    } else if(mode == "render") {
        if(argc != 3) {
            std::cerr << "error: render needs 1 arg" << std::endl;
            std::cerr << "render MAP" << std::endl;
            return EXIT_FAILURE;
        }
        
        const std::string map_src(argv[2]);

        return render(map_src);
    } else if(mode == "serve") {
        if(argc != 3) {
            std::cerr << "error: serve needs 1 arg" << std::endl;
            std::cerr << "render serve" << std::endl;
            return EXIT_FAILURE;
        }

        const std::string map_src(argv[2]);

        return serve(map_src);
    } else {
        std::cerr << "error: invalid mode: " << mode << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
