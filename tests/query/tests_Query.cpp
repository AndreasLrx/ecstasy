#include <gtest/gtest.h>
#include "ecstasy/query/Query.hpp"
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

#include "ecstasy/query/Select.hpp"
#include "ecstasy/query/modifiers/Maybe.hpp"
#include "ecstasy/query/modifiers/Not.hpp"
#include "ecstasy/query/modifiers/Or.hpp"

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

    auto query3 = ecstasy::query::Query(entities, positions, sizes);
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

TEST(Select, omitStorages)
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
        ecstasy::query::Query(entities, positions, velocities).getMask(), util::BitSet("11000001000001000101000001"));
    /// with movable marker (0, 8, 12, 24 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(ecstasy::query::Query(entities, positions, velocities, movables).getMask(),
        util::BitSet("11000000000001000100000001"));

    /// Select Query: select only position and velocity data but query for positions, entities, velocities and movables
    auto query = ecstasy::query::Select<decltype(positions), decltype(velocities)>::where(
        positions, entities, velocities, movables);

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

TEST(Query, NotModifier)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<MovableMarker> statics;
    ecstasy::Entities entities;
    auto notStatics = ecstasy::query::modifier::Not(statics);

    for (int i = 0; i < 25; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
    }

    /// -- No static in the storage --
    /// without looking at static marker (0, 6, 8, 12, 18, 24 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(
        ecstasy::query::Query(entities, positions, velocities).getMask(), util::BitSet("11000001000001000101000001"));
    /// without any statics in the storage (since it is a not it shoudln't change the query result because no entity has
    /// it)
    GTEST_ASSERT_EQ(ecstasy::query::Query(entities, positions, velocities, notStatics).getMask(),
        util::BitSet("11000001000001000101000001"));

    /// -- Statics in the storage --
    for (int i = 0; i < 25; i++) {
        if (i % 4 == 0)
            statics.emplace(i);
    }
    notStatics.reloadMask();
    /// It doesn't change anything since static isn't in the query
    GTEST_ASSERT_EQ(
        ecstasy::query::Query(entities, positions, velocities).getMask(), util::BitSet("11000001000001000101000001"));
    /// Returns the same result as before with the statics removed (6, 18 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(ecstasy::query::Query(entities, positions, velocities, notStatics).getMask(),
        util::BitSet("10000001000000000001000000"));

    /// Select query for all entities with position, velocities and without the statics component
    auto query = ecstasy::query::Select<decltype(positions), decltype(velocities)>::where(
        entities, positions, velocities, notStatics);
    auto it = query.begin();
    /// 6
    {
        size_t index = 6;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 18
    ++it;
    {
        size_t index = 18;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }
}

TEST(Query, Maybe)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;
    auto maybeVector = ecstasy::query::modifier::Maybe(vectors);

    for (int i = 0; i < 25; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    /// Without looking at the vectors return (0, 6, 8, 12, 18, 24 (and 25 is sentinel bit))
    GTEST_ASSERT_EQ(
        ecstasy::query::Query(entities, positions, velocities).getMask(), util::BitSet("11000001000001000101000001"));
    /// Keeping entities with vectors (0, 8, 12, 24 (and 25 is the sentinel bit))
    GTEST_ASSERT_EQ(ecstasy::query::Query(entities, positions, velocities, vectors).getMask(),
        util::BitSet("11000000000001000100000001"));
    /// Getting vectors if existing, must be the same result as the first one
    GTEST_ASSERT_EQ(ecstasy::query::Query(entities, positions, velocities, maybeVector).getMask(),
        util::BitSet("11000001000001000101000001"));

    /// Select query for all entities with position, velocities and without the statics component
    auto query = ecstasy::query::Select<decltype(positions), decltype(velocities), decltype(maybeVector)>::where(
        entities, positions, velocities, maybeVector);

    auto it = query.begin();
    /// 0
    {
        size_t index = 0;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 6
    ++it;
    {
        size_t index = 6;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }
    /// 8
    ++it;
    {
        size_t index = 8;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
    /// 12
    ++it;
    {
        size_t index = 12;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 18
    ++it;
    {
        size_t index = 18;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }

    /// 24
    ++it;
    {
        size_t index = 24;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
}

TEST(Query, Or)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;
    auto velocityOrVector = ecstasy::query::modifier::Or(velocities, vectors);

    for (int i = 0; i < 13; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    // clang-format off
    /// velocityOrVector mask has not been reloaded yet
    auto query = ecstasy::query::Select<decltype(positions)>::where(positions, velocityOrVector);
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("1"));

    velocityOrVector.reloadMask();
    auto maybeVelocity = ecstasy::query::modifier::Maybe(velocities);
    auto maybeVec = ecstasy::query::modifier::Maybe(vectors);
    auto query2 = ecstasy::query::Select<decltype(positions), 
        ecstasy::query::modifier::Maybe<decltype(velocities)>,
        ecstasy::query::modifier::Maybe<decltype(vectors)>>::
        where(positions, maybeVelocity, maybeVec, velocityOrVector);
    GTEST_ASSERT_EQ(ecstasy::query::Query(positions, velocities).getMask(), util::BitSet("11000101000001"));
    GTEST_ASSERT_EQ(ecstasy::query::Query(positions, vectors).getMask(),    util::BitSet("11000100010001"));
    GTEST_ASSERT_EQ(query2.getMask(),                                       util::BitSet("11000101010001")); /// Or of the two precedent masks
    // clang-format on

    auto it = query2.begin();
    /// 0 (vel and vec)
    {
        size_t index = 0;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 4 (!vel and vec)
    ++it;
    {
        size_t index = 4;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_FALSE(vel);
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 6 (vel and !vec)
    ++it;
    {
        size_t index = 6;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }

    /// 8 (vel and vec)
    ++it;
    {
        size_t index = 8;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 12 (vel and vec)
    ++it;
    {
        size_t index = 12;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
}