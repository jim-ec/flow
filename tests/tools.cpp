//
// Created by jim on 27.05.19.
//
#include <sstream>

#include "tools.h"

std::string concat(const std::string &a, const std::string &b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

std::string concat(const std::string &a, int n)
{
    std::stringstream ss;
    ss << a << n;
    return ss.str();
}
