#include "catch2/catch.hpp"

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
