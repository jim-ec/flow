#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>
#include <array>

#include "flow/Maybe.h"
#include "flow/Elements.h"
#include "flow/ElementsReferenced.h"
#include "flow/Flatten.h"
#include "flow/Filter.h"
#include "flow/Map.h"
#include "flow/Zip.h"
#include "flow/Chain.h"
#include "flow/Take.h"
#include "flow/Stride.h"
#include "flow/Fuse.h"
#include "flow/Fold.h"
#include "flow/Inspect.h"
#include "flow/Generate.h"
#include "flow/Flow.h"
#include "flow/Cycle.h"

#include "TestsAuxiliary.h"

int add(int a, int b) { return a + b; }

TEST_CASE("Stream")
{
    std::vector<int> xs = {1, 2, 3, 4};
    
    auto a = flow::elements2(xs);
    auto b = a | flow::map2([] (int i) { return i * 2; });
    auto c = b | flow::map2([] (int i) { return i + 1; });
    auto d = c | flow::take2(2);

    REQUIRE(d.next() == 3);
    REQUIRE(d.next() == 5);
    REQUIRE(d.next() == flow::None());
}

TEST_CASE("Maybe: None")
{
    flow::Maybe<int> a = flow::None();
    
    REQUIRE(!a.hasValue());
}

TEST_CASE("Maybe: Owning")
{
    int x = 5;
    flow::Maybe<int> a = x;
    
    REQUIRE(a.hasValue());
    REQUIRE(a.value() == 5);
    REQUIRE(&a.value() != &x);
}

TEST_CASE("Maybe: Reference")
{
    int x = 5;
    flow::Maybe<int &> a = x;
    
    REQUIRE(a.hasValue());
    REQUIRE(a.value() == 5);
    REQUIRE(&a.value() == &x);
}

TEST_CASE("Maybe: Pointer")
{
    int x = 5;
    flow::Maybe<int *> a = &x;
    
    REQUIRE(a.hasValue());
    REQUIRE(a.value() == &x);
    REQUIRE(*a.value() == 5);
}

TEST_CASE("Maybe: Move out")
{
    Identifier id(5);
    flow::Maybe<Identifier> a = id;
    Identifier id2 = std::move(a).value();
    
    REQUIRE(id2.move_constructed);
    REQUIRE(a.value().moved_away);
}

TEST_CASE("Maybe: Assignment")
{
    int n = 5;
    flow::Maybe<int> a = n;
    flow::Maybe<int> b = flow::None();
    
    b = a;
    
    REQUIRE(b.value() == 5);
}

TEST_CASE("Take")
{
    auto a = flow::successors(1) | flow::take(5);

    REQUIRE(a.next().value() == 1);
    REQUIRE(a.next().value() == 2);
    REQUIRE(a.next().value() == 3);
    REQUIRE(a.next().value() == 4);
    REQUIRE(a.next().value() == 5);
    REQUIRE(!a.next().hasValue());
}

TEST_CASE("Owning subsequences")
{
    auto uppercase = [] (char c) { return static_cast<char>(std::toupper(c)); };

    auto a = flow::successors(1) | flow::take(2) | flow::map([=] (int n)
    {
        auto string = n == 1 ? std::string("hello") : std::string("ciao");
        return flow::elements(string) | flow::map(uppercase);
    });

    auto a1 = a.next().value();
    REQUIRE(a1.next().value() == 'H');
    REQUIRE(a1.next().value() == 'E');
    REQUIRE(a1.next().value() == 'L');
    REQUIRE(a1.next().value() == 'L');
    REQUIRE(a1.next().value() == 'O');
    REQUIRE(!a1.next().hasValue());

    auto a2 = a.next().value();
    REQUIRE(a2.next().value() == 'C');
    REQUIRE(a2.next().value() == 'I');
    REQUIRE(a2.next().value() == 'A');
    REQUIRE(a2.next().value() == 'O');
    REQUIRE(!a2.next().hasValue());

    REQUIRE(!a.next().hasValue());
}

TEST_CASE("Cycle")
{
    auto xs = {1, 2, 3};

    auto a = flow::elements(xs) | flow::cycle();

    REQUIRE(a.next().value() == 1);
    REQUIRE(a.next().value() == 2);
    REQUIRE(a.next().value() == 3);
    REQUIRE(a.next().value() == 1);
    REQUIRE(a.next().value() == 2);
    REQUIRE(a.next().value() == 3);
    REQUIRE(a.next().value() == 1);
    REQUIRE(a.next().value() == 2);
    REQUIRE(a.next().value() == 3);
    REQUIRE(a.next().hasValue());
}

TEST_CASE("Cycle empty")
{
    std::initializer_list<int> xs = {};

    auto a = flow::elements(xs) | flow::cycle();

    REQUIRE(!a.next().hasValue());
}

TEST_CASE("Composition")
{
    auto xs = {1, 2, 3, 4, 5, 6};

    auto a = flow::elements(xs)
        | flow::map([] (int i) { return i + 2; })
        | flow::filter([] (int i) { return i % 2 == 0; });

    auto aCopy = a;

    REQUIRE(aCopy.next().value() == 4);
    REQUIRE(aCopy.next().value() == 6);
    REQUIRE(aCopy.next().value() == 8);
    REQUIRE(!aCopy.next().hasValue());

    int sum = 0;
    for (int x: a) {
        sum += x;
    }

    REQUIRE(sum == 4 + 6 + 8);
}

TEST_CASE("Then")
{
    std::vector<flow::Maybe<int>> xs = {3, flow::None(), 7};

    auto c = flow::elements(xs) | flow::then([] (int i) { return i + 1; });

    REQUIRE(c.next().value() == flow::Maybe(4));
    REQUIRE(!c.next().value().hasValue());
    REQUIRE(c.next().value() == flow::Maybe(8));
    REQUIRE(!c.next().hasValue());
}

TEST_CASE("Zip")
{
    auto as = {1, 2, 3};
    auto bs = {'a', 'b', 'c'};
    auto c = flow::elements(as) | flow::zip(flow::elements(bs));

    REQUIRE(c.next().value() == std::pair(1, 'a'));
    REQUIRE(c.next().value() == std::pair(2, 'b'));
    REQUIRE(c.next().value() == std::pair(3, 'c'));
    REQUIRE(!c.next().hasValue());
}

TEST_CASE("Dereference")
{
    std::array<int, 4> as{3, 1, 4, 2};
    std::array<int const *, 4> bs{&as[1], &as[3], &as[0], &as[2]};

    auto b = flow::elements(bs)
             | flow::dereference();

    REQUIRE(b.next().value() == 1);
    REQUIRE(b.next().value() == 2);
    REQUIRE(b.next().value() == 3);
    REQUIRE(b.next().value() == 4);
    REQUIRE(!b.next().hasValue());
}

TEST_CASE("Chain")
{
    std::array<int, 2> as{1, 2};
    std::array<int, 2> bs{3, 4};

    auto flow = flow::elements(as)
                | flow::chain(flow::elements(bs));

    REQUIRE(flow.next().value() == 1);
    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 3);
    REQUIRE(flow.next().value() == 4);
    REQUIRE(!flow.next().hasValue());
}

TEST_CASE("Fold")
{
    auto flow = flow::Flow(flow::successors(1)) | flow::take(4);
    auto sum = flow::fold(flow, 0, [] (int a, int b) { return a + b; });
    REQUIRE(sum == 10);
}

TEST_CASE("Fold maybe")
{
    auto flow = flow::Flow(flow::successors(1)) | flow::take(4);
    auto sum = flow::fold(flow, [] (int a, int b) { return a + b; });
    REQUIRE(sum.value() == 10);
}

TEST_CASE("Fold maybe empty")
{
    auto flow = flow::Flow(flow::successors(1)) | flow::take(0);
    auto sum = flow::fold(flow, [] (int a, int b) { return a + b; });
    REQUIRE(!sum.hasValue());
}

TEST_CASE("Generate")
{
    int n = 4;
    auto generateDescending = [=] () mutable -> flow::Maybe<int> {
        if (n == 0)
        {
            return flow::None();
        }
        else
        {
            return n--;
        }
    };

    auto flow = flow::generate(generateDescending);

    REQUIRE(flow.next().value() == 4);
    REQUIRE(flow.next().value() == 3);
    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 1);
    REQUIRE(!flow.next().hasValue());
}

TEST_CASE("Referenced elements")
{
    std::array xs = {1, 2, 3};

    auto seq = flow::elementsReferenced(xs);

    REQUIRE(&seq.next().value() == &xs[0]);
    REQUIRE(&seq.next().value() == &xs[1]);
    REQUIRE(&seq.next().value() == &xs[2]);
    REQUIRE(!seq.next().hasValue());
}

TEST_CASE("Referenced elements mutate")
{
    std::array xs = {1, 2, 3};

    auto seq = flow::elementsReferenced(xs);

    seq.next().value() = 8;

    REQUIRE(xs[0] == 8);
}

TEST_CASE("Elements")
{
    auto xs = {1, 2, 3};

    auto seq = flow::elements(xs);
    REQUIRE(seq.next().value() == 1);
    REQUIRE(seq.next().value() == 2);
    REQUIRE(seq.next().value() == 3);
    REQUIRE(!seq.next().hasValue());
}

TEST_CASE("Filter")
{
    std::array<int, 4> as{1, 2, 3, 4};

    auto flow = flow::elements(as)
                | flow::filter([] (int n) { return n % 2 == 0; });

    REQUIRE(flow.next().value() == 2);
    REQUIRE(flow.next().value() == 4);
    REQUIRE(!flow.next().hasValue());
}

TEST_CASE("Strings")
{
    std::vector<std::string> strings{"hello,", "ciao,", "foo"};
    auto flow = flow::elements(strings)
                | flow::map([] (std::string const &s) { return flow::elements(s); })
                | flow::flatten();

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
    REQUIRE(!flow.next().hasValue());
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

    REQUIRE(flow.next().value().id == 1);
    REQUIRE(flow.next().value().id == 2);
    REQUIRE(flow.next().value().id == 3);
    REQUIRE(flow.next().value().id == 4);
    REQUIRE(!flow.next().hasValue());

    // There are two inner maps.
    REQUIRE(invocations == 2);
}
