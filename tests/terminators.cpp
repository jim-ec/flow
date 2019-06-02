#include "catch2/catch.hpp"

#include <list>
#include <vector>
#include <functional>
#include <map>

#include "sequences/Sequence.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Collecting")
{
    SECTION("Base case")
    {
        auto v = make_mutation_linear()
                .take(4)
                .collect<std::vector<int>>();

        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 0);
        REQUIRE(v[1] == 1);
        REQUIRE(v[2] == 2);
        REQUIRE(v[3] == 3);
    }

    SECTION("To hash map")
    {
        auto v = make_mutation_linear()
                .take(4)
                .map([] (int n) { return std::make_pair('A' + char(n), n + 1); })
                .collect<std::map<char, int>>();

        REQUIRE(v.size() == 4);
        REQUIRE(v['A'] == 1);
        REQUIRE(v['B'] == 2);
        REQUIRE(v['C'] == 3);
        REQUIRE(v['D'] == 4);
    }
}

TEST_CASE("Counting")
{
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};
    auto sequence = make_sequence(v);
    REQUIRE(sequence.count() == v.size());
}

TEST_CASE("Partitioning")
{
    auto s = make_mutation_linear().take(6); // { 0 1 2 3 4 5 }

    SECTION("Base case")
    {
        auto[v1, v2] = s.partition<std::vector<int>>([](int n) { return n % 2 == 0; });

        REQUIRE(v1.size() == 3);

        REQUIRE(v1[0] == 0);
        REQUIRE(v1[1] == 2);
        REQUIRE(v1[2] == 4);

        REQUIRE(v2.size() == 3);

        REQUIRE(v2[0] == 1);
        REQUIRE(v2[1] == 3);
        REQUIRE(v2[2] == 5);
    }

    SECTION("Into different collection types")
    {
        auto[v1, v2] = s.partition<std::vector<int>, std::list<int>>([](int n) { return n % 2 == 0; });

        REQUIRE(v1.size() == 3);

        REQUIRE(v1[0] == 0);
        REQUIRE(v1[1] == 2);
        REQUIRE(v1[2] == 4);

        REQUIRE(v2.size() == 3);

        auto iter = v2.begin();
        REQUIRE(*iter == 1);
        REQUIRE(*(++iter) == 3);
        REQUIRE(*(++iter) == 5);
    }
}

TEST_CASE("Inner product")
{
    int data1[] = {3, 4, 5};
    int data2[] = {1, 2, 3};

    // 3 + 4 * 2 + 5 * 3 = 26

    SECTION("Explicit functions")
    {
        int ip = make_sequence(data1).inner_product(
                make_sequence(data2),
                [](int a, int b) { return a * b; },
                [](int a, int b) { return a + b; }
        );
        REQUIRE(ip == 26);
    }

    SECTION("Defaulted functions")
    {
        int ip = make_sequence(data1).inner_product(make_sequence(data2));
        REQUIRE(ip == 26);
    }
}
