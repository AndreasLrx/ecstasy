///
/// @file tests_BitSet.cpp
/// @author Yanis Guaye (yanis.guaye@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include <gtest/gtest.h>

#include "util/BitSet.hpp"

namespace util
{
    /// Used to access BitSet internals in testing.
    class BitSetTester {
      public:
        static std::size_t getStoreSize(BitSet const &set)
        {
            return set._store.size();
        }
    };
} // namespace util

TEST(BitSet, init)
{
    util::BitSet empty;
    util::BitSet word(64);
    util::BitSet beeg(420);
    util::BitSet fromStr("1000101");

    EXPECT_EQ(empty.size(), 0);
    EXPECT_EQ(word.size(), 64);
    EXPECT_EQ(beeg.size(), 420);
    EXPECT_EQ(fromStr.size(), 7);

    EXPECT_EQ(util::BitSetTester::getStoreSize(empty), 0);
    EXPECT_EQ(util::BitSetTester::getStoreSize(word), 1);
    EXPECT_EQ(util::BitSetTester::getStoreSize(beeg), 7);
    EXPECT_EQ(util::BitSetTester::getStoreSize(fromStr), 1);

    for (std::size_t i = 0; i < 64; ++i)
        EXPECT_FALSE(word[i]) << "index was " << i;
    for (std::size_t i = 0; i < 420; ++i)
        EXPECT_FALSE(beeg[i]) << "index was " << i;
    EXPECT_TRUE(fromStr[0]);
    EXPECT_FALSE(fromStr[1]);
    EXPECT_TRUE(fromStr[2]);
    EXPECT_FALSE(fromStr[3]);
    EXPECT_FALSE(fromStr[4]);
    EXPECT_FALSE(fromStr[5]);
    EXPECT_TRUE(fromStr[6]);
}

TEST(BitSet, set)
{
    util::BitSet set("0111001000");

    set.set(0);
    EXPECT_TRUE(set.test(0));
    set.set(0, false);
    EXPECT_FALSE(set.test(0));

    set.setAll();
    EXPECT_EQ(set, util::BitSet("1111111111"));
    set.setAll(false);
    EXPECT_EQ(set, util::BitSet("0000000000"));
}

TEST(BitSet, Reference)
{
    util::BitSet set(21);

    EXPECT_FALSE(set[2]);
    set[2] = true;
    EXPECT_TRUE(set[2]);
    set[2] = false;
    EXPECT_FALSE(set[2]);

    set[1].flip();
    EXPECT_TRUE(set[1]);
    set[1].flip();
    EXPECT_FALSE(set[1]);

    util::BitSet::Reference ref = set[3];
    EXPECT_FALSE(ref);
    EXPECT_TRUE(~ref);
}

TEST(BitSet, flippinBits)
{
    util::BitSet set("110100100");
    util::BitSet flipped = ~set;

    EXPECT_EQ(set, util::BitSet("110100100"));
    EXPECT_EQ(flipped, util::BitSet("001011011"));
    set.flip();
    EXPECT_EQ(set, util::BitSet("001011011"));
}

TEST(BitSet, operations)
{
    EXPECT_EQ(util::BitSet("111") & util::BitSet("11011100"), util::BitSet("100"));
    EXPECT_EQ(util::BitSet("111") | util::BitSet("11011100"), util::BitSet("111"));
    EXPECT_EQ(util::BitSet("111") ^ util::BitSet("11011100"), util::BitSet("011"));

    util::BitSet set1("11011100");
    util::BitSet set2("11011100");
    util::BitSet set3("11011100");

    set1 &= util::BitSet("111");
    set2 |= util::BitSet("111");
    set3 ^= util::BitSet("111");

    EXPECT_EQ(set1, util::BitSet("100"));
    EXPECT_EQ(set2, util::BitSet("111"));
    EXPECT_EQ(set3, util::BitSet("011"));
}

TEST(BitSet, resize)
{
    util::BitSet set("101110");

    set.resize(420);
    set[419] = true;

    EXPECT_EQ(util::BitSetTester::getStoreSize(set), 7);

    EXPECT_FALSE(set[0]);
    EXPECT_TRUE(set[1]);
    EXPECT_TRUE(set[2]);
    EXPECT_TRUE(set[3]);
    EXPECT_FALSE(set[4]);
    EXPECT_TRUE(set[5]);

    EXPECT_FALSE(set[413]);
    EXPECT_FALSE(set[414]);
    EXPECT_FALSE(set[415]);
    EXPECT_FALSE(set[416]);
    EXPECT_FALSE(set[417]);
    EXPECT_TRUE(set[419]);
}

TEST(BitSet, resizeSentinel)
{
    util::BitSet set;

    set.resizeSentinel(0, true);
    EXPECT_EQ(set.size(), 1);
    EXPECT_TRUE(set[0]);

    set.resizeSentinel(4, true);
    EXPECT_EQ(set.size(), 5);

    EXPECT_FALSE(set[0]);
    EXPECT_FALSE(set[1]);
    EXPECT_FALSE(set[2]);
    EXPECT_FALSE(set[3]);
    EXPECT_TRUE(set[4]);
}

TEST(BitSet, pushPop)
{
    util::BitSet set;
    util::BitSet wordSized(64);

    set.push(1);
    set.push(0);
    set.push(1);
    set.push(1);
    set.push(0);

    EXPECT_EQ(set, util::BitSet("01101"));

    EXPECT_FALSE(set.pop());
    EXPECT_TRUE(set.pop());
    EXPECT_TRUE(set.pop());

    EXPECT_EQ(set, util::BitSet("01"));

    EXPECT_EQ(util::BitSetTester::getStoreSize(wordSized), 1);
    wordSized.push(1);
    EXPECT_EQ(util::BitSetTester::getStoreSize(wordSized), 2);
}

TEST(BitSet, firstSet)
{
    EXPECT_EQ(util::BitSet("1").firstSet(), 0);
    EXPECT_EQ(util::BitSet("100").firstSet(), 2);
    EXPECT_EQ(util::BitSet("101").firstSet(1), 2);
    EXPECT_EQ(util::BitSet("111").firstSet(1), 1);

    util::BitSet big(1678);

    big[681] = true;
    EXPECT_EQ(big.firstSet(), 681);
    EXPECT_EQ(big.firstSet(600), 681);
    EXPECT_EQ(big.firstSet(681), 681);
}

TEST(BitSet, firstUnset)
{
    EXPECT_EQ(util::BitSet("0").firstUnset(), 0);
    EXPECT_EQ(util::BitSet("011").firstUnset(), 2);
    EXPECT_EQ(util::BitSet("010").firstUnset(1), 2);
    EXPECT_EQ(util::BitSet("000").firstUnset(1), 1);

    util::BitSet big(1678);

    big.setAll();
    big[681] = false;
    EXPECT_EQ(big.firstUnset(), 681);
    EXPECT_EQ(big.firstUnset(600), 681);
    EXPECT_EQ(big.firstUnset(681), 681);
}
