#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"

#include <cstring>
#include <vector>

#include "sequences/sequence.h"
#include "sequences/mutation.h"

using namespace sequences;

TEST_CASE("Counting")
{
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};
    auto sequence = makeSequence(v);
    REQUIRE(sequence.count() == v.size());
}

TEST_CASE("Sub-sequences")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = makeSequence(v);

    REQUIRE(sequence.count() == 6);
    REQUIRE(sequence.skip(2).count() == 4);
    REQUIRE(sequence.range(3).count() == 3);
    REQUIRE(sequence.skip(1).range(3).count() == 3);
    REQUIRE(sequence.range(3).skip(1).count() == 2);
}

TEST_CASE("Empty sequences")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = makeSequence(v).range(0);

    REQUIRE(sequence.count() == 0);
    REQUIRE(sequence.empty());
}

TEST_CASE("Containment")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = makeSequence(v);

    REQUIRE(sequence.contains(4));
    REQUIRE(!sequence.contains(0));
}

TEST_CASE("Last")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = makeSequence(v);

    REQUIRE(sequence.lastElement() == 6);
}

TEST_CASE("Iteration")
{
    std::vector<int> v{1, 2, 3, 4, 5};
    auto sequence = makeSequence(v);

    REQUIRE(*sequence.next() == 1);
    REQUIRE(*sequence.next() == 2);
    REQUIRE(*sequence.next() == 3);
    REQUIRE(*sequence.next() == 4);
    REQUIRE(*sequence.next() == 5);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Mapping")
{
    std::vector<int> v{1, 2, 3, 4, 5};
    auto sequence = makeSequence(v).map<int>([](int n) { return n * n; });

    REQUIRE(*sequence.next() == 1);
    REQUIRE(*sequence.next() == 4);
    REQUIRE(*sequence.next() == 9);
    REQUIRE(*sequence.next() == 16);
    REQUIRE(*sequence.next() == 25);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Filtering")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto sequence = makeSequence(v).filter([](int n) {
        return n % 2 == 0;
    });

    REQUIRE(*sequence.next() == 2);
    REQUIRE(*sequence.next() == 4);
    REQUIRE(*sequence.next() == 6);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Zipping")
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::vector<char> w{'a', 'b', 'c', 'd'};

    auto sequence = makeSequence(v).zip(makeSequence(w));

    REQUIRE(makePair(1, 'a') == *sequence.next());
    REQUIRE(makePair(2, 'b') == *sequence.next());
    REQUIRE(makePair(3, 'c') == *sequence.next());
    REQUIRE(makePair(4, 'd') == *sequence.next());
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Inspecting")
{
    std::vector<char> v{'a', 'b', 'c'};
    char buf[4] = {};
    int i = 0;
    makeSequence(v).inspect([&buf, &i](char &c) {
        buf[i++] = c;
        ++c;
    }).count();
    REQUIRE(v[0] == 'b');
    REQUIRE(v[1] == 'c');
    REQUIRE(v[2] == 'd');
    REQUIRE(i == 3);
    REQUIRE(strcmp("abc", buf) == 0);
}

TEST_CASE("For each")
{
    std::vector<char> v{'a', 'b', 'c'};
    char buf[4] = {};
    int i = 0;
    makeSequence(v).forEach([&buf, &i](char c) {
        buf[i++] = c;
    });
    REQUIRE(i == 3);
    REQUIRE(strcmp("abc", buf) == 0);
}

TEST_CASE("Mutation sequence")
{
    int counter = 0;

    auto sequence = makeMutation(2, [&counter](int n) {
        ++counter;
        return n * n;
    });

    REQUIRE(*sequence.next() == 2);
    REQUIRE(*sequence.next() == 4);
    REQUIRE(*sequence == 16);
    REQUIRE(*sequence.skip(2) == 1u << 16u);
    REQUIRE(counter == 4);
}

TEST_CASE("Finite mutation")
{
    auto sequence = makeMutation(1, [](int n) {
        return 2 * n;
    }).range(4);
    REQUIRE(*sequence.next() == 1);
    REQUIRE(*sequence.next() == 2);
    REQUIRE(*sequence.next() == 4);
    REQUIRE(*sequence.next() == 8);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Skipping over end")
{
    auto sequence = makeMutation(1, [](int n) {
        return 2 * n;
    }).range(1).skip(10);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Combine mapping and filtering")
{
    auto sequence = makeMutationLinear()
            .map<int>([](int n) {
                return n * n;
            })
            .filter([](int n) {
                return n % 2 == 0;
            })
            .map<int>([](int n) {
                return n + 1;
            });

    REQUIRE(*sequence.next() == 0 + 1);
    REQUIRE(*sequence.next() == 4 + 1);
    REQUIRE(*sequence.next() == 16 + 1);
    REQUIRE(*sequence.next() == 36 + 1);
    REQUIRE(*sequence.next() == 64 + 1);
    REQUIRE(*sequence.next() == 100 + 1);
    REQUIRE(*sequence.skip(2) == 256 + 1);
}

TEST_CASE("Pair mapping")
{
    auto evenNumberSequence = makeMutationLinear(0, 2);
    auto oddNumberSequence = makeMutationLinear(1, 2);

    auto numbers = evenNumberSequence
            .zip(oddNumberSequence)
            .map2<int>([](int even, int odd) {
                return odd - even;
            })
            .range(20);

    for (int n : numbers)
    {
        REQUIRE(1 == n);
    }
}

TEST_CASE("Sequence of write-through pointers")
{
    int a = 1;
    int b = 2;

    std::vector<int *> pointers{&a, &b};

    auto sequence = makeSequence(pointers).inspect([](int *n) {
        *n = 10;
    });

    REQUIRE(*sequence.next() == &a);
    REQUIRE(*sequence.next() == &b);
    REQUIRE(sequence.next() == sequence.end());
}

TEST_CASE("Copy to other sequence")
{
    std::vector<int> v;
    auto sequence = makeMutationLinear().range(3);
    sequence.emplaceTo(v);
    sequence.emplaceTo(v);

    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 0);
    REQUIRE(v[4] == 1);
    REQUIRE(v[5] == 2);
}

TEST_CASE("Index access")
{
    auto sequence = makeMutationLinear(1, 2);

    int a = sequence[0];
    int b = sequence[1];
    int c = sequence[2];

    REQUIRE(a == 1);
    REQUIRE(b == 3);
    REQUIRE(c == 5);
}

TEST_CASE("Chaining")
{
    auto s1 = makeMutationLinear().range(3);
    auto s2 = makeMutationLinear().range(3).map<int>([](int n) {
        return n + 3;
    });

    auto s3 = s1.chain(s2);

    REQUIRE(0 == *s3.next());
    REQUIRE(1 == *s3.next());
    REQUIRE(2 == *s3.next());
    REQUIRE(3 == *s3.next());
    REQUIRE(4 == *s3.next());
    REQUIRE(5 == *s3.next());
    REQUIRE(s3.end() == s3.next());
}

