#include <gtest/gtest.h>
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/resources/entity/Entity.hpp"
#include "ecstasy/storages/MapStorage.hpp"

TEST(Entities, all)
{
    ecstasy::Entities entities;
    ecstasy::Entities entities2;

    /// Entity has not been created
    EXPECT_FALSE(entities.isAlive(entities.get(0)));

    /// Create an entity alive
    ecstasy::Entity e = entities.create();
    EXPECT_EQ(e.getIndex(), 0);
    EXPECT_EQ(e.getGeneration(), 1);
    EXPECT_TRUE(entities.isAlive(e));

    /// There is no entity alive in the entities2 storage
    EXPECT_FALSE(entities2.isAlive(e));

    /// Create a second entity dead
    ecstasy::Entity e2 = entities.create(false);
    EXPECT_EQ(e2.getIndex(), 1);
    EXPECT_EQ(e2.getGeneration(), 1);
    EXPECT_FALSE(entities.isAlive(e2));

    /// Kill our first entity
    EXPECT_TRUE(entities.erase(e));
    EXPECT_FALSE(entities.erase(e));

    /// Create a new entity, it should use the empty slot from our previous entity kill
    ecstasy::Entity e3 = entities.create();
    e2 = entities.get(0);
    EXPECT_EQ(e3 <=> e2, std::strong_ordering::equal);
    EXPECT_EQ(e3.getIndex(), 0);
    EXPECT_EQ(e3.getGeneration(), 2);

    /// Unexisting entity
    EXPECT_FALSE(entities.isAlive(entities.get(42)));
    EXPECT_FALSE(entities.erase(entities.get(42)));
}

struct Vector2i {
    int x;
    int y;

    Vector2i(int px, int py) : x(px), y(py)
    {
    }
};

using Position = Vector2i;
using Velocity = Vector2i;
using Size = Vector2i;

TEST(Entities, builder)
{
    ecstasy::MapStorage<Position> positions;
    ecstasy::MapStorage<Velocity> velocities;
    ecstasy::MapStorage<Size> sizes;
    ecstasy::MapStorage<Vector2i> vectors;
    ecstasy::Entities entities;

    /// Build the entity
    ecstasy::Entities::Builder builder = entities.builder();
    builder.with(positions, 1, 2).with(velocities, 3, 4).with(sizes, 4, 5);
    EXPECT_THROW(builder.with(positions, 42, 84), std::logic_error);
    ecstasy::Entity e = builder.build();

    /// Mess with the builder after build done
    EXPECT_THROW(builder.with(vectors, 5, 2), std::logic_error);
    EXPECT_THROW(builder.build(), std::logic_error);

    /// Test if entity has all attached components
    EXPECT_TRUE(e.has(positions));
    EXPECT_TRUE(e.has(velocities));
    EXPECT_TRUE(e.has(sizes));
    EXPECT_FALSE(e.has(vectors));
}

TEST(Entities, deletion)
{
    ecstasy::Entities entities;

    /// Create 10 entities
    for (int i = 0; i < 10; i++)
        entities.create();

    /// Save them for generation tests
    ecstasy::Entity e0 = entities.get(0);
    ecstasy::Entity e2 = entities.get(2);

    /// 10 entities
    GTEST_ASSERT_EQ(entities.getMask(), util::BitSet("1111111111"));

    /// Invalid entities (not existing)
    GTEST_ASSERT_FALSE(entities.erase(entities.get(50)));
    GTEST_ASSERT_FALSE(entities.kill(entities.get(50)));

    /// Still 10 entities
    GTEST_ASSERT_EQ(entities.getMask(), util::BitSet("1111111111"));
    /// Erase the first and mark 2 for deletion
    GTEST_ASSERT_TRUE(entities.erase(entities.get(0)));
    GTEST_ASSERT_TRUE(entities.erase(entities.get(8)));
    /// 5 then 2 to have first case resize killed bitset and 2nd doesn't (max coverage)
    GTEST_ASSERT_TRUE(entities.kill(entities.get(5)));
    GTEST_ASSERT_TRUE(entities.kill(entities.get(2)));
    GTEST_ASSERT_EQ(entities.getMask(), util::BitSet("1011111110"));

    GTEST_ASSERT_FALSE(entities.isAlive(e0));
    GTEST_ASSERT_TRUE(entities.isAlive(e2));

    /// Entities marked for deletion are removed
    auto deleted = entities.maintain();
    GTEST_ASSERT_EQ(entities.getMask(), util::BitSet("1011011010"));
    GTEST_ASSERT_EQ(deleted, std::vector<ecstasy::Entity>({entities.get(2), entities.get(5)}));

    /// Messing again with the deletions
    /// Invalid: not alive
    GTEST_ASSERT_FALSE(entities.erase(entities.get(0)));
    GTEST_ASSERT_FALSE(entities.kill(entities.get(0)));

    /// Create a new entity which will take index 0 to test generation conditions
    entities.create();
    GTEST_ASSERT_TRUE(entities.isAlive(entities.get(0)));
    GTEST_ASSERT_EQ(entities.get(0).getGeneration(), 2);
    GTEST_ASSERT_EQ(e0.getGeneration(), 1);
    /// Invalid: different generations
    GTEST_ASSERT_FALSE(entities.erase(e0));
    GTEST_ASSERT_FALSE(entities.kill(e0));
}