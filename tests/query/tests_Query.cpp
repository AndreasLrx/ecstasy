#include <gtest/gtest.h>
#include "ecstasy/query/Query.hpp"
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

struct Position {
    int x;
    int y;

    Position(int px, int py) : x(px), y(py)
    {
    }
    Position(std::pair<int, int> pair) : x(pair.first), y(pair.second)
    {
    }
};

using Size = Position;

TEST(Query, where)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Size> sizes;
    ecstasy::Entities entities;
    for (int i = 0; i < 10; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            sizes.emplace(i, i * 10, i * 2);
    }

    ecstasy::Query query(entities);
    GTEST_ASSERT_EQ(query.getMask(), entities.getMask());
    query.where(positions);
    GTEST_ASSERT_NE(query.getMask(), entities.getMask());
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("0101010101"));
    query.where(sizes);
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("0101000001"));

    auto it = query.fetch(positions, sizes);
    /// 0
    // auto [pos, size] = *it;
    // GTEST_ASSERT_EQ(pos.y, 0);
    // GTEST_ASSERT_EQ(size.y, 0);

    // /// 6
    // ++it;
    // auto [pos6, size6] = *it;
    // GTEST_ASSERT_EQ(pos6.y, 60);
    // GTEST_ASSERT_EQ(size6.y, 12);

    // /// 8
    // ++it;
    // auto [pos8, size8] = *it;
    // GTEST_ASSERT_EQ(pos8.y, 80);
    // GTEST_ASSERT_EQ(size8.y, 16);
}