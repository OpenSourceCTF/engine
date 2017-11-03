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
        spdlog::get("game")->critical("exception in color constructor stoi (", e.what(), ")");
    } catch(...) {
        spdlog::get("game")->critical("unknown exception in color constructor stoi");
    }
}

void to_json(nlohmann::json& j, const color& p)
{
    j = nlohmann::json{{"r", p.r}, {"g", p.g}, {"b", p.b}, {"a", p.a}};
}

void from_json(const nlohmann::json& j, color& p)
{
    p = color(
        j.at("r").get<int>(),
        j.at("g").get<int>(),
        j.at("b").get<int>(),
        j.at("a").get<int>()
    );
}
