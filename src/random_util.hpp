#ifndef RANDOM_UTIL_HPP
#define RANDOM_UTIL_HPP

#include <random>

struct random_util
{
    bool is_initialized = false;

    std::random_device rd;
    std::mt19937 eng;

    // use this to initialize
    static random_util& get_instance();

    // prevent copies
    random_util(random_util const&) = delete;
    // prevent move which could invalidate state
    random_util(random_util&&) = delete;

private:
    // prevent client from creating new instances
    random_util() {}
};

#endif

