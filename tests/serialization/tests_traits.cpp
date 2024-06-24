#include <gtest/gtest.h>
#include "ecstasy/registry/Registry.hpp"
#include "ecstasy/resources/entity/RegistryEntity.hpp"
#include "ecstasy/serialization/RawSerializer.hpp"
#include "ecstasy/serialization/Serializer.hpp"
#include "ecstasy/storages/MapStorage.hpp"

using namespace ecstasy::serialization;

struct Position {
  public:
    float x;
    float y;

    Position(float px = 0.f, float py = 0.f) : x(px), y(py)
    {
    }

    Position(RawSerializer &serializer) : x(serializer.load<float>()), y(serializer.load<float>())
    {
    }

    RawSerializer &operator>>(RawSerializer &serializer) const
    {
        return serializer.appendRaw(*this);
    }

    Position &operator<<(RawSerializer &serializer)
    {
        serializer >> x >> y;
        return *this;
    }
};

struct NonSerializable {
  public:
    float x;
    float y;

    NonSerializable(float px = 0.f, float py = 0.f) : x(px), y(py)
    {
    }
};

TEST(can_save_type, all)
{
    static_assert(concepts::can_save_type_v<RawSerializer, int>, "False negative on int");
    static_assert(concepts::can_save_type_v<RawSerializer, Position>, "False negative on Position component");
    static_assert(
        !concepts::can_save_type_v<RawSerializer, NonSerializable>, "False positive on NonSerializable component");
}
