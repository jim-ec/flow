#include "catch2/catch.hpp"

#include <list>
#include <vector>

#include "sequences/Sequence.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Counting")
{
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};
    auto sequence = make_sequence(v);
    REQUIRE(sequence.count() == v.size());
}

TEST_CASE("Partitioning")
{
    auto s = make_mutation_linear().range(6); // { 0 1 2 3 4 5 }

    SECTION("Into same type")
    {
        auto[v1, v2] = s.partition<std::vector>([](int n) { return n % 2 == 0; });

        REQUIRE(v1.size() == 3);

        REQUIRE(v1[0] == 0);
        REQUIRE(v1[1] == 2);
        REQUIRE(v1[2] == 4);

        REQUIRE(v2.size() == 3);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 3);
        REQUIRE(v2[2] == 5);
    }

    SECTION("Into different type")
    {
        struct Info1 {
            int a;
            explicit Info1(int a) : a{a} {}
        };

        struct Info2 {
            int b;
            explicit Info2(int b) : b{b} {}
        };

        auto[v1, v2] = s.partition<std::vector<Info1>, std::list<Info2>>([](int n) { return n % 2 == 0; });

        REQUIRE(v1.size() == 3);

        REQUIRE(v1[0].a == 0);
        REQUIRE(v1[1].a == 2);
        REQUIRE(v1[2].a == 4);

        REQUIRE(v2.size() == 3);

        auto iter = v2.begin();
        REQUIRE(iter->b == 1);
        REQUIRE((++iter)->b == 3);
        REQUIRE((++iter)->b == 5);
    }
}
