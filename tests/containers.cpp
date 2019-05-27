#include "catch2/catch.hpp"

#include <cstring>
#include <vector>
#include <string>
#include <sstream>

#include "sequences/Sequence.h"
#include "sequences/Mutation.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Copy to other sequence")
{
    std::vector<int> v;
    auto sequence = make_mutation_linear().range(3);
    sequence.emplace_to(v);
    sequence.emplace_to(v);

    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 0);
    REQUIRE(v[4] == 1);
    REQUIRE(v[5] == 2);
}
