#include <gtest/gtest.h>
#include <math.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resource/Resource.hpp"
#include "ecstasy/resource/entity/RegistryEntity.hpp"
#include "ecstasy/storage/MapStorage.hpp"
#include "ecstasy/system/ISystem.hpp"

class A : public ecstasy::ISystem {
  public:
    void run(ecstasy::Registry &registry) override
    {
        (void)registry;
        std::cout << "A";
    }
};

class B : public ecstasy::ISystem {
  public:
    void run(ecstasy::Registry &registry) override
    {
        (void)registry;
        std::cout << "B";
    }
};

struct Counter : public ecstasy::Resource {
    int value;

    Counter(int initial = 0)
    {
        this->value = initial;
    }

    void count()
    {
        ++this->value;
    }
};

namespace test
{
    struct Comp {
    };
} // namespace test

SET_COMPONENT_STORAGE(test::Comp, ecstasy::MapStorage)

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
    Vector2i v;

    Position(int x, int y) : v(x, y)
    {
    }
};

struct Velocity {
    Vector2i v;

    Velocity(int x, int y) : v(x, y)
    {
    }
};

struct Size {
    Vector2i v;

    Size(int x, int y) : v(x, y)
    {
    }
};

using Density = int;

struct Gravity : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [entity, velocity, density] : registry.query<ecstasy::Entities, Velocity, Density>())
            velocity.v.y += 2 * density;
    }
};

struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.query<Position, Velocity>()) {
            position.v.x += velocity.v.x;
            position.v.y += velocity.v.y;
        }
    }
};

TEST(Registry, systems)
{
    testing::internal::CaptureStdout();
    ecstasy::Registry registry;

    registry.addSystem<A>();
    EXPECT_THROW(registry.addSystem<A>(), std::logic_error);
    EXPECT_THROW(registry.runSystem<B>(), std::logic_error);
    registry.addSystem<B>();

    registry.runSystem<A>();
    registry.runSystems();
    registry.runSystems();

    /// Order depends on type_index except for the first runSystem
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE((output == "AABAB") || (output == "ABABA"));
}

TEST(Registry, resources)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    /// Resource not present
    EXPECT_THROW(registry.getResource<Counter>(), std::logic_error);
    EXPECT_THROW(cregistry.getResource<Counter>(), std::logic_error);

    /// Add resource with an initial value of 5 and add one
    registry.addResource<Counter>(5).count();
    EXPECT_EQ(registry.getResource<Counter>().value, 6);
    EXPECT_EQ(cregistry.getResource<Counter>().value, 6);

    /// Try to add resource already present
    EXPECT_THROW(registry.addResource<Counter>(), std::logic_error);
}

TEST(Registry, storages)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    /// Storage not present
    EXPECT_THROW(registry.getStorage<A>(), std::logic_error);
    EXPECT_THROW(cregistry.getStorage<A>(), std::logic_error);
    /// First call instantiate the storage and the second only fetch it.
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);
    EXPECT_EQ(cregistry.getStorage<A>().size(), 0);

    /// Add resource with an initial value of 5 and add one
    registry.addStorage<Counter>();
    EXPECT_EQ(registry.getStorage<Counter>().size(), 0);
}

TEST(Registry, erase_entities)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    for (int i = 0; i < 10; i++)
        registry.entityBuilder().with<Position>(1, 2).with<Size>(3, 4).build();
    GTEST_ASSERT_EQ(cregistry.getEntities().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111111111"));
    registry.eraseEntity(1);
    registry.eraseEntity(5);
    GTEST_ASSERT_EQ(registry.getEntities().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111011101"));

    GTEST_ASSERT_TRUE(cregistry.getEntities().isAlive(registry.getEntity(0)));
    GTEST_ASSERT_FALSE(registry.getEntities().isAlive(registry.getEntity(1)));
    GTEST_ASSERT_FALSE(registry.getEntities().isAlive(registry.getEntity(5)));
}

TEST(Registry, EntityBuilder)
{
    ecstasy::Registry registry;

    /// Build the entity
    ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
    builder.with<Position>(1, 2).with<Velocity>(3, 4).with<Size>(4, 5);
    EXPECT_THROW(builder.with<Position>(42, 84), std::logic_error);
    ecstasy::RegistryEntity e(builder.build(), registry);

    /// Mess with the builder after build done
    EXPECT_THROW(builder.with<Vector2i>(5, 2), std::logic_error);
    EXPECT_THROW(builder.build(), std::logic_error);

    /// Test if entity has all attached components
    EXPECT_TRUE(e.has<Position>());
    EXPECT_TRUE(e.has<Velocity>());
    EXPECT_TRUE(e.has<Size>());
    EXPECT_FALSE(e.has<Vector2i>());
}

TEST(Registry, functionnal)
{
    ecstasy::Registry registry;

    registry.addSystem<Gravity>();
    registry.addSystem<Movement>();

    ecstasy::RegistryEntity e1 =
        ecstasy::RegistryEntity(registry.entityBuilder().with<Position>(0, 0).build(), registry);
    ecstasy::RegistryEntity e2 =
        ecstasy::RegistryEntity(registry.entityBuilder().with<Position>(0, 0).with<Density>(3).build(), registry);
    ecstasy::RegistryEntity e3 =
        ecstasy::RegistryEntity(registry.entityBuilder().with<Position>(0, 0).with<Velocity>(2, 4).build(), registry);
    ecstasy::RegistryEntity e4 = ecstasy::RegistryEntity(
        registry.entityBuilder().with<Position>(0, 0).with<Velocity>(1, 2).with<Density>(2).build(), registry);

    /// Systems
    for (int i = 0; i < 10; i++) {
        /// E1
        GTEST_ASSERT_EQ(e1.get<Position>().v, Vector2i(0, 0));
        /// E2
        GTEST_ASSERT_EQ(e2.get<Position>().v, Vector2i(0, 0));
        /// E3
        GTEST_ASSERT_EQ(e3.get<Position>().v, Vector2i(2 * i, 4 * i));
        GTEST_ASSERT_EQ(e3.get<Velocity>().v, Vector2i(2, 4));
        /// E4
        GTEST_ASSERT_EQ(e4.get<Position>().v, Vector2i(1 * i, 2 * i + 4 * ((i * (i + 1)) / 2)));
        GTEST_ASSERT_EQ(e4.get<Velocity>().v, Vector2i(1, 2 + 4 * i));

        registry.runSystem<Gravity>();
        registry.runSystem<Movement>();
    }
}