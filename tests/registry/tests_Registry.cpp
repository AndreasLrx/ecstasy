#include <gtest/gtest.h>
#include <math.h>
#include <thread>
#include "ecstasy/query/conditions/include.hpp"
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/registry/modifiers/And.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Not.hpp"
#include "ecstasy/registry/modifiers/Or.hpp"
#include "ecstasy/resources/IResource.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "ecstasy/storages/MarkerStorage.hpp"
#include "ecstasy/system/ISystem.hpp"
#include "util/StackAllocator.hpp"
#include "util/meta/outer_join.hpp"

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

struct Counter : public ecstasy::IResource {
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
    struct Comp {};
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

struct NeedRegistry {
    const ecstasy::Registry &registry;

    NeedRegistry(const ecstasy::Registry &aRegistry) : registry(aRegistry)
    {
    }
};

struct NeedStorage {
    const ecstasy::MapStorage<Position> &storage;

    NeedStorage(const ecstasy::MapStorage<Position> &aStorage) : storage(aStorage)
    {
    }
};

struct NeedResource {
    Counter &resource;

    NeedResource(Counter &aResource) : resource(aResource)
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
    using StorageType = ecstasy::MarkerStorage<Movable>;
};
struct Static {
    using StorageType = ecstasy::MarkerStorage<Static>;
};

struct Gravity : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [entity, velocity, density] : registry.query<ecstasy::Entities, Velocity, const Density>())
            velocity.v.y += 2 * density;
    }
};

struct Movement : public ecstasy::ISystem {
    void run(ecstasy::Registry &registry) override final
    {
        for (auto [position, velocity] : registry.select<Position, const Velocity>()
                                             .where<Position, Movable, const Velocity, ecstasy::Not<Static>>()) {
            position.v.x += velocity.v.x;
            position.v.y += velocity.v.y;
        }
    }
};

template <typename T1, typename T2>
void assert_equals()
{
    static_assert(std::is_same_v<T1, T2>, "Types differs.");
}

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
    ASSERT_FALSE(registry.hasResource<Counter>());
    EXPECT_THROW(static_cast<void>(registry.getResource<Counter>()), std::logic_error);
    EXPECT_THROW(static_cast<void>(cregistry.getResource<Counter>()), std::logic_error);

    /// Add resource with an initial value of 5 and add one
    registry.addResource<Counter>(5).count();
    ASSERT_TRUE(registry.hasResource<Counter>());
    EXPECT_EQ(registry.getResource<Counter>().get().value, 6);
    EXPECT_EQ(cregistry.getResource<Counter>().get().value, 6);

    /// Try to add resource already present
    EXPECT_THROW(static_cast<void>(registry.addResource<Counter>()), std::logic_error);

    {
        auto query = registry.query<ecstasy::Entities>();
        for (auto [e] : query) {
            static_assert(std::is_same_v<decltype(e), ecstasy::Entity>);
        }
    }

    {
        auto cquery = registry.query<const ecstasy::Entities>();
        for (auto [e] : cquery) {
            assert_equals<decltype(e), ecstasy::Entity>();
        }
    }
}

TEST(Registry, storages)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    /// Storage not present
    EXPECT_THROW(static_cast<void>(registry.getStorage<A>()), std::logic_error);
    EXPECT_THROW(static_cast<void>(cregistry.getStorage<A>()), std::logic_error);
    /// First call instantiate the storage and the second only fetch it.
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);
    EXPECT_EQ(registry.getStorageSafe<const A>().size(), 0);
    EXPECT_EQ(cregistry.getStorage<A>().size(), 0);
    EXPECT_EQ(cregistry.getStorage<const A>().size(), 0);
    assert_equals<decltype(registry.getStorage<A>()), ecstasy::MapStorage<A> &>();
    assert_equals<decltype(registry.getStorage<const A>()), const ecstasy::MapStorage<A> &>();
    assert_equals<decltype(registry.getStorageSafe<A>()), ecstasy::MapStorage<A> &>();
    assert_equals<decltype(registry.getStorageSafe<const A>()), const ecstasy::MapStorage<A> &>();

    /// Add resource with an initial value of 5 and add one
    registry.addStorage<Counter>();
    EXPECT_EQ(registry.getStorage<Counter>().size(), 0);
}

TEST(Registry, getQueryable)
{
    ecstasy::Registry registry;

    assert_equals<decltype(registry.getQueryable<Position>()), ecstasy::MapStorage<Position> &>();
    assert_equals<decltype(registry.getQueryable<const Position>()), const ecstasy::MapStorage<Position> &>();
    assert_equals<decltype(registry.getQueryable<ecstasy::MapStorage<Position>>()), ecstasy::MapStorage<Position> &>();
    static_assert(ecstasy::IsStorage<const ecstasy::MapStorage<Position>>);
    static_assert(ecstasy::query::Queryable<const ecstasy::MapStorage<Position>>);
    assert_equals<decltype(registry.getQueryable<const ecstasy::MapStorage<Position>>()),
        const ecstasy::MapStorage<Position> &>();
    assert_equals<decltype(registry.getQueryable<ecstasy::Entities>()), ecstasy::Entities &>();
    assert_equals<decltype(registry.getQueryable<const ecstasy::Entities>()), const ecstasy::Entities &>();
}

TEST(Registry, eraseEntity)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    for (int i = 0; i < 10; i++)
        registry.entityBuilder().with<Position>(1, 2).with<Size>(3, 4).build();
    GTEST_ASSERT_EQ(cregistry.getEntities().get().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_TRUE(registry.eraseEntity(registry.getEntity(1)));
    GTEST_ASSERT_FALSE(registry.eraseEntity(registry.getEntity(1)));
    GTEST_ASSERT_TRUE(registry.eraseEntity(registry.getEntity(5)));
    GTEST_ASSERT_EQ(registry.getEntities().get().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111011101"));

    GTEST_ASSERT_TRUE(cregistry.getEntities().get().isAlive(registry.getEntity(0)));
    GTEST_ASSERT_FALSE(registry.getEntities().get().isAlive(registry.getEntity(1)));
    GTEST_ASSERT_FALSE(registry.getEntities().get().isAlive(registry.getEntity(5)));
}

TEST(Registry, eraseEntities)
{
    ecstasy::Registry registry;
    const ecstasy::Registry &cregistry = registry;

    for (int i = 0; i < 10; i++)
        registry.entityBuilder().with<Position>(1, 2).with<Size>(3, 4).build();
    GTEST_ASSERT_EQ(cregistry.getEntities().get().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111111111"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111111111"));
    ecstasy::Entity toDelete[] = {registry.getEntity(1), registry.getEntity(5)};
    GTEST_ASSERT_EQ(registry.eraseEntities(std::span(toDelete)), 2);
    GTEST_ASSERT_EQ(registry.eraseEntities(std::span(toDelete)), 0);
    GTEST_ASSERT_EQ(registry.getEntities().get().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Position>().getMask(), util::BitSet("1111011101"));
    GTEST_ASSERT_EQ(registry.getStorage<Size>().getMask(), util::BitSet("1111011101"));

    GTEST_ASSERT_TRUE(cregistry.getEntities().get().isAlive(registry.getEntity(0)));
    GTEST_ASSERT_FALSE(registry.getEntities().get().isAlive(registry.getEntity(1)));
    GTEST_ASSERT_FALSE(registry.getEntities().get().isAlive(registry.getEntity(5)));
}

TEST(Registry, EntityBuilder)
{
    ecstasy::Registry registry;
    registry.addResource<Counter>();

    {
        /// Build the entity
        ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
        builder.with<Position>(1, 2).with<Velocity>(3, 4).with<Size>(4, 5).with<std::vector<int>>({1, 2, 3, 4, 5, 6});
        EXPECT_THROW(builder.with<Position>(42, 84), std::logic_error);
        ecstasy::RegistryEntity e(builder.build(), registry);

        /// Mess with the builder after build done
        EXPECT_THROW(builder.with<Vector2i>(5, 2), std::logic_error);
        EXPECT_THROW(builder.build(), std::logic_error);

        /// Test if entity has all attached components
        EXPECT_TRUE(e.has<Position>());
        EXPECT_TRUE(e.has<Velocity>());
        EXPECT_TRUE(e.has<Size>());
        EXPECT_TRUE(e.has<std::vector<int>>());
        EXPECT_EQ(e.get<std::vector<int>>().size(), 6);
        EXPECT_FALSE(e.has<Vector2i>());
    }

    {
        /// Use with Registry
        ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
        ecstasy::RegistryEntity e(builder.with<NeedRegistry, ecstasy::Registry>().build(), builder.getRegistry());
        EXPECT_EQ(&e.get<NeedRegistry>().registry, &registry);
    }

    {
        /// Use Queryables with the builder
        ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
        ecstasy::RegistryEntity e(builder.with<NeedStorage, const Position>().build(), builder.getRegistry());
        EXPECT_EQ(&e.get<NeedStorage>().storage, &registry.getStorage<Position>());
    }

    {
        /// Use Queryables with the builder
        ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
        ecstasy::RegistryEntity e(
            builder.with<NeedStorage, ecstasy::MapStorage<Position>>().build(), builder.getRegistry());
        EXPECT_EQ(&e.get<NeedStorage>().storage, &registry.getStorage<Position>());
    }

    {
        /// Use Queryables with the builder
        ecstasy::Registry::EntityBuilder builder = registry.entityBuilder();
        ecstasy::RegistryEntity e(builder.with<NeedResource, Counter>().build(), builder.getRegistry());
        auto resource = registry.getResource<Counter, false>();
        EXPECT_EQ(&e.get<NeedResource>().resource, &resource.get());
    }
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

    auto query = registry.query<Position, Velocity, ecstasy::Maybe<Density>>();
    auto query2 = registry.query<Position, Velocity>();
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11000101000001"));
    GTEST_ASSERT_EQ(query.getMask(), query2.getMask());

    auto it = query.begin();
    /// 0
    {
        int index = 0;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(density);
        GTEST_ASSERT_EQ(density->get(), index * 4);
    }

    /// 6
    ++it;
    {
        int index = 6;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(density);
    }
    /// 8
    ++it;
    {
        int index = 8;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(density);
        GTEST_ASSERT_EQ(density->get(), index * 4);
    }
    /// 12
    ++it;
    {
        int index = 12;
        auto [pos, velocity, density] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_EQ(velocity.v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(density);
        GTEST_ASSERT_EQ(density->get(), index * 4);
    }
}

TEST(Registry, MaybeSelect)
{
    ecstasy::Registry registry;

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

    auto query = registry.query<Position, Velocity, ecstasy::Maybe<Density>>();
    auto select =
        registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>();

    GTEST_ASSERT_EQ(query.getMask(), select.getMask());
    GTEST_ASSERT_EQ(query.getMask(), util::BitSet("11000101000001"));
}

TEST(Registry, ImplicitWhere)
{
    ecstasy::Registry registry;

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

    /// Missing standard component, Wihtout allocator
    {
        auto explicitQuery = registry.select<Position>().where<Position, Velocity>();
        auto implicitQuery = registry.select<Position>().where<Velocity>();
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }

    /// Missing modifier
    {
        auto explicitQuery =
            registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>();
        auto implicitQuery = registry.select<ecstasy::Maybe<Density>, Position>().where<Position, Velocity>();
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }

    /// Missing modifier and component
    {
        auto explicitQuery =
            registry.select<Position, ecstasy::Maybe<Density>>().where<Position, Velocity, ecstasy::Maybe<Density>>();
        auto implicitQuery = registry.select<Position, ecstasy::Maybe<Density>>().where<Velocity>();
        GTEST_ASSERT_EQ(explicitQuery.getMask(), implicitQuery.getMask());
    }
}

TEST(Registry, OrSelect)
{
    ecstasy::Registry registry;

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

    // clang-format off
    auto query = registry.select<Position>().where<ecstasy::Or<Velocity, Density>>();
    auto query2 = registry.select<Position, ecstasy::Maybe<Velocity>, ecstasy::Maybe<Density>>().where<ecstasy::Or<Velocity, Density>>();
    auto posAndVel = registry.query<Position, Velocity>();
    auto posAndDensity = registry.query<Position, Density>();

    GTEST_ASSERT_EQ(posAndVel.getMask(),     util::BitSet("11000101000001"));
    GTEST_ASSERT_EQ(posAndDensity.getMask(), util::BitSet("11000100010001"));
    GTEST_ASSERT_EQ(query.getMask(),         util::BitSet("11000101010001")); /// Or of the two precedent masks
    GTEST_ASSERT_EQ(query.getMask(), query2.getMask());
    // clang-format on

    auto it = query2.begin();
    /// 0 (vel and vec)
    {
        int index = 0;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), index * 4);
    }

    /// 4 (!vel and vec)
    ++it;
    {
        int index = 4;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_FALSE(vel);
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), index * 4);
    }

    /// 6 (vel and !vec)
    ++it;
    {
        int index = 6;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_FALSE(vec);
    }

    /// 8 (vel and vec)
    ++it;
    {
        int index = 8;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), index * 4);
    }

    /// 12 (vel and vec)
    ++it;
    {
        int index = 12;
        auto [pos, vel, vec] = *it;
        GTEST_ASSERT_EQ(pos.v, Vector2i(index * 2, index * 10));
        GTEST_ASSERT_TRUE(vel);
        GTEST_ASSERT_EQ(vel->get().v, Vector2i(index * 10, index * 2));
        GTEST_ASSERT_TRUE(vec);
        GTEST_ASSERT_EQ(vec->get(), index * 4);
    }
}

TEST(Registry, withoutEntities)
{
    ecstasy::Registry registry(false);

    GTEST_ASSERT_FALSE(registry.hasResource<ecstasy::Entities>());
}

struct Life {
    int value;
    int getValue() const
    {
        return value;
    }
    Life(int v) : value(v)
    {
    }
};

struct Shield {
    int value;
    int getValue() const
    {
        return value;
    }
    Shield(int v) : value(v)
    {
    }
};

TEST(Registry, ConditionnalQueryConstConst)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::query::Condition<15, 0, std::less<>>>()) {
        /// Must be false for some but no iterations should be done because 15 is never lower than 0.
        GTEST_ASSERT_TRUE(life.value < 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 0);
}

TEST(Registry, ConditionnalQueryConstMember)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::query::Condition<0, &Life::value, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value > 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 7);
}

TEST(Registry, ConditionnalQueryConstGetter)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::query::Condition<0, &Life::getValue, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value > 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 7);
}

TEST(Registry, ConditionnalQueryMemberConst)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::query::Condition<&Life::value, 0, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 6);
}

TEST(Registry, ConditionnalQueryMemberMember)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++) {
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).with<Shield>((i % 3 == 0) ? -100 : 40).build();
    }

    size_t i = 0;
    for (auto [life, shield] :
        registry.select<Life, Shield>().where<ecstasy::query::Condition<&Life::value, &Shield::value, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < shield.value);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 4);
}

TEST(Registry, ConditionnalQueryMemberGetter)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++) {
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).with<Shield>((i % 3 == 0) ? -100 : 40).build();
    }

    size_t i = 0;
    for (auto [life, shield] : registry.select<Life, Shield>()
                                   .where<ecstasy::query::Condition<&Life::value, &Shield::getValue, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < shield.value);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 4);
}

TEST(Registry, ConditionnalQueryGetterConst)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::query::Condition<&Life::getValue, 0, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 6);
}

TEST(Registry, ConditionnalQueryGetterMember)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).with<Shield>((i % 3 == 0) ? -100 : 40).build();

    size_t i = 0;
    for (auto [life, shield] : registry.select<Life, Shield>()
                                   .where<ecstasy::query::Condition<&Life::getValue, &Shield::value, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < shield.value);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 4);
}

TEST(Registry, ConditionnalQueryGetterGetter)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 42 : -42).with<Shield>((i % 3 == 0) ? -100 : 40).build();

    size_t i = 0;
    for (auto [life, shield] :
        registry.select<Life, Shield>()
            .where<ecstasy::query::Condition<&Life::getValue, &Shield::getValue, std::less<>>>()) {
        GTEST_ASSERT_TRUE(life.value < shield.value);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 4);
}

TEST(Registry, Less)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::Less<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value < 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 6);
}

TEST(Registry, LessEqual)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : -42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::LessEqual<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value <= 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 13);
}

TEST(Registry, Greater)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : 42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::Greater<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value > 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 6);
}

TEST(Registry, GreaterEqual)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : 42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::GreaterEqual<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value >= 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 13);
}

TEST(Registry, EqualTo)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : 42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::EqualTo<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value == 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 7);
}

TEST(Registry, NotEqualTo)
{
    ecstasy::Registry registry;

    for (int i = 0; i < 13; i++)
        registry.entityBuilder().with<Life>((i % 2 == 0) ? 0 : 42).build();

    size_t i = 0;
    for (auto [life] : registry.select<Life>().where<ecstasy::NotEqualTo<&Life::value, 0>>()) {
        GTEST_ASSERT_TRUE(life.value != 0);
        ++i;
    }
    GTEST_ASSERT_EQ(i, 6);
}

#define PRINT(name)                                    \
    class name : public ecstasy::ISystem {             \
      public:                                          \
        void run(ecstasy::Registry &registry) override \
        {                                              \
            (void)registry;                            \
            std::cout << #name;                        \
        }                                              \
    }

PRINT(C);
PRINT(D);
PRINT(E);
PRINT(F);

TEST(Registry, Pipeline_Implicit_Order)
{
    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    registry.addSystem<A>();
    registry.addSystem<B>();
    registry.addSystem<C>();
    registry.addSystem<D>();
    registry.addSystem<E>();
    registry.addSystem<F>();

    const ecstasy::Pipeline &pipeline = registry.getPipeline();

    GTEST_ASSERT_EQ(
        pipeline.getSystemPhase<A>().getId(), static_cast<size_t>(ecstasy::Pipeline::PredefinedPhases::OnUpdate));

    registry.runSystems();
    GTEST_ASSERT_EQ(testing::internal::GetCapturedStdout(), "ABCDEF");
}

TEST(Timer, Pipeline_system_timers_rates)
{
    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    // No timer, run every frame
    registry.addSystem<A>();
    // Run every 2 frames
    ecstasy::ISystem &b = registry.addSystem<B>();
    GTEST_ASSERT_EQ(b.getTimer().getType(), ecstasy::Timer::Type::Rate);
    EXPECT_EQ(b.getTimer().getRate(), 1);
    EXPECT_THROW(static_cast<void>(b.getTimer().getInterval()), std::runtime_error);
    b.getTimer().setRate(2);
    EXPECT_EQ(b.getTimer().getRate(), 2);
    EXPECT_THROW(static_cast<void>(b.getTimer().getInterval()), std::runtime_error);
    // Run every 4 frames
    registry.addSystem<C>().getTimer().setRate(4);
    // Run every 8 frames
    registry.addSystem<D>().getTimer().setRate(8);
    // Run every 16 frames
    registry.addSystem<E>().getTimer().setRate(16);

    for (int i = 0; i < 32; i++) {
        registry.runSystems();
    }
    // Every systems are runned at first frame, then their rate applies
    GTEST_ASSERT_EQ(
        testing::internal::GetCapturedStdout(), "ABCDEAABAABCAABAABCDAABAABCAABAABCDEAABAABCAABAABCDAABAABCAABA");
}

TEST(Timer, Pipeline_system_timers_intervals)
{
    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    // No timer, run every frame
    registry.addSystem<A>();

    // Run every 2 frames
    ecstasy::ISystem &b = registry.addSystem<B>();
    b.getTimer().setInterval(std::chrono::milliseconds(200));
    GTEST_ASSERT_EQ(b.getTimer().getType(), ecstasy::Timer::Type::TimeInterval);
    EXPECT_EQ(b.getTimer().getInterval(), std::chrono::milliseconds(200));
    EXPECT_THROW(static_cast<void>(b.getTimer().getRate()), std::runtime_error);

    // Run every 4 frames
    registry.addSystem<C>().getTimer().setInterval(std::chrono::milliseconds(350));
    // Run every 8 frames
    registry.addSystem<D>().getTimer().setInterval(std::chrono::milliseconds(800));
    // Run every 16 frames
    registry.addSystem<E>().getTimer().setInterval(std::chrono::seconds(1));

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    long loops = 0;
    while (start + std::chrono::seconds(3) > end) {
        ++loops;
        registry.runSystems();
        if (loops == 1)
            start = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        end = std::chrono::system_clock::now();
    }

    std::string captured = testing::internal::GetCapturedStdout();

    auto nb_a = std::count_if(captured.begin(), captured.end(), [](char c) {
        return c == 'A';
    });
    auto nb_b = std::count_if(captured.begin(), captured.end(), [](char c) {
        return c == 'B';
    });
    auto nb_c = std::count_if(captured.begin(), captured.end(), [](char c) {
        return c == 'C';
    });
    auto nb_d = std::count_if(captured.begin(), captured.end(), [](char c) {
        return c == 'D';
    });
    auto nb_e = std::count_if(captured.begin(), captured.end(), [](char c) {
        return c == 'E';
    });
    auto nb_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Not sure about these values, but they should be close
    GTEST_ASSERT_EQ(nb_a, loops);
    ASSERT_NEAR(nb_b, nb_ms / 200, 1);
    ASSERT_NEAR(nb_c, nb_ms / 350, 1);
    ASSERT_NEAR(nb_d, nb_ms / 800, 1);
    ASSERT_NEAR(nb_e, nb_ms / 1000, 1);
}

TEST(Registry, Pipeline_Explicit_Order)
{
    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    registry.addSystem<A, 0>();
    registry.addSystem<D, 3>();
    registry.addSystem<B, 1>();
    registry.addSystem<E, 4>();
    registry.addSystem<C, 2>();
    registry.addSystem<F, 5>();

    GTEST_ASSERT_EQ(registry.getPipeline().getSystemPhase<E>().getId(), 4);

    registry.runSystems();
    GTEST_ASSERT_EQ(testing::internal::GetCapturedStdout(), "ABCDEF");
}

TEST(Registry, Pipeline_Explicit_Descending)
{
    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    registry.addSystem<A, 5>();
    registry.addSystem<B, 4>();
    registry.addSystem<C, 3>();
    registry.addSystem<D, 2>();
    registry.addSystem<E, 1>();
    registry.addSystem<F, 0>();

    registry.runSystems();
    GTEST_ASSERT_EQ(testing::internal::GetCapturedStdout(), "FEDCBA");
}

TEST(Registry, Pipeline_Custom_Phases)
{
    const size_t abc = 1;
    const size_t def = 2;

    ecstasy::Registry registry;
    testing::internal::CaptureStdout();

    registry.addSystem<A, abc>();
    registry.addSystem<B, abc>();
    registry.addSystemInPhase<D>(def);
    registry.addSystemInPhase<C>(abc);
    registry.addSystem<E, def>();
    registry.addSystem<F, def>();

    registry.runSystems();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "ABCDEF");

    testing::internal::CaptureStdout();
    registry.runSystemsPhase(abc);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "ABC");
    testing::internal::CaptureStdout();
    registry.runSystemsPhase(def);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "DEF");
}

TEST(Registry, clear)
{
    ecstasy::Registry registry;

    registry.addSystem<A>();
    registry.addResource<Counter>();
    registry.entityBuilder().with<Life>(5).build();

    GTEST_ASSERT_TRUE(registry.hasResource<Counter>());
    GTEST_ASSERT_EQ(registry.getStorage<Life>().size(), 1);
    registry.clear();
    GTEST_ASSERT_FALSE(registry.hasResource<Counter>());
    EXPECT_THROW(static_cast<void>(registry.getStorage<Life>()), std::logic_error);
}

TEST(Registry, modifiers_allocator_size)
{
    // Non modifiers must have a 0 size (no need for allocation)
    GTEST_ASSERT_EQ(ecstasy::modifier_allocator_size_v<Velocity>, 0);
    GTEST_ASSERT_EQ(ecstasy::modifier_allocator_size_v<Position>, 0);

    // Modifiers must have the size of the effective modifier (not the registry modifier helper)
    GTEST_ASSERT_EQ(
        ecstasy::modifier_allocator_size_v<ecstasy::Maybe<Velocity>>, sizeof(ecstasy::Maybe<Velocity>::Modifier));

    size_t computed_size, expected_size;
    computed_size = ecstasy::modifier_allocator_size_v<ecstasy::Or<Velocity, Position>>;
    expected_size = sizeof(ecstasy::Or<Velocity, Position>::Modifier);
    GTEST_ASSERT_EQ(computed_size, expected_size);

    // Must handle nested modifiers
    computed_size = ecstasy::modifier_allocator_size_v<ecstasy::Or<Velocity, ecstasy::Or<Density, Position>>>;
    expected_size = sizeof(ecstasy::Or<Velocity, ecstasy::Or<Density, Position>>::Modifier)
        + sizeof(ecstasy::Or<Density, Position>::Modifier);
    GTEST_ASSERT_EQ(computed_size, expected_size);

    // Same for multiple types
    computed_size = ecstasy::modifiers_allocator_size_v<Velocity, Position, Density>;
    GTEST_ASSERT_EQ(computed_size, 0);

    computed_size = ecstasy::modifiers_allocator_size_v<Density, ecstasy::Or<Velocity, Position>>;
    expected_size = sizeof(ecstasy::Or<Velocity, Position>::Modifier);
    GTEST_ASSERT_EQ(computed_size, expected_size);

    computed_size =
        ecstasy::modifiers_allocator_size_v<ecstasy::Maybe<Velocity>, Density, ecstasy::Or<Velocity, Position>>;
    expected_size = sizeof(ecstasy::Or<Velocity, Position>::Modifier) + sizeof(ecstasy::Maybe<Velocity>::Modifier);
    GTEST_ASSERT_EQ(computed_size, expected_size);

    computed_size = ecstasy::modifiers_allocator_size_v<ecstasy::Maybe<Velocity>, Density,
        ecstasy::Or<ecstasy::Not<Velocity>, Position>>;
    expected_size = sizeof(ecstasy::Or<ecstasy::Not<Velocity>, Position>::Modifier)
        + sizeof(ecstasy::Maybe<Velocity>::Modifier) + sizeof(ecstasy::Not<Velocity>::Modifier);
    GTEST_ASSERT_EQ(computed_size, expected_size);
}

TEST(Registry, stackAllocator)
{
    ecstasy::Registry registry;

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

    {
        auto query = registry.query<ecstasy::Maybe<Velocity>>();
    }

    {
        auto query = registry.select<ecstasy::Maybe<Position>>().where<ecstasy::Or<Velocity, Density>>();
    }

    {
        auto regquery = registry.select<ecstasy::Maybe<Density>>().where<ecstasy::And<Velocity, Position>>();

        size_t i = 0;
        size_t indexes[] = {0, 6, 8, 12};
        for (auto [density] : regquery) {
            size_t real_i = indexes[i];
            bool expect_density = (real_i == 12) || (real_i == 8) || (real_i == 0);
            GTEST_ASSERT_EQ(density.has_value(), expect_density);
            if (expect_density) {
                GTEST_ASSERT_EQ(density->get(), real_i * 4);
            }
            ++i;
        }
    }
}

TEST(Registry, RegistryStackQueryMemory)
{
    ecstasy::Registry registry;

    {
        // No modifier, no view because autolock set to false
        using NoAllocator = ecstasy::Registry::RegistryStackQueryMemory<
            util::meta::Traits<ecstasy::MapStorage<Position>, ecstasy::MapStorage<Velocity>>,
            util::meta::Traits<ecstasy::MapStorage<Position>, ecstasy::MapStorage<Velocity>>, util::meta::Traits<>,
            false>;

        assert_equals<NoAllocator::ModifiersAllocator, ecstasy::EmptyType>();
        assert_equals<NoAllocator::ViewsAllocator, ecstasy::EmptyType>();
        // assert_equals<std::integral_constant<size_t, sizeof(NoAllocator)>, std::integral_constant<size_t, 2>>();
        // static_assert(sizeof(NoAllocator) == sizeof(NoAllocator::ModifiersAllocatorReference));
    }

    {
        // Only modifier, no view because autolock set to false
        using OnlyModifiers = ecstasy::Registry::RegistryStackQueryMemory<util::meta::Traits<ecstasy::Maybe<Position>>,
            util::meta::Traits<ecstasy::Or<ecstasy::Maybe<Position>, Velocity>, ecstasy::Maybe<Position>>,
            util::meta::Traits<>, false>;
        constexpr size_t expected_size = sizeof(ecstasy::Maybe<Position>::Modifier)
            + sizeof(ecstasy::Or<ecstasy::Maybe<Position>, Velocity>::Modifier)
            + sizeof(ecstasy::Maybe<Position>::Modifier);
        using ExpectedModifierAlloc = util::StackAllocator<expected_size, ecstasy::query::modifier::ModifierBase>;

        assert_equals<OnlyModifiers::ModifiersAllocator, ExpectedModifierAlloc>();
        static_assert(OnlyModifiers::ModifiersAllocatorSize::value == expected_size);
        assert_equals<OnlyModifiers::ViewsAllocator, ecstasy::EmptyType>();
        static_assert(sizeof(OnlyModifiers) == sizeof(ExpectedModifierAlloc));
    }

    {
        // No modifier but views because autolock set to true and storages inherit from SharedRecursiveMutex (ie are
        // Lockable)
        using OnlyViews = ecstasy::Registry::RegistryStackQueryMemory<
            util::meta::Traits<ecstasy::MapStorage<Position>, ecstasy::MapStorage<Velocity>>,
            util::meta::Traits<ecstasy::MapStorage<Position>, ecstasy::MapStorage<Velocity>>, util::meta::Traits<>,
            true>;

        assert_equals<OnlyViews::ModifiersAllocator, ecstasy::EmptyType>();

#ifdef ECSTASY_LOCKABLE_STORAGES
        constexpr size_t expected_size = sizeof(ecstasy::thread::LockableView<ecstasy::MapStorage<Position>>)
            + sizeof(ecstasy::thread::LockableView<ecstasy::MapStorage<Velocity>>);
        using ExpectedViewAlloc = util::StackAllocator<expected_size, ecstasy::thread::LockableViewBase>;

        assert_equals<OnlyViews::ViewsAllocator, ExpectedViewAlloc>();
        static_assert(OnlyViews::ViewsAllocatorSize::value == expected_size);
        static_assert(sizeof(OnlyViews) == sizeof(ExpectedViewAlloc));
#else
        assert_equals<OnlyViews::ViewsAllocator, ecstasy::EmptyType>();
        static_assert(sizeof(OnlyViews) > 1 && sizeof(OnlyViews) < 4);
#endif
    }

    {
        // Modifiers and lockable views
        using ModifiersAndViews = ecstasy::Registry::RegistryStackQueryMemory<util::meta::Traits<Density>,
            util::meta::Traits<ecstasy::Or<ecstasy::Maybe<Position>, Velocity>, Density>, util::meta::Traits<>, true>;
        constexpr size_t expected_modifier_size = sizeof(ecstasy::Maybe<Position>::Modifier)
            + sizeof(ecstasy::Or<ecstasy::Maybe<Position>, Velocity>::Modifier);
        using ExpectedModifierAlloc =
            util::StackAllocator<expected_modifier_size, ecstasy::query::modifier::ModifierBase>;

        assert_equals<ModifiersAndViews::ModifiersAllocator, ExpectedModifierAlloc>();
        static_assert(ModifiersAndViews::ModifiersAllocatorSize::value == expected_modifier_size);
#ifdef ECSTASY_LOCKABLE_STORAGES
        constexpr size_t expected_view_size = sizeof(ecstasy::thread::LockableView<ecstasy::MapStorage<Density>>);
        using ExpectedViewAlloc = util::StackAllocator<expected_view_size, ecstasy::thread::LockableViewBase>;

        assert_equals<ModifiersAndViews::ViewsAllocator, ExpectedViewAlloc>();
        static_assert(ModifiersAndViews::ViewsAllocatorSize::value == expected_view_size);
        static_assert(sizeof(ModifiersAndViews) == sizeof(ExpectedModifierAlloc) + sizeof(ExpectedViewAlloc));
#else

        assert_equals<ModifiersAndViews::ViewsAllocator, ecstasy::EmptyType>();
        static_assert(sizeof(ModifiersAndViews) == sizeof(ExpectedModifierAlloc));
#endif
    }
}
