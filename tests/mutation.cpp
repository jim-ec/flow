#include "catch2/catch.hpp"

#include <cstring>
#include <vector>
#include <string>

#include "tools.h"
#include "sequences/Sequence.h"
#include "sequences/Mutation.h"

using namespace sequences;

TEST_CASE("Mutation sequence")
{
    int counter = 0;

    auto sequence = make_mutation(2, [&counter](int n) {
        ++counter;
        return n * n;
    });

    REQUIRE(sequence.next() == 2);
    REQUIRE(sequence.next() == 4);
    REQUIRE(*sequence == 16);
    REQUIRE(*sequence.skipped(2) == 1u << 16u);
    REQUIRE(counter == 4);
}

TEST_CASE("Finite mutation")
{
    auto sequence = make_mutation(1, [](int n) {
        return 2 * n;
    }).range(4);
    REQUIRE(sequence.next() == 1);
    REQUIRE(sequence.next() == 2);
    REQUIRE(sequence.next() == 4);
    REQUIRE(sequence.next() == 8);
    REQUIRE(sequence.empty());
}
