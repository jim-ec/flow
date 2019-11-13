#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>

#include "sequences/Iterator.h"
#include "sequences/sequences/Elements.h"
#include "sequences/sequences/MoveElements.h"
#include "sequences/sequences/Flatten.h"
#include "sequences/sequences/Filter.h"
#include "sequences/sequences/Map.h"
#include "sequences/sequences/Chain.h"
#include "sequences/sequences/Successors.h"
#include "sequences/sequences/Take.h"

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

static std::string
display(int n)
{
    std::stringstream ss;
    ss << n;
    return ss.str();
}

static S
f(S const &s)
{
    return S{s.id * 2};
}

TEST_CASE("NextGen")
{
    std::vector<S> xs;
    xs.emplace_back(5);
    xs.emplace_back(6);

    MoveElements a{std::move(xs)};
    Map b{a, &f};

    auto &seq = b;

    printf("New:\n");
    for(const S& n : Over{seq})
    {
        printf("|  \"%d\": %s\n", n.id, n.display_flags().data());
    }

//    printf("Old:\n");
//    for(const S& n : xs)
//    {
//        printf("|  \"%d\"\n", n.id);
//    }

//    std::vector<int> xs{1, 2, 3, 4};
//
//    Elements a{xs};
//    Map b{a, &display};
//
//    auto &seq = b;
//
//    for(std::string n : Over{seq})
//    {
//        printf("%s\n", n.data());
//    }
//
//    std::vector<int> xs{1, 2, 3, 4};
//    std::vector<int> ys{10, 20, 30, 40};
//    Elements x{xs};
//    Elements y{ys};
//    std::vector<Elements<int>> as{x, y};
//    Elements a{as};
//    Flatten b{a};
//    Filter c{b, &is_even};
//    Map d{c, &inc};
//
//    Successors<float> a2{0u};
//    Take b2{a2, 10};
//
//    Chain a3{d, b2};
//
//    auto &seq = a3;
//
//    for(int n : Over{seq})
//    {
//        printf("%d\n", n);
//    }

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
