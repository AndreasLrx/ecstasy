#include <chrono>
#include <gtest/gtest.h>
#include "ecstasy/query/Query.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"

#include "ecstasy/query/Select.hpp"
#include "ecstasy/query/conditions/Condition.hpp"
#include "ecstasy/query/modifiers/And.hpp"
#include "ecstasy/query/modifiers/Maybe.hpp"
#include "ecstasy/query/modifiers/Not.hpp"
#include "ecstasy/query/modifiers/Or.hpp"
#include "ecstasy/query/modifiers/Xor.hpp"

template <typename L>
using LV = ecstasy::thread::LockableView<L>;

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

struct MovableMarker {};

/// Debugging helpers, this print types T1 and T2 in the compilation error message
template <typename T1, typename T2>
void assert_equals()
{
    static_assert(std::is_same_v<T1, T2>, "Types T1 and T2 differs.");
}

#ifdef ECSTASY_THREAD_SAFE
TEST(ThreadSafeQuery, all)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Size> sizes;
    ecstasy::Entities entities;
    {
        const auto &ce = entities;
        auto queryLocked = ecstasy::query::ThreadSafeQuery(ce, positions, sizes);
        assert_equals<decltype(queryLocked)::QueryData, std::tuple<ecstasy::Entity, Position &, Size &>>();
        assert_equals<decltype(queryLocked)::Queryables,
            std::tuple<LV<const ecstasy::Entities>, LV<ecstasy::MapStorage<Position>>,
                LV<ecstasy::MapStorage<Size>>>>();
    }

    {
        auto queryLocked_explicit = ecstasy::query::ThreadSafeQuery<const ecstasy::Entities,
            ecstasy::MapStorage<Position>, ecstasy::MapStorage<Size>>(entities, positions, sizes);
        assert_equals<decltype(queryLocked_explicit)::QueryData, std::tuple<ecstasy::Entity, Position &, Size &>>();
        assert_equals<decltype(queryLocked_explicit)::Queryables,
            std::tuple<LV<const ecstasy::Entities>, LV<ecstasy::MapStorage<Position>>,
                LV<ecstasy::MapStorage<Size>>>>();
    }

    {
        auto queryLocked_explicit = ecstasy::query::ThreadSafeQuery<ecstasy::MapStorage<Position>,
            const ecstasy::Entities, ecstasy::MapStorage<Size>>(positions, entities, sizes);
        assert_equals<decltype(queryLocked_explicit)::QueryData, std::tuple<Position &, ecstasy::Entity, Size &>>();
        assert_equals<decltype(queryLocked_explicit)::Queryables,
            std::tuple<LV<ecstasy::MapStorage<Position>>, LV<const ecstasy::Entities>,
                LV<ecstasy::MapStorage<Size>>>>();
    }

    {
        auto queryLocked = ecstasy::query::ThreadSafeQuery<ecstasy::Entities, ecstasy::MapStorage<Position>,
            ecstasy::MapStorage<Size>>(entities, positions, sizes);
        assert_equals<decltype(queryLocked)::QueryData, std::tuple<ecstasy::Entity, Position &, Size &>>();
        assert_equals<decltype(queryLocked)::Queryables,
            std::tuple<LV<ecstasy::Entities>, LV<ecstasy::MapStorage<Position>>, LV<ecstasy::MapStorage<Size>>>>();
    }

    {
        auto queryLocked = ecstasy::query::ThreadSafeQuery(entities, positions, sizes);
        assert_equals<decltype(queryLocked)::QueryData, std::tuple<ecstasy::Entity, Position &, Size &>>();
        assert_equals<decltype(queryLocked)::Queryables,
            std::tuple<LV<ecstasy::Entities>, LV<ecstasy::MapStorage<Position>>, LV<ecstasy::MapStorage<Size>>>>();
    }
}
#endif

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

TEST(Query, IteratorPosition)
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
    GTEST_ASSERT_EQ(it.getPosition(), 0);
    auto [e, pos, size] = *it;
    GTEST_ASSERT_EQ(pos.y, 0);
    GTEST_ASSERT_EQ(size.y, 0);

    /// 6
    ++it;
    GTEST_ASSERT_EQ(it.getPosition(), 6);
    auto [e2, pos6, size6] = *it;
    auto [e2_, pos6_, size6_] = query3.getQueryData(it.getPosition());
    GTEST_ASSERT_EQ(pos6.y, 60);
    GTEST_ASSERT_EQ(size6.y, 12);
    GTEST_ASSERT_EQ(pos6.y, pos6_.y);
    GTEST_ASSERT_EQ(size6.y, size6_.y);

    /// 8
    ++it;
    GTEST_ASSERT_EQ(it.getPosition(), 8);
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
        int index = 8;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 12
    ++it;
    {
        int index = 12;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 24
    ++it;
    {
        int index = 24;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }
}

TEST(Query, NotModifier)
{
    assert_equals<ecstasy::query::modifier::Not<ecstasy::MapStorage<MovableMarker>>::Internal,
        ecstasy::MapStorage<MovableMarker> &>();
    assert_equals<ecstasy::query::modifier::Not<const ecstasy::MapStorage<MovableMarker>>::Internal,
        const ecstasy::MapStorage<MovableMarker> &>();
#ifdef ECSTASY_LOCKABLE_STORAGES
    assert_equals<ecstasy::query::modifier::Not<ecstasy::MapStorage<MovableMarker>, true>::Internal,
        ecstasy::thread::LockableView<ecstasy::MapStorage<MovableMarker>>>();
    assert_equals<ecstasy::query::modifier::Not<const ecstasy::MapStorage<MovableMarker>, true>::Internal,
        ecstasy::thread::LockableView<const ecstasy::MapStorage<MovableMarker>>>();
#endif

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
        int index = 6;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }

    /// 18
    ++it;
    {
        int index = 18;
        auto [pos, velocity] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
    }
}

TEST(Query, Maybe)
{
    assert_equals<ecstasy::query::modifier::Maybe<ecstasy::MapStorage<MovableMarker>>::Internal,
        ecstasy::MapStorage<MovableMarker> &>();
    assert_equals<ecstasy::query::modifier::Maybe<const ecstasy::MapStorage<MovableMarker>>::Internal,
        const ecstasy::MapStorage<MovableMarker> &>();
#ifdef ECSTASY_LOCKABLE_STORAGES
    assert_equals<ecstasy::query::modifier::Maybe<ecstasy::MapStorage<MovableMarker>, true>::Internal,
        ecstasy::thread::LockableView<ecstasy::MapStorage<MovableMarker>>>();
    assert_equals<ecstasy::query::modifier::Maybe<const ecstasy::MapStorage<MovableMarker>, true>::Internal,
        ecstasy::thread::LockableView<const ecstasy::MapStorage<MovableMarker>>>();
#endif

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
        int index = 0;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 6
    ++it;
    {
        int index = 6;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }
    /// 8
    ++it;
    {
        int index = 8;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
    /// 12
    ++it;
    {
        int index = 12;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 18
    ++it;
    {
        int index = 18;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }

    /// 24
    ++it;
    {
        int index = 24;
        auto [pos, velocity, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
}

TEST(Query, Or)
{
    assert_equals<ecstasy::query::modifier::OrImpl<false, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::MapStorage<MovableMarker> &, ecstasy::MapStorage<Position> &>>();
    assert_equals<ecstasy::query::modifier::OrImpl<false, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<const ecstasy::MapStorage<MovableMarker> &, const ecstasy::MapStorage<Position> &>>();
#ifdef ECSTASY_LOCKABLE_STORAGES
    assert_equals<ecstasy::query::modifier::OrImpl<true, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<ecstasy::MapStorage<Position>>>>();
    assert_equals<ecstasy::query::modifier::OrImpl<true, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<const ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<const ecstasy::MapStorage<Position>>>>();
#endif

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
        int index = 0;
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
        int index = 4;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_FALSE(vel);
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }

    /// 6 (vel and !vec)
    ++it;
    {
        int index = 6;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }

    /// 8 (vel and vec)
    ++it;
    {
        int index = 8;
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
        int index = 12;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(Vector2i(index * 2, index * 10), pos);
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), Vector2i(index * 4, index * 6));
    }
}

TEST(Query, parameter_orders)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;

    for (int i = 0; i < 13; i++) {
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    {
        bool test = util::meta::type_set_eq_v<std::tuple<Position, Velocity>, std::tuple<Position, Velocity>>;
        GTEST_ASSERT_TRUE(test);
    }
    {
        bool test = util::meta::type_set_eq_v<std::tuple<Position, Velocity>, std::tuple<Velocity, Position>>;
        GTEST_ASSERT_TRUE(test);
    }

    {
        bool test = util::meta::type_set_eq_v<std::tuple<Position>, std::tuple<Velocity, Position>>;
        GTEST_ASSERT_FALSE(test);
    }
    {
        bool test = util::meta::type_set_eq_v<std::tuple<Position, Velocity>, std::tuple<Velocity>>;
        GTEST_ASSERT_FALSE(test);
    }

    /// If it compiles then it works
    auto query = ecstasy::query::Select<decltype(positions), decltype(velocities)>::where(velocities, positions);
    auto query2 = ecstasy::query::Select<decltype(positions), decltype(velocities), decltype(vectors)>::where(
        vectors, velocities, positions);
}

TEST(Query, OrVariadic)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;
    auto velocityOrVector = ecstasy::query::modifier::Or(positions, velocities, vectors);
    auto maybeVector = ecstasy::query::modifier::Maybe(vectors);

    for (int i = 0; i < 13; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    velocityOrVector.reloadMask();
    static_cast<void>(velocityOrVector.getQueryData(0));
    auto query = ecstasy::query::Select<decltype(maybeVector)>::where(maybeVector, velocityOrVector);
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11011101011101"));
}

TEST(Query, AndVariadic)
{
    assert_equals<ecstasy::query::modifier::AndImpl<false, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::MapStorage<MovableMarker> &, ecstasy::MapStorage<Position> &>>();
    assert_equals<ecstasy::query::modifier::AndImpl<false, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<const ecstasy::MapStorage<MovableMarker> &, const ecstasy::MapStorage<Position> &>>();
#ifdef ECSTASY_LOCKABLE_STORAGES
    assert_equals<ecstasy::query::modifier::AndImpl<true, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<ecstasy::MapStorage<Position>>>>();
    assert_equals<ecstasy::query::modifier::AndImpl<true, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<const ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<const ecstasy::MapStorage<Position>>>>();
#endif

    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;
    auto velocityOrVector = ecstasy::query::modifier::And(positions, velocities, vectors);
    auto maybeVector = ecstasy::query::modifier::Maybe(vectors);

    for (int i = 0; i < 13; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    velocityOrVector.reloadMask();
    static_cast<void>(velocityOrVector.getQueryData(0));
    auto query = ecstasy::query::Select<decltype(vectors)>::where(vectors, velocityOrVector);
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11000100000001"));
}

TEST(Query, XorVariadic)
{
    assert_equals<ecstasy::query::modifier::XorImpl<false, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::MapStorage<MovableMarker> &, ecstasy::MapStorage<Position> &>>();
    assert_equals<ecstasy::query::modifier::XorImpl<false, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<const ecstasy::MapStorage<MovableMarker> &, const ecstasy::MapStorage<Position> &>>();
#ifdef ECSTASY_LOCKABLE_STORAGES
    assert_equals<ecstasy::query::modifier::XorImpl<true, ecstasy::MapStorage<MovableMarker>,
                      ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<ecstasy::MapStorage<Position>>>>();
    assert_equals<ecstasy::query::modifier::XorImpl<true, const ecstasy::MapStorage<MovableMarker>,
                      const ecstasy::MapStorage<Position>>::Internal,
        std::tuple<ecstasy::thread::LockableView<const ecstasy::MapStorage<MovableMarker>>,
            ecstasy::thread::LockableView<const ecstasy::MapStorage<Position>>>>();
#endif

    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;
    auto velocityOrVector = ecstasy::query::modifier::Xor(positions, velocities, vectors);
    auto maybeVector = ecstasy::query::modifier::Maybe(vectors);

    for (int i = 0; i < 13; i++) {
        entities.create();
        if (i % 2 == 0)
            positions.emplace(i, i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            velocities.emplace(i, i * 10, i * 2);
        if (i % 4 == 0)
            vectors.emplace(i, i * 4, i * 6);
    }

    velocityOrVector.reloadMask();
    static_cast<void>(velocityOrVector.getQueryData(0));
    auto query = ecstasy::query::Select<decltype(maybeVector)>::where(maybeVector, velocityOrVector);

    /// Xor of three inputs return true if an odd number of its input are true
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11011100001101"));
}

/// Constant - Constant
/// Constant - Value Member
/// Constant - Member function
/// Value Member - Constant
/// Value Member - Value Member
/// Value Member - Member function
/// Member function - Constant
/// Member function - Value Member
/// Member function - Member function

struct Life {
    int getValue() const
    {
        return value;
    }
    int value;

    Life(int v) : value(v)
    {
    }
};

TEST(Condition, MemberToConst)
{
    Life life{42};
    Life death{-42};

    using memberCondition = ecstasy::query::Condition<&Life::value, 0, std::less<>>;
    using methodCondition = ecstasy::query::Condition<&Life::getValue, 0, std::less<>>;

    GTEST_ASSERT_FALSE(memberCondition::test(life));
    GTEST_ASSERT_FALSE(methodCondition::test(life));
    GTEST_ASSERT_TRUE(memberCondition::test(death));
}

TEST(QueryImplementation, Conditional)
{
    ecstasy::MapStorage<Life> lifes;

    for (int i = 0; i < 13; i++)
        lifes.emplace(i, (i % 2 == 0) ? 42 : -42);

    auto query = ecstasy::query::QueryImplementation<util::meta::Traits<decltype(lifes)>,
        util::meta::Traits<ecstasy::query::Condition<&Life::value, 0, std::less<>>>>(lifes);

    for (auto [life] : query)
        GTEST_ASSERT_TRUE(life.value < 0);
}

TEST(QueryImplementation, BatchQuery)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    int entities = 200;

    for (int i = 0; i < entities; i++) {
        positions.emplace(i, i, i * 2);
        velocities.emplace(i, i * 2, i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto [pos, vel] : ecstasy::query::Query(positions, velocities)) {
        pos.x += vel.v.x;
        pos.y += vel.v.y;
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cerr << "Took " << duration.count() << "s in single thread" << std::endl;

    for (int i = 0; i < entities; i++) {
        auto &pos = positions.getQueryData(i);
        GTEST_ASSERT_EQ(pos.x, i + i * 2);
        GTEST_ASSERT_EQ(pos.y, i * 2 + i);
    }

    start = std::chrono::high_resolution_clock::now();
    ecstasy::query::Query(positions, velocities).splitThreads(20, [](auto components) {
        auto &[pos, vel] = components;
        pos.x += vel.v.x;
        pos.y += vel.v.y;
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    });
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durationMthreads = end - start;
    std::cerr << "Took " << durationMthreads.count() << "s in batch query" << std::endl;
    GTEST_ASSERT_LT(durationMthreads.count(), duration.count());

    for (int i = 0; i < entities; i++) {
        auto &pos = positions.getQueryData(i);
        GTEST_ASSERT_EQ(pos.x, i + i * 2 * 2);
        GTEST_ASSERT_EQ(pos.y, i * 2 + i * 2);
    }
}
