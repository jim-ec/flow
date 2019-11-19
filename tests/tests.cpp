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

static Identifier
f(Identifier const &s)
{
    return Identifier(s.id * 2);
}

TEST_CASE("Merge") {
    auto as = {1, 2, 3};
    auto bs = {'a', 'b', 'c'};
    auto a = elements(as);
    auto b = elements(bs);
    auto c = a | merge(b, a) | enumerate();

    REQUIRE(c.next().value() == std::tuple(0, std::tuple(1, 'a', 1)));
    REQUIRE(c.next().value() == std::tuple(1, std::tuple(2, 'b', 2)));
    REQUIRE(c.next().value() == std::tuple(2, std::tuple(3, 'c', 3)));
    REQUIRE(!c.next().has_value());
}

TEST_CASE("Deref")
{
    std::array<int, 4> as{3, 1, 4, 2};
    std::array<int const *, 4> bs{&as[1], &as[3], &as[0], &as[2]};

    auto b = elements(bs)
             | deref();

    REQUIRE(b.next().value() == 1);
    REQUIRE(b.next().value() == 2);
    REQUIRE(b.next().value() == 3);
    REQUIRE(b.next().value() == 4);
    REQUIRE(!b.next().has_value());
}

TEST_CASE("Chain")
{
    std::array<int, 2> as{1, 2};
    std::array<int, 2> bs{3, 4};

    auto flow = elements(as)
                | chain(elements(bs));

    REQUIRE(flow.next().value() == 1);
    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 3);
    REQUIRE(flow.next().value() == 4);
    REQUIRE(!flow.next().has_value());
}

TEST_CASE("Fold")
{
    auto flow = Flow(Successors(1)) | take(4);
    auto sum = fold(
        flow, 0, [](
            int a,
            int b
        ) { return a + b; }
    );
    REQUIRE(sum == 10);
}

TEST_CASE("Cofold")
{
    auto cofold_descending = [](int n) -> std::optional<std::pair<int, int>> {
        if (n == 0)
        {
            return {};
        }
        else
        {
            return std::pair(n, n - 1);
        }
    };

    auto flow = Cofold(4, cofold_descending);

    REQUIRE(flow.next().value() == 4);
    REQUIRE(flow.next().value() == 3);
    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 1);
    REQUIRE(!flow.next().has_value());
}

//TEST_CASE("Copy elements") {
//    std::vector<S> elements;
//    elements.emplace_back();
//
//    exhaust(Elements{elements});
//
//    REQUIRE(!elements[0].moved_away);
//}

//TEST_CASE("Move elements") {
//    std::vector<S> elements;
//    elements.emplace_back();
//
//    exhaust(MoveElements(std::move(elements)));
//
//    REQUIRE(elements[0].moved_away);
//}

TEST_CASE("Filter")
{
    std::array<int, 4> as{1, 2, 3, 4};

    auto flow = elements(as)
                | filter([](int n) { return n % 2 == 0; });

    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 4);
    REQUIRE(!flow.next().has_value());
}

TEST_CASE("Strings")
{
    std::vector<std::string> strings{"hello,", "ciao,", "foo"};
    // TODO: Here is a case of a severe lifetime bug.
    //  If I use `elements` rather then `ref_elements`,
    //  than each string gets copied before the mapping takes place.
    //  But the mapping creates another `elements` sequence, which requires the
    //  underlying container to outlive itself. Because the string is destroyed
    //  (when?) before the returned sequence, it is dangled.
    // TODO: Rebuild this test using `Identifier` and `Container`.
    auto flow = ref_elements(strings)
                | map([](std::string const *s) { return elements(*s); })
                | flatten();

    REQUIRE(flow.next().value() == 'h');
    REQUIRE(flow.next().value() == 'e');
    REQUIRE(flow.next().value() == 'l');
    REQUIRE(flow.next().value() == 'l');
    REQUIRE(flow.next().value() == 'o');
    REQUIRE(flow.next().value() == ',');
    REQUIRE(flow.next().value() == 'c');
    REQUIRE(flow.next().value() == 'i');
    REQUIRE(flow.next().value() == 'a');
    REQUIRE(flow.next().value() == 'o');
    REQUIRE(flow.next().value() == ',');
    REQUIRE(flow.next().value() == 'f');
    REQUIRE(flow.next().value() == 'o');
    REQUIRE(flow.next().value() == 'o');
    REQUIRE(!flow.next().has_value());
}

TEST_CASE("Flatten")
{
    Container<Container<Identifier>> cc;
    cc.a().a().id = 1;
    cc.a().b().id = 2;
    cc.b().a().id = 3;
    cc.b().b().id = 4;

    int invocations = 0;

    // TODO: Same lifetime problems as in "Strings".
    auto flow = ref_elements(cc) |
                map(
                    [&](Container<Identifier> const *c) {
                        ++invocations;
                        return elements(*c);
                    }
                ) |
                flatten();

    // While building the sequence, the functors must not be called
    // due to lazy evaluation.
    REQUIRE(invocations == 0);

    REQUIRE(flow.next().value() == cc.a().a());
    REQUIRE(flow.next().value() == cc.a().b());
    REQUIRE(flow.next().value() == cc.b().a());
    REQUIRE(flow.next().value() == cc.b().b());
    REQUIRE(!flow.next().has_value());

    // There are two inner maps.
    REQUIRE(invocations == 2);
}

TEST_CASE("NextGen")
{
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
