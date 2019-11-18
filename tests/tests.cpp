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
#include "sequences/sequences/Merge.h"
#include "sequences/sequences/Chain.h"
#include "sequences/sequences/Successors.h"
#include "sequences/sequences/Take.h"
#include "sequences/sequences/Stride.h"
#include "sequences/sequences/Merge.h"
#include "sequences/sequences/Fuse.h"
#include "sequences/sequences/Enumerate.h"
#include "sequences/sequences/Fold.h"
#include "sequences/sequences/Deref.h"
#include "sequences/sequences/Inspect.h"
#include "sequences/sequences/Cofold.h"

#include "common.h"

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

static std::optional<std::pair<int, int>>
cofold_descending(int n)
{
    if (n == 0)
    {
        return {};
    }
    else
    {
        return std::pair{n, n - 1};
    }
}

TEST_CASE("NextGen")
{
    Cofold a{&cofold_descending, 10};

    for (int n : Exhaust{a}) {
        printf("%d\n", n);
    }

//
//
//    std::vector<int> numbers{3, 1, 4, 2};
//    std::vector<int const *> pointers{
//        &numbers[1],
//        &numbers[3],
//        &numbers[0],
//        &numbers[2]
//    };
//
//    Elements a{pointers};
//    Deref aa{a};
//    Inspect aaa{aa, [](int const &n) {
//        printf("Inspector: %d\n", n);
//    }};
//
//    for (int n : Exhaust{aaa}) {
//        printf("%d\n", n);
//    }

//    Successors a{1};
//    Take aa{a, 4};
//    Fold aaa{aa, [](int c, int a) {
////        std::stringstream ss;
////        ss << n << ", " << a;
////        return ss.str();
//        return c * a;
//    }};
//
////    std::string sum = aaa.reduce("");
//    int sum = *aaa.reduce_maybe();
//
//    printf("%d\n", sum);

//    Successors a{0};
//    Stride aa{a, 2};
//    Take aaa{aa, 5};
//    Enumerate aaaa{aaa};
//
//    for(auto const &[i, n] : Exhaust{aaaa})
//    {
//        printf("%zu: %d\n", i, n);
//    }


//    Successors<int> a{0};
//    Successors<int> b{1};
//    Successors<int> c{2};
//
//    Stride aa{a, 2};
//    Map bb{b, &display};
//    Map cc{c, [](int n) { return 1 << n; }};
//
//    Take aaa{aa, 5};
//
//    Merge m{std::tuple{aaa, bb, cc}};
//    Take mm{m, 10};
//
//    for(auto [a, b, c] : Exhaust{mm})
//    {
//        printf("%d, %s, %d\n", a, b.data(), c);
//    }

//    std::vector<int> xs{1, 2, 3, 4};
//    std::vector<int> ys{10, 20, 30, 40};
//    Elements x{xs};
//    Elements y{ys};
//    std::vector<Elements<int>> as{x, y};
//    Elements a{as};
//    Flatten b{a};
//    Filter c{b, &is_even};
//    Map d{c, &inc};

//    Successors<float> a{0u};
//    Take b{a, 10};
//    Stride c{b, 3};
//
//    auto &seq = c;
//
//    for (int n : Exhaust{seq})
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
