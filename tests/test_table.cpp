//Unit test for Transposition Table

#include "doctest.h"
#include "table.h"

using namespace C4;

TEST_CASE("Transposition table: .get returns correct values") {
    TranspositionTable t;
    const std::uint64_t key = 0x26122007ULL;   
    t.update(key, 42, 5);
    auto [v, col] = t.get(key);
    CHECK(v == 42);
    CHECK(col == 5);
}

TEST_CASE("Transposition table: .get returns 0 when a key doesn't exist") {
    TranspositionTable t;
    CHECK(t.get(0x26122007ULL).first == 0);
}

TEST_CASE("Transposition table: .get returns 0 when the requested key doesn't match the key stored") {
    TranspositionTable t;
    const std::uint64_t k1 = 2007;
    const std::uint64_t k2 = 2007 + 8388593;   // k1 + TABLE_SIZE
    t.update(k1, 7, 3);
    CHECK(t.get(k2).first == 0);
}

TEST_CASE("Transposition table: Reset clears the tables") {
    TranspositionTable t;
    t.update(99, 5, 1);
    t.reset();
    CHECK(t.get(99).first == 0);
}

TEST_CASE("Transposition table: A new key can override an old key at the same slot"){
    TranspositionTable t;
    const std::uint64_t k1 = 2007;
    const std::uint64_t k2 = 2007 + 8388495;

    t.update(k1, 2, 6);
    t.update(k2, 1, 2);
    auto [v, col] = t.get(k2);
    CHECK(v == 1);
    CHECK(col == 2);
}

TEST_CASE("Bucket transposition table: Buckets are implemented correctly"){
    BucketTranspositionTable t;
    const std::uint64_t k1 = (1LL << 29) + 2007;
    const std::uint64_t k2 = (1LL << 29) + 2007 + t.TABLE_SIZE;
    
    t.update(k1, 2, 6);
    t.update(k2, 1, 2);

    auto [v1, col1] = t.get(k1);
    CHECK(v1 == 2);
    CHECK(col1 == 6);

    auto [v2, col2] = t.get(k2);
    CHECK(v2 == 1);
    CHECK(col2 == 2);

    t.update(k1, 1, 5);

    auto [v3, col3] = t.get(k1);
    CHECK(v3 == 1);
    CHECK(col3 == 5);

}