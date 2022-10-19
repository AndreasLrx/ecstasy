#include <gtest/gtest.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resource/Resource.hpp"
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

    /// Resource not present
    EXPECT_THROW(registry.getResource<Counter>(), std::logic_error);

    /// Add resource with an initial value of 5 and add one
    registry.addResource<Counter>(5).count();
    EXPECT_EQ(registry.getResource<Counter>().value, 6);

    /// Try to add resource already present
    EXPECT_THROW(registry.addResource<Counter>(), std::logic_error);
}

TEST(Registry, storages)
{
    ecstasy::Registry registry;

    /// Resource not present
    EXPECT_THROW(registry.getStorage<Counter>(), std::logic_error);
    EXPECT_THROW(registry.getStorage<A>(), std::logic_error);
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);
    EXPECT_EQ(registry.getStorageSafe<A>().size(), 0);

    /// Add resource with an initial value of 5 and add one
    registry.addStorage<Counter>();
    EXPECT_EQ(registry.getStorage<Counter>().size(), 0);
}