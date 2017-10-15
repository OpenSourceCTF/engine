#include "util.hpp"

std::vector<std::string> split_on(const std::string & str, const char n)
{
    std::vector<std::string> pieces;
    std::stringstream ss(str);
    while(ss.good()) {
        std::string sub;
        std::getline(ss, sub, n);
        pieces.emplace_back(sub);
    }

    return pieces;
}

std::vector<polygon> make_square_poly(
    const std::uint32_t x,
    const std::uint32_t y
) {
    return {
        polygon(x, y, x+1, y, x+1, y+1),
        polygon(x, y, x, y+1, x+1, y+1)
    };
};

