#include <gtest/gtest.h>
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/MapStorage.hpp"

TEST(Entities, all)
{
    ecstasy::Entities entities;

    /// Create an entity alive
    ecstasy::Entity e = entities.create();
    EXPECT_EQ(e.getIndex(), 0);
    EXPECT_EQ(e.getGeneration(), 1);
    EXPECT_TRUE(entities.isAlive(e));

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