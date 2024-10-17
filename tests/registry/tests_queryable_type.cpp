#include <gtest/gtest.h>
#include "ecstasy/config.hpp"
#include "ecstasy/query/modifiers/Maybe.hpp"
#include "ecstasy/registry/modifiers/Maybe.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/MapStorage.hpp"
#include "ecstasy/registry/concepts/queryable_type.hpp"

using namespace ecstasy;

template <typename T>
class WrapperImpl {
  public:
    using WrappedType = T;

    WrapperImpl(T &data) : _data(data)
    {
    }

    T *operator->() const
    {
        return &_data;
    }

  private:
    T &_data;
};

/// Debugging helpers, this print types T1 and T2 in the compilation error message
template <typename T1, typename T2>
void assert_equals()
{
    static_assert(std::is_same_v<T1, T2>, "Types T1 and T2 differs.");
}

struct Position {
    float x;
    float y;
};

TEST(queryable_type, Components)
{
    assert_equals<queryable_type_t<Position>, getStorageType<Position>>();
    assert_equals<getStorageType<Position>, MapStorage<Position>>();
    assert_equals<queryable_type_t<Position>, MapStorage<Position>>();
    assert_equals<getStorageType<const Position>, const MapStorage<Position>>();
    assert_equals<queryable_type_t<const Position>, getStorageType<const Position>>();
    assert_equals<queryable_type_t<const Position>, const MapStorage<Position>>();
}

TEST(queryable_type, Resources)
{
    assert_equals<queryable_type_t<ecstasy::Entities>, ecstasy::Entities>();
    assert_equals<queryable_type_t<const ecstasy::Entities>, const ecstasy::Entities>();
}

TEST(queryable_type, Storage)
{
    assert_equals<queryable_type_t<MapStorage<Position>>, MapStorage<Position>>();
    assert_equals<queryable_type_t<const MapStorage<Position>>, const MapStorage<Position>>();
}

TEST(queryable_type, Modifier)
{
    assert_equals<queryable_type_t<query::modifier::Maybe<MapStorage<Position>>>,
        query::modifier::Maybe<MapStorage<Position>>>();
}

TEST(queryable_type, RegistryModifier)
{
    assert_equals<queryable_type_t<Maybe<Position>>,
        query::modifier::Maybe<MapStorage<Position>, thread::AUTO_LOCK_DEFAULT>>();
}
