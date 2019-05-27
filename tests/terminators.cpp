#include "catch2/catch.hpp"

#include <cstring>
#include <vector>
#include <string>
#include <sstream>

#include "sequences/Sequence.h"
#include "sequences/Mutation.h"

#include "tools.h"

using namespace sequences;

TEST_CASE("Counting")
{
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};
    auto sequence = make_sequence(v);
    REQUIRE(sequence.count() == v.size());
}
