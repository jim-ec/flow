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
    std::vector<int> x{1, 2, 3, 4};
    std::vector<int> y{10, 20, 30, 40};
    Elements xs{x};
    Elements ys{y};
    std::vector<Sequence<int> *> a{&xs, &ys};
    Elements as{a};
    Flatten bs{as};

    auto &seq = bs;

    for (auto x = seq.next(); x; x = seq.next())
    {
        auto n = *x;
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
