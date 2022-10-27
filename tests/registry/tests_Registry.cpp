#include <gtest/gtest.h>
#include <math.h>
#include "ecstasy/query/concepts/GetMissingTypes.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Not.hpp"
#include "ecstasy/resource/Resource.hpp"
#include "ecstasy/resource/entity/RegistryEntity.hpp"
#include "ecstasy/storage/MapStorage.hpp"
#include "ecstasy/system/ISystem.hpp"

#ifdef __GNUG__
    #include <cstdlib>
    #include <cxxabi.h>
    #include <memory>

std::string demangle(const char *name)
{
    int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void (*)(void *)> res{abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

    return (status == 0) ? res.get() : name;
}

#else

// does nothing if not g++
std::string demangle(const char *name)
{
    return name;
}
#endif

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

/// Movable marker to test complex queries
struct Movable {
};
struct Static {
};

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
        ecstasy::ModifiersAllocator allocator;

        for (auto [position, velocity] :
            registry.select<Position, Velocity>().where<Position, Movable, Velocity, ecstasy::Not<Static>>(allocator)) {
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
    registry.eraseEntity(registry.getEntity(1));
    registry.eraseEntity(registry.getEntity(5));
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
    ecstasy::RegistryEntity e3 = ecstasy::RegistryEntity(
        registry.entityBuilder().with<Position>(0, 0).with<Velocity>(2, 4).with<Movable>().build(), registry);
    ecstasy::RegistryEntity e4 = ecstasy::RegistryEntity(
        registry.entityBuilder().with<Position>(0, 0).with<Velocity>(1, 2).with<Density>(2).with<Movable>().build(),
        registry);
    ecstasy::RegistryEntity e5 = ecstasy::RegistryEntity(
        registry.entityBuilder().with<Position>(0, 0).with<Velocity>(1, 2).with<Density>(2).build(), registry);
    ecstasy::RegistryEntity e6 = ecstasy::RegistryEntity(registry.entityBuilder()
                                                             .with<Position>(0, 0)
                                                             .with<Velocity>(1, 2)
                                                             .with<Density>(2)
                                                             .with<Movable>()
                                                             .with<Static>()
                                                             .build(),
        registry);
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
        /// E5 -> doesn't have the Movable marker which blocks the Movement System
        GTEST_ASSERT_EQ(e5.get<Position>().v, Vector2i(0, 0));
        GTEST_ASSERT_EQ(e5.get<Velocity>().v, Vector2i(1, 2 + 4 * i));
        /// E5 -> Have the Static marker which blocks the Movement System
        GTEST_ASSERT_EQ(e6.get<Position>().v, Vector2i(0, 0));
        GTEST_ASSERT_EQ(e6.get<Velocity>().v, Vector2i(1, 2 + 4 * i));

        registry.runSystem<Gravity>();
        registry.runSystem<Movement>();
    }
}

TEST(Registry, MaybeQuery)
{
    ecstasy::Registry registry;
    ecstasy::ModifiersAllocator allocator;

    for (int i = 0; i < 13; i++) {
        auto builder = registry.entityBuilder();
        if (i % 2 == 0)
            builder.with<Position>(i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            builder.with<Velocity>(i * 10, i * 2);
        if (i % 4 == 0)
            builder.with<Density>(i * 4);
        builder.build();
    }

    auto query = registry.query<Position, Velocity, ecstasy::Maybe<Density>>(allocator);
    auto query2 = registry.query<Position, Velocity>();
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11000101000001"));
    GTEST_ASSERT_EQ(query.getMask(), query2.getMask());

    auto it = query.begin();
    /// 0
    {
        size_t index = 0;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_NE(density, nullptr);
        GTEST_ASSERT_EQ(*density, index * 4);
    }

    /// 6
    ++it;
    {
        size_t index = 6;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_EQ(density, nullptr);
    }
    /// 8
    ++it;
    {
        size_t index = 8;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_NE(density, nullptr);
        GTEST_ASSERT_EQ(*density, index * 4);
    }
    /// 12
    ++it;
    {
        size_t index = 12;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_NE(density, nullptr);
        GTEST_ASSERT_EQ(*density, index * 4);
    }
}

TEST(Registry, MaybeSelect)
{
    ecstasy::Registry registry;
    ecstasy::ModifiersAllocator allocator;

    for (int i = 0; i < 13; i++) {
        auto builder = registry.entityBuilder();
        if (i % 2 == 0)
            builder.with<Position>(i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            builder.with<Velocity>(i * 10, i * 2);
        if (i % 4 == 0)
            builder.with<Density>(i * 4);
        builder.build();
    }

    auto query = registry.query<Position, Velocity, ecstasy::Maybe<Density>>(allocator);
    auto select =
        registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>(
            allocator);
    GTEST_ASSERT_EQ(query.getMask(), select.getMask());
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11000101000001"));
}

TEST(Registry, get_missing_types)
{
    /// Get Missing Types basic test
    {
        using expected = std::tuple<int>;
        using got = ecstasy::query::available_types<char, float, double>::get_missings_t<float, int>;
        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    // clang-format off
    /// Missing one modifier
    {
        using got = ecstasy::query::available_types<
            ecstasy::queryable_type_t<Position>,
            ecstasy::queryable_type_t<Velocity>>::
            get_missings_t<
                ecstasy::queryable_type_t<Position>, 
                ecstasy::queryable_type_t<ecstasy::Maybe<Density>>>;
        using expected = std::tuple<ecstasy::query::modifier::Maybe<ecstasy::getStorageType<Density>>>;
        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    /// Missing one queryable
    {
        using got = ecstasy::query::available_types<ecstasy::queryable_type_t<Velocity>>::
            get_missings_t<ecstasy::queryable_type_t<Position>>;
        using expected = std::tuple<ecstasy::getStorageType<Position>>;
        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }

    /// missing one queryable and one modifier
    {
        using got = ecstasy::query::available_types<ecstasy::queryable_type_t<Velocity>>::
            get_missings_t<ecstasy::queryable_type_t<Position>, ecstasy::queryable_type_t<ecstasy::Maybe<Density>>>;
        using expected = std::tuple<ecstasy::getStorageType<Position>, ecstasy::query::modifier::Maybe<ecstasy::getStorageType<Density>>>;
        GTEST_ASSERT_EQ(typeid(expected), typeid(got));
    }
    // clang-format on
}

TEST(Registry, ImplicitWhere)
{
    ecstasy::Registry registry;
    ecstasy::ModifiersAllocator allocator;

    for (int i = 0; i < 13; i++) {
        auto builder = registry.entityBuilder();
        if (i % 2 == 0)
            builder.with<Position>(i * 2, i * 10);
        if (i % 3 == 0 || i == 8)
            builder.with<Velocity>(i * 10, i * 2);
        if (i % 4 == 0)
            builder.with<Density>(i * 4);
        builder.build();
    }

    /// Missing standard component, With allocator
    {
        auto explicitQuery = registry.select<Position>().where<Position, Velocity>(allocator);
        auto implicitQuery = registry.select<Position>().where<Velocity>(allocator);
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }

    /// Missing standard component, Wihtout allocator
    {
        auto explicitQuery = registry.select<Position>().where<Position, Velocity>();
        auto implicitQuery = registry.select<Position>().where<Velocity>();
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }

    /// Missing modifier, With allocator (required)
    {
        auto explicitQuery =
            registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>(
                allocator);
        auto implicitQuery = registry.select<ecstasy::Maybe<Density>, Position>().where<Position, Velocity>(allocator);
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }

    /// Missing modifier and component, With allocator (required)
    {
        auto explicitQuery =
            registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>(
                allocator);
        auto implicitQuery = registry.select<Position, ecstasy::Maybe<Density>>().where<Velocity>(allocator);
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }
}