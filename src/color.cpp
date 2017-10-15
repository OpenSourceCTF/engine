#include "color.hpp"


// from rgba hex like ffaabb or ffaabbff
color::color(const std::string & hex)
{
    try {
        r = hex.length() >= 2 ? std::stoi(hex.substr(0, 2), 0, 16) : 255;
        g = hex.length() >= 4 ? std::stoi(hex.substr(2, 2), 0, 16) : 255;
        b = hex.length() >= 6 ? std::stoi(hex.substr(4, 2), 0, 16) : 255;
        a = hex.length() >= 8 ? std::stoi(hex.substr(6, 2), 0, 16) : 255;
    } catch(const std::exception& e) {
        std::cerr << "error: exception in color constructor stoi (" << e.what() << ")" << std::endl;
    } catch(...) {
        std::cerr << "error: unknown exception in color constructor stoi" << std::endl;
    }
}

void to_json(nlohmann::json& j, const color& p)
{
    j = nlohmann::json{{"r", p.r}, {"g", p.g}, {"b", p.b}, {"a", p.a}};
}

void from_json(const nlohmann::json& j, color& p)
{
    p.r = j.at("r").get<int>();
    p.g = j.at("g").get<int>();
    p.b = j.at("b").get<int>();
    p.a = j.at("a").get<int>();
}
