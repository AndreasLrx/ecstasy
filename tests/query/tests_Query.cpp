#include <gtest/gtest.h>
#include "ecstasy/query/Query.hpp"
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

#include "ecstasy/query/ComplexQuery.hpp"

struct Vector2i {
    int x;
    int y;

    Vector2i(int px, int py) : x(px), y(py)
    {
    }

    bool operator==(const Vector2i &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Position {
    int x;
    int y;

    Position(int px, int py) : x(px), y(py)
    {
    }
    Position(std::pair<int, int> pair) : x(pair.first), y(pair.second)
    {
    }

    bool operator==(const Vector2i &other) const
    {
        return x == other.x && y == other.y;
    }
};

using Size = Position;

struct Velocity {
    Vector2i v;

    Velocity(int x, int y) : v(x, y)
    {
    }
};

struct MovableMarker {
};

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

    auto query3 = ecstasy::Query(entities, positions, sizes);
    GTEST_ASSERT_EQ(query3.getMask(), util::BitSet("1101000001"));

    auto it = query3.begin();
    /// 0
    auto [e, pos, size] = *it;
    GTEST_ASSERT_EQ(pos.y, 0);
    GTEST_ASSERT_EQ(size.y, 0);

    /// 6
    ++it;
    auto [e2, pos6, size6] = *it;
    GTEST_ASSERT_EQ(pos6.y, 60);
    GTEST_ASSERT_EQ(size6.y, 12);

    /// 8
    ++it;
    auto [e3, pos8, size8] = *it;
    GTEST_ASSERT_EQ(pos8.y, 80);
    GTEST_ASSERT_EQ(size8.y, 16);
}

TEST(ComplexQuery, omitStorages)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<MovableMarker> movables;
    ecstasy::Entities entities;

    for (int i = 0; i < 25; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            movables.emplace(i);
    }

    /// Simple queries
    /// without movable marker (0, 6, 8, 12, 18, 24 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(
        ecstasy::Query(entities, positions, velocities).getMask(), util::BitSet("11000001000001000101000001"));
    /// with movable marker (0, 8, 12, 24 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(ecstasy::Query(entities, positions, velocities, movables).getMask(),
        util::BitSet("11000000000001000100000001"));

    /// Complex Query: select only position and velocity data but query for positions, entities, velocities and movables
    auto query =
        ecstasy::Select<decltype(positions), decltype(velocities)>::where(positions, entities, velocities, movables);

    auto it = query.begin();
    /// 0
    {
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(0, 0), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(0, 0));
    }

    /// 8
    ++it;
    {
        size_t index = 8;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 12
    ++it;
    {
        size_t index = 12;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 24
    ++it;
    {
        size_t index = 24;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }
}
