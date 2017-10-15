#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>


#include "libs/json.hpp"
#include "settings.hpp"
#include "map.hpp"

int main(int argc, char ** argv)
{
    settings::get_instance(); // to load settings up front

    if(argc < 2) {
        std::cerr << "usage: ./map_loader [export|render] [PARAMS]" << std::endl;
        std::cerr << "./map_loader export tp_maps/Head.json tp_maps/Head.png maps/head.json" << std::endl;
        std::cerr << "./map_loader render maps/head.json" << std::endl;
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

        std::cout << "exporting " << json_src << "... " << std::flush;

        map m;

        if(m.tp_import(json_src, png_src) != 0) {
            return EXIT_FAILURE;
        }

        std::ofstream out_f(out_src);
        out_f << nlohmann::json(m).dump(4);
        out_f.close();
        std::cout << "done!" << std::endl;
    } else if(mode == "render") {
        if(argc != 3) {
            std::cerr << "error: render needs 1 arg" << std::endl;
            std::cerr << "render MAP" << std::endl;
            return EXIT_FAILURE;
        }

        std::ifstream t(argv[2]);
        std::stringstream buf;
        buf << t.rdbuf();

        map m = nlohmann::json::parse(buf.str());

        if(m.render() != 0) {
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "error: invalid mode: " << mode << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
