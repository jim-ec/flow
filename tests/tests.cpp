#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>
#include <array>

#include "flow/Elements.h"
#include "flow/ElementsReferenced.h"
#include "flow/Flatten.h"
#include "flow/Filter.h"
#include "flow/Map.h"
#include "flow/Zip.h"
#include "flow/Chain.h"
#include "flow/Successors.h"
#include "flow/Take.h"
#include "flow/Stride.h"
#include "flow/Fuse.h"
#include "flow/Fold.h"
#include "flow/Inspect.h"
#include "flow/Unfold.h"
#include "flow/Flow.h"
#include "flow/Cycle.h"

#include "TestsAuxiliary.h"

TEST_CASE("Take")
{
    auto a = flow::successors(1) | flow::take(5);
    
    REQUIRE(a.yield().value() == 1);
    REQUIRE(a.yield().value() == 2);
    REQUIRE(a.yield().value() == 3);
    REQUIRE(a.yield().value() == 4);
    REQUIRE(a.yield().value() == 5);
    REQUIRE(!a.yield().has_value());
}

TEST_CASE("Owning subsequences")
{
    auto uppercase = [] (char c) { return static_cast<char>(std::toupper(c)); };
    
    auto a = flow::successors(1) | flow::take(2) | flow::map([=] (int n)
    {
        auto string = n == 1 ? std::string("hello") : std::string("ciao");
        return flow::elements(string) | flow::map(uppercase);
    });
    
    auto a1 = a.yield().value();
    REQUIRE(a1.yield().value() == 'H');
    REQUIRE(a1.yield().value() == 'E');
    REQUIRE(a1.yield().value() == 'L');
    REQUIRE(a1.yield().value() == 'L');
    REQUIRE(a1.yield().value() == 'O');
    REQUIRE(!a1.yield().has_value());
    
    auto a2 = a.yield().value();
    REQUIRE(a2.yield().value() == 'C');
    REQUIRE(a2.yield().value() == 'I');
    REQUIRE(a2.yield().value() == 'A');
    REQUIRE(a2.yield().value() == 'O');
    REQUIRE(!a2.yield().has_value());
    
    REQUIRE(!a.yield().has_value());
}

TEST_CASE("Cycle")
{
    auto xs = {1, 2, 3};
    
    auto a = flow::elements(xs) | flow::cycle();
    
    REQUIRE(a.yield().value() == 1);
    REQUIRE(a.yield().value() == 2);
    REQUIRE(a.yield().value() == 3);
    REQUIRE(a.yield().value() == 1);
    REQUIRE(a.yield().value() == 2);
    REQUIRE(a.yield().value() == 3);
    REQUIRE(a.yield().value() == 1);
    REQUIRE(a.yield().value() == 2);
    REQUIRE(a.yield().value() == 3);
    REQUIRE(a.yield().has_value());
}

TEST_CASE("Cycle empty")
{
    std::initializer_list<int> xs = {};
    
    auto a = flow::elements(xs) | flow::cycle();
    
    REQUIRE(!a.yield().has_value());
}

TEST_CASE("Composition")
{
    auto xs = {1, 2, 3, 4, 5, 6};
    
    auto a = flow::elements(xs)
        | flow::map([] (int i) { return i + 2; })
        | flow::filter([] (int i) { return i % 2 == 0; });
    
    auto aCopy = a;
    
    REQUIRE(aCopy.yield().value() == 4);
    REQUIRE(aCopy.yield().value() == 6);
    REQUIRE(aCopy.yield().value() == 8);
    REQUIRE(!aCopy.yield().has_value());
    
    int sum = 0;
    for (int x: a) {
        sum += x;
    }
    
    REQUIRE(sum == 4 + 6 + 8);
}

TEST_CASE("Then")
{
    auto xs = {std::optional(3), std::optional<int>(), std::optional(7)};
    
    auto c = flow::elements(xs) | flow::then([] (int i) { return i + 1; });
    
    REQUIRE(c.yield().value() == std::optional(4));
    REQUIRE(!c.yield().value().has_value());
    REQUIRE(c.yield().value() == std::optional(8));
    REQUIRE(!c.yield().has_value());
}

TEST_CASE("Zip")
{
    auto as = {1, 2, 3};
    auto bs = {'a', 'b', 'c'};
    auto c = flow::elements(as) | flow::zip(flow::elements(bs));
    
    REQUIRE(c.yield().value() == std::tuple(1, 'a'));
    REQUIRE(c.yield().value() == std::tuple(2, 'b'));
    REQUIRE(c.yield().value() == std::tuple(3, 'c'));
    REQUIRE(!c.yield().has_value());
}

TEST_CASE("Dereference")
{
    std::array<int, 4> as{3, 1, 4, 2};
    std::array<int const *, 4> bs{&as[1], &as[3], &as[0], &as[2]};

    auto b = flow::elements(bs)
             | flow::dereference();
    
    REQUIRE(b.yield().value() == 1);
    REQUIRE(b.yield().value() == 2);
    REQUIRE(b.yield().value() == 3);
    REQUIRE(b.yield().value() == 4);
    REQUIRE(!b.yield().has_value());
}

TEST_CASE("Chain")
{
    std::array<int, 2> as{1, 2};
    std::array<int, 2> bs{3, 4};

    auto flow = flow::elements(as)
                | flow::chain(flow::elements(bs));

    REQUIRE(flow.yield().value() == 1);
    REQUIRE(flow.yield().value() == 2);
    REQUIRE(flow.yield().value() == 3);
    REQUIRE(flow.yield().value() == 4);
    REQUIRE(!flow.yield().has_value());
}

TEST_CASE("Fold")
{
    auto flow = flow::Flow(flow::Successors(1)) | flow::take(4);
    auto sum = flow::fold(flow, 0, [] (int a, int b) { return a + b; });
    REQUIRE(sum == 10);
}

TEST_CASE("Fold maybe")
{
    auto flow = flow::Flow(flow::Successors(1)) | flow::take(4);
    auto sum = flow::fold(flow, [] (int a, int b) { return a + b; });
    REQUIRE(sum.value() == 10);
}

TEST_CASE("Fold maybe empty")
{
    auto flow = flow::Flow(flow::Successors(1)) | flow::take(0);
    auto sum = flow::fold(flow, [] (int a, int b) { return a + b; });
    REQUIRE(!sum.has_value());
}

TEST_CASE("Unfold")
{
    auto unfoldDescending = [] (int n) -> std::optional<std::pair<int, int>> {
        if (n == 0)
        {
            return {};
        }
        else
        {
            return std::pair(n, n - 1);
        }
    };

    auto flow = flow::unfold(4, unfoldDescending);

    REQUIRE(flow.yield().value() == 4);
    REQUIRE(flow.yield().value() == 3);
    REQUIRE(flow.yield().value() == 2);
    REQUIRE(flow.yield().value() == 1);
    REQUIRE(!flow.yield().has_value());
}

TEST_CASE("Referenced elements")
{
    std::array xs = {1, 2, 3};
    
    auto seq = flow::elementsReferenced(xs);
    
    REQUIRE(seq.yield().value() == &xs[0]);
    REQUIRE(seq.yield().value() == &xs[1]);
    REQUIRE(seq.yield().value() == &xs[2]);
    REQUIRE(!seq.yield().has_value());
}

TEST_CASE("Referenced elements mutate")
{
    std::array xs = {1, 2, 3};
    
    auto seq = flow::elementsReferenced(xs);
    
    *seq.yield().value() = 8;
    
    REQUIRE(xs[0] == 8);
}

TEST_CASE("Elements")
{
    auto xs = {1, 2, 3};

    auto seq = flow::elements(xs);
    REQUIRE(seq.yield().value() == 1);
    REQUIRE(seq.yield().value() == 2);
    REQUIRE(seq.yield().value() == 3);
    REQUIRE(!seq.yield().has_value());
}

TEST_CASE("Filter")
{
    std::array<int, 4> as{1, 2, 3, 4};

    auto flow = flow::elements(as)
                | flow::filter([] (int n) { return n % 2 == 0; });

    REQUIRE(flow.yield().value() == 2);
    REQUIRE(flow.yield().value() == 4);
    REQUIRE(!flow.yield().has_value());
}

TEST_CASE("Strings")
{
    std::vector<std::string> strings{"hello,", "ciao,", "foo"};
    auto flow = flow::elements(strings)
                | flow::map([] (std::string const &s) { return flow::elements(s); })
                | flow::flatten();

    REQUIRE(flow.yield().value() == 'h');
    REQUIRE(flow.yield().value() == 'e');
    REQUIRE(flow.yield().value() == 'l');
    REQUIRE(flow.yield().value() == 'l');
    REQUIRE(flow.yield().value() == 'o');
    REQUIRE(flow.yield().value() == ',');
    REQUIRE(flow.yield().value() == 'c');
    REQUIRE(flow.yield().value() == 'i');
    REQUIRE(flow.yield().value() == 'a');
    REQUIRE(flow.yield().value() == 'o');
    REQUIRE(flow.yield().value() == ',');
    REQUIRE(flow.yield().value() == 'f');
    REQUIRE(flow.yield().value() == 'o');
    REQUIRE(flow.yield().value() == 'o');
    REQUIRE(!flow.yield().has_value());
}

TEST_CASE("Flatten")
{
    Container<Container<Identifier>> cc;
    cc.a().a().id = 1;
    cc.a().b().id = 2;
    cc.b().a().id = 3;
    cc.b().b().id = 4;

    int invocations = 0;

    // The container is moved here, so we do not need to expect any copies, only moves.
    auto flow = flow::elements(std::move(cc))
                | flow::map([&] (Container<Identifier> const &c) { ++invocations; return flow::elements(c); })
                | flow::flatten();

    // While building the sequence, the functors must not be called due to lazy evaluation.
    REQUIRE(invocations == 0);

    REQUIRE(flow.yield().value().id == 1);
    REQUIRE(flow.yield().value().id == 2);
    REQUIRE(flow.yield().value().id == 3);
    REQUIRE(flow.yield().value().id == 4);
    REQUIRE(!flow.yield().has_value());

    // There are two inner maps.
    REQUIRE(invocations == 2);
}
