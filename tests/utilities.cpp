#include "catch2/catch.hpp"

#include <vector>

#include "sequences/Sequence.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Iteration")
{
    std::vector<int> v{1, 2, 3, 4, 5};
    auto sequence = make_sequence(v);

    REQUIRE(sequence.next() == 1);
    REQUIRE(sequence.next() == 2);
    REQUIRE(sequence.next() == 3);
    REQUIRE(sequence.next() == 4);
    REQUIRE(sequence.next() == 5);
    REQUIRE(sequence.empty());
}

TEST_CASE("Empty sequences")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = make_sequence(v).take(0);

    REQUIRE(sequence.count() == 0);
    REQUIRE(sequence.empty());
}

TEST_CASE("Containment")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = make_sequence(v);

    REQUIRE(sequence.contains(4));
    REQUIRE(!sequence.contains(0));
}

TEST_CASE("Last")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = make_sequence(v);

    REQUIRE(sequence.last_element() == 6);
}

TEST_CASE("Skipping over end")
{
    auto sequence = make_mutation(1, [](int n) {
        return 2 * n;
    }).take(1).skip(10);
    REQUIRE(sequence.empty());
}

TEST_CASE("Get value")
{
    SECTION("Not empty")
    {
        int data[] = {3};
        auto s = make_sequence(data);
        const auto& cs = s;

        REQUIRE(s.value() == 3);
        REQUIRE(cs.value() == 3);
    }

    SECTION("Empty")
    {
        std::vector<int> data;
        auto s = make_sequence(data);
        const auto& cs = s;

        int fallback = 6;
        REQUIRE(s.value_or(fallback) == 6);
        
        REQUIRE(cs.value_or(6) == 6);
    }
}
