#include <gtest/gtest.h>
#include <unordered_map>

#include "ecstasy/rtti/Type.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

#include "ecstasy/rtti/TypeRegistry.hpp"

using namespace ecstasy::rtti;

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Size {
    float width;
    float height;
};

// Size is not registered
REGISTER_TYPES(Position, Velocity);

TEST(TypeRegistry, all)
{
    TypeRegistry &registry = TypeRegistry::getInstance();
    AType &position = registry.get("Position");
    auto unregistered_position = Type<Position>("Position");
    auto unregistered_size = Type<Size>("Size");

    /// has checks
    // Successfull has
    ASSERT_TRUE(registry.has("Position"));
    ASSERT_TRUE(registry.has(position.getHash()));
    ASSERT_TRUE(registry.has(typeid(Position)));
    ASSERT_TRUE(registry.has<Position>());
    // Unsuccessfull has
    ASSERT_FALSE(registry.has("Size"));
    ASSERT_FALSE(registry.has(unregistered_size.getHash()));
    ASSERT_FALSE(registry.has(typeid(Size)));
    ASSERT_FALSE(registry.has<Size>());

    // Successfull getters
    ASSERT_EQ(registry.get(position.getHash()), position);
    ASSERT_EQ(registry.find(position.getHash())->get(), position);
    ASSERT_EQ(registry.get(typeid(Position)), position);
    ASSERT_EQ(registry.find(typeid(Position))->get(), position);
    ASSERT_EQ(registry.get<Position>(), position);
    ASSERT_EQ(registry.find<Position>()->get(), position);
    ASSERT_EQ(registry.get("Position"), position);
    ASSERT_EQ(registry.find("Position")->get(), position);
    ASSERT_EQ(registry.get(unregistered_position), position);
    ASSERT_EQ(registry.find(unregistered_position)->get(), position);
    ASSERT_EQ(registry.getIf([](const auto &pair) {
        return *pair.second == std::string_view("Position");
    }),
        position);
    ASSERT_EQ(registry
                  .findIf([](const auto &pair) {
                      return *pair.second == std::string_view("Position");
                  })
                  ->get(),
        position);

    // Unsuccessfull getters
    ASSERT_THROW(static_cast<void>(registry.get(unregistered_size.getHash())), std::out_of_range);
    ASSERT_EQ(registry.find(unregistered_size.getHash()), std::nullopt);
    ASSERT_THROW(static_cast<void>(registry.get(typeid(Size))), std::out_of_range);
    ASSERT_EQ(registry.find(typeid(Size)), std::nullopt);
    ASSERT_THROW(static_cast<void>(registry.get<Size>()), std::out_of_range);
    ASSERT_EQ(registry.find<Size>(), std::nullopt);
    ASSERT_THROW(static_cast<void>(registry.get("Size")), std::out_of_range);
    ASSERT_EQ(registry.find("Size"), std::nullopt);
    ASSERT_THROW(static_cast<void>(registry.get(unregistered_size)), std::out_of_range);
    ASSERT_EQ(registry.find(unregistered_size), std::nullopt);
    ASSERT_THROW(static_cast<void>(registry.getIf([](const auto &pair) {
        return *pair.second == std::string_view("Size");
    })),
        std::out_of_range);
    ASSERT_EQ(registry.findIf([](const auto &pair) {
        return *pair.second == std::string_view("Size");
    }),
        std::nullopt);

    // Register without using the macro and retry the has/get/find tests
    AType &size = registry.registerType<Size>("Size");
    ASSERT_TRUE(registry.has("Size"));
    ASSERT_TRUE(registry.has(unregistered_size.getHash()));
    ASSERT_TRUE(registry.has(typeid(Size)));
    ASSERT_TRUE(registry.has<Size>());

    ASSERT_EQ(registry.get(unregistered_size.getHash()), size);
    ASSERT_EQ(registry.find(unregistered_size.getHash()), size);
    ASSERT_EQ(registry.get(typeid(Size)), size);
    ASSERT_EQ(registry.find(typeid(Size)), size);
    ASSERT_EQ(registry.get<Size>(), size);
    ASSERT_EQ(registry.find<Size>()->get(), size);
    ASSERT_EQ(registry.get("Size"), size);
    ASSERT_EQ(registry.find("Size"), size);
    ASSERT_EQ(registry.get(unregistered_size), size);
    ASSERT_EQ(registry.find(unregistered_size)->get(), size);
}
