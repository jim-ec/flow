#include "catch2/catch.hpp"

#include <vector>

#include "sequences/Sequence.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Copy to other sequence")
{
    std::vector<int> v;
    auto sequence = make_mutation_linear().range(3);
    sequence.push_back_to(v);
    sequence.push_back_to(v);

    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 0);
    REQUIRE(v[4] == 1);
    REQUIRE(v[5] == 2);
}
