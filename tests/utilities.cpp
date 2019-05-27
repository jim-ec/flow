#include "catch2/catch.hpp"

#include <cstring>
#include <vector>
#include <string>
#include <sstream>

#include "sequences/Sequence.h"
#include "sequences/Mutation.h"

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
    auto sequence = make_sequence(v).range(0);

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
    }).range(1).skip(10);
    REQUIRE(sequence.empty());
}
