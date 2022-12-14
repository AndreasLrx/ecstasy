#include <gtest/gtest.h>

#include "util/serialization/toml/TomlArrayNode.hpp"
#include "util/serialization/toml/TomlNode.hpp"
#include "util/serialization/toml/TomlObjectNode.hpp"

using namespace util::serialization;

TEST(TomlNode, Empty)
{
    TomlNode<toml::value<int64_t>> a;

    GTEST_ASSERT_FALSE(a.isNull());
    GTEST_ASSERT_TRUE(a.isInteger());
    GTEST_ASSERT_TRUE(a.tryAsInteger());
    GTEST_ASSERT_EQ(a.asInteger(), 0);
}

TEST(TomlNode, Copy)
{
    auto a = TomlNode(toml::value<std::string>("Hello world!"));

    GTEST_ASSERT_TRUE(a.isString());
    GTEST_ASSERT_TRUE(a.tryAsString());
    GTEST_ASSERT_EQ(a.asString(), "Hello world!");
    GTEST_ASSERT_FALSE(a.tryAsInteger());
    EXPECT_THROW(a.asInteger(), std::bad_optional_access);
}

TEST(TomlNode, ObjectNode)
{
    TomlObjectNode a{toml::table()};
    const auto &ca = const_cast<const TomlObjectNode &>(a);

    GTEST_ASSERT_TRUE(a.isObject());
    GTEST_ASSERT_TRUE(a.tryAsObject());
    GTEST_ASSERT_TRUE(const_cast<const TomlObjectNode &>(a).tryAsObject());

    GTEST_ASSERT_FALSE(a.tryAsArray());
    EXPECT_THROW(a.asArray(), std::runtime_error);
    GTEST_ASSERT_FALSE(ca.tryAsArray());
    EXPECT_THROW(ca.asArray(), std::runtime_error);
}

TEST(TomlNode, ArrayNode)
{
    TomlArrayNode a(toml::array(1, 2, 3));

    GTEST_ASSERT_TRUE(a.isArray());
    GTEST_ASSERT_TRUE(a.tryAsArray());
    GTEST_ASSERT_TRUE(const_cast<const TomlArrayNode &>(a).tryAsArray());
    GTEST_ASSERT_FALSE(a.tryAsString());
    EXPECT_THROW(a.asString(), std::bad_optional_access);
}

TEST(TomlNode, StringView)
{
    TomlNode<toml::value<std::string>> a(std::string_view("Hello world!"));

    GTEST_ASSERT_TRUE(a.isString());
    GTEST_ASSERT_TRUE(a.tryAsString());
    GTEST_ASSERT_EQ(a.asString(), "Hello world!");
    GTEST_ASSERT_FALSE(a.tryAsInteger());
    EXPECT_THROW(a.asInteger(), std::bad_optional_access);
}

TEST(TomlNode, String)
{
    TomlNode<toml::value<std::string>> a;
    /// Scope to be sure the string is stored in the node and is not just a reference to the str variable
    {
        std::string str = "Hello world!";
        a = TomlNode<toml::value<std::string>>(str);
    }

    GTEST_ASSERT_TRUE(a.isString());
    GTEST_ASSERT_TRUE(a.tryAsString());
    GTEST_ASSERT_EQ(a.asString(), "Hello world!");
    GTEST_ASSERT_FALSE(a.tryAsInteger());
    EXPECT_THROW(a.asInteger(), std::bad_optional_access);
}

TEST(TomlNode, Integer)
{
    TomlNode<toml::value<int64_t>> a(42l);

    GTEST_ASSERT_TRUE(a.isInteger());
    GTEST_ASSERT_TRUE(a.tryAsInteger());
    GTEST_ASSERT_EQ(a.asInteger(), 42);
    GTEST_ASSERT_FALSE(a.tryAsFloat());
    EXPECT_THROW(a.asFloat(), std::bad_optional_access);
}

TEST(TomlNode, Float)
{
    TomlNode<toml::value<double>> a(42.0);

    GTEST_ASSERT_TRUE(a.isFloat());
    GTEST_ASSERT_TRUE(a.tryAsFloat());
    GTEST_ASSERT_EQ(a.asFloat(), 42.0);
    GTEST_ASSERT_FALSE(a.tryAsBoolean());
    EXPECT_THROW(a.asBoolean(), std::bad_optional_access);
}

TEST(TomlNode, Boolean)
{
    TomlNode<toml::value<bool>> a(true);

    GTEST_ASSERT_TRUE(a.isBoolean());
    GTEST_ASSERT_TRUE(a.tryAsBoolean());
    GTEST_ASSERT_TRUE(a.asBoolean());
    GTEST_ASSERT_FALSE(a.tryAsDate());
    EXPECT_THROW(a.asDate(), std::bad_optional_access);
}

TEST(TomlNode, Date)
{
    INode::Date date(std::chrono::year(1), std::chrono::month(2), std::chrono::day(3));
    TomlNode<toml::value<toml::date>> a(TomlConversion::toToml(date));

    GTEST_ASSERT_TRUE(a.isDate());
    GTEST_ASSERT_TRUE(a.tryAsDate());
    GTEST_ASSERT_EQ(a.asDate(), date);
    GTEST_ASSERT_FALSE(a.tryAsTime());
    EXPECT_THROW(a.asTime(), std::bad_optional_access);
}

TEST(TomlNode, Time)
{
    INode::Time time(42);
    TomlNode<toml::value<toml::time>> a(TomlConversion::toToml(time));

    GTEST_ASSERT_TRUE(a.isTime());
    GTEST_ASSERT_TRUE(a.tryAsTime());
    GTEST_ASSERT_EQ(a.asTime(), time);
    GTEST_ASSERT_FALSE(a.tryAsDateTime());
    EXPECT_THROW(a.asDateTime(), std::bad_optional_access);
}

TEST(TomlNode, DateTime)
{
    INode::DateTime tp = std::chrono::system_clock::now();
    TomlNode<toml::value<toml::date_time>> a(TomlConversion::toToml(tp));
    auto &ca = const_cast<const TomlNode<toml::value<toml::date_time>> &>(a);

    GTEST_ASSERT_TRUE(a.isDateTime());
    GTEST_ASSERT_TRUE(a.tryAsDateTime());
    GTEST_ASSERT_EQ(a.asDateTime(), tp);
    GTEST_ASSERT_FALSE(a.tryAsObject());
    EXPECT_THROW(a.asObject(), std::runtime_error);
    GTEST_ASSERT_FALSE(ca.tryAsObject());
    EXPECT_THROW(ca.asObject(), std::runtime_error);
}