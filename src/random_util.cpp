#include "random_util.hpp"

random_util& random_util::get_instance()
{
    static random_util instance;

    if(instance.is_initialized) {
        return instance;
    }

    instance.eng = std::mt19937(instance.rd());

    return instance;
}
