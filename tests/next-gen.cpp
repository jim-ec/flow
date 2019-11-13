#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>

#include "sequences/Iterator.h"

#include "tools.h"

using namespace sequences;

static int
inc(int n)
{
    return n + 1;
}

static bool
is_even(int n)
{
    return n % 2 == 0;
}

TEST_CASE("NextGen")
{
    std::vector<int> xs{1, 2, 3, 4};
    std::vector<int> ys{10, 20, 30, 40};
    Elements x{xs};
    Elements y{ys};
    std::vector<Elements<int>> as{x, y};
    Elements a{as};
    Flatten b{a};
    Filter c{b, &is_even};
    Map d{c, &inc};

    auto &seq = d;

    for(int n : Over{seq})
    {
        printf("%d\n", n);
    }

//    SECTION("Base case")
//    {
//        Successors a;
//        Filter b{a, &is_even};
//        Map c{b, &inc};
//        Take d{c, 3};
//
//        auto &seq = d;
//
//        for (auto x = seq.next(); x; x = seq.next())
//        {
//            auto n = *x;
//            printf("%d\n", n);
//        }
//    }
}
