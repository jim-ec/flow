#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>

#include "sequences/Iterator.h"
#include "sequences/divergent/Elements.h"
#include "sequences/divergent/MoveElements.h"
#include "sequences/functional/Flatten.h"
#include "sequences/functional/Filter.h"
#include "sequences/functional/Map.h"
#include "sequences/convergent/Merge.h"
#include "sequences/convergent/Chain.h"
#include "sequences/divergent/Successors.h"
#include "sequences/functional/Take.h"
#include "sequences/functional/Stride.h"
#include "sequences/convergent/Merge.h"
#include "sequences/functional/Fuse.h"
#include "sequences/functional/Enumerate.h"
#include "sequences/convergent/Fold.h"
#include "sequences/functional/Deref.h"
#include "sequences/functional/Inspect.h"
#include "sequences/divergent/Cofold.h"
#include "sequences/Flow.h"

#include "common.h"

using namespace sequences;

static int
inc(int n) {
    return n + 1;
}

static bool
is_even(int n) {
    return n % 2 == 0;
}

static std::string
display(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

static S
f(S const &s) {
    return S(s.id * 2);
}

static std::optional<std::pair<int, int>>
cofold_descending(int n) {
    if (n == 0) {
        return {};
    }
    else {
        return std::pair(n, n - 1);
    }
}

TEST_CASE("Merge") {
    auto a = Flow(Elements{1, 2, 3});
    auto b = Flow(Elements{'a', 'b', 'c'});
    auto c = a | merge(b, a) | enumerate();

    REQUIRE(c.next() == std::optional(std::tuple(0, std::tuple(1, 'a', 1))));
    REQUIRE(c.next() == std::optional(std::tuple(1, std::tuple(2, 'b', 2))));
    REQUIRE(c.next() == std::optional(std::tuple(2, std::tuple(3, 'c', 3))));
    REQUIRE(!c.next().has_value());
}

TEST_CASE("Deref") {
    std::vector<int> numbers{3, 1, 4, 2};

    Flow a(Elements{numbers});
    Flow b = Flow(Elements{&numbers[1], &numbers[3], &numbers[0], &numbers[2]})
             | deref();

    REQUIRE(b.next() == std::optional(1));
    REQUIRE(b.next() == std::optional(2));
    REQUIRE(b.next() == std::optional(3));
    REQUIRE(b.next() == std::optional(4));
    REQUIRE(!b.next().has_value());
}

TEST_CASE("Chain") {
    auto flow = Flow(Elements{1, 2})
                | chain(Flow(Elements{3, 4}));

    REQUIRE(flow.next() == std::optional(1));
    REQUIRE(flow.next() == std::optional(2));
    REQUIRE(flow.next() == std::optional(3));
    REQUIRE(flow.next() == std::optional(4));
    REQUIRE(!flow.next().has_value());
}

TEST_CASE("Fold") {
    auto flow = Flow(Successors(1)) | take(4);
    auto sum = fold(flow, 0, [] (int a, int b) { return a + b; });
    REQUIRE(sum == 10);
}

TEST_CASE("NextGen") {
//    Cofold a{&cofold_descending, 10};
//
//    for (int n : ForEach{a}) {
//        printf("%d\n", n);
//    }

//
//


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
//    for(auto [a, b, c] : ForEach{mm})
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
