#pragma once
#include <sstream>

template <typename T>
std::string to_string(const T& t)
{
    std::ostringstream ss;
    ss << t;
    return ss.str();
}
