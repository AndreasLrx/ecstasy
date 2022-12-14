#include <gtest/gtest.h>
#include "util/serialization/toml/TomlObjectNode.hpp"

using namespace util::serialization;

TEST(TomlObjectNode, initialState)
{
    TomlObjectNode node;

    GTEST_ASSERT_TRUE(node.empty());
    GTEST_ASSERT_EQ(node.size(), 0);
    GTEST_ASSERT_EQ(node.getType(), INode::Type::Object);
}

TEST(TomlObjectNode, tomlTableInitialization)
{
    using namespace std::string_view_literals;

    toml::table table = toml::parse(R"(
        name = "toml++"
        authors = ["Mark Gillard <mark.gillard@outlook.com.au>"]
        cpp = 17
    )"sv);
    TomlObjectNode node(table);

    GTEST_ASSERT_EQ(node.size(), 3);
    GTEST_ASSERT_EQ(node.get("name").lock()->getType(), INode::Type::String);
    GTEST_ASSERT_EQ(node.get("authors").lock()->getType(), INode::Type::Array);
    GTEST_ASSERT_EQ(node.get("cpp").lock()->getType(), INode::Type::Integer);
}

TEST(TomlObjectNode, emptyGet)
{
    TomlObjectNode node;
    const TomlObjectNode &cnode = const_cast<const TomlObjectNode &>(node);

    EXPECT_THROW(node.get("key"), std::out_of_range);
    EXPECT_THROW(cnode.get("key"), std::out_of_range);
    GTEST_ASSERT_EQ(node.tryGet("key").use_count(), 0);
    GTEST_ASSERT_EQ(cnode.tryGet("key").use_count(), 0);
}

TEST(TomlObjectNode, Insert)
{
    TomlObjectNode node;
    const TomlObjectNode &cnode = const_cast<const TomlObjectNode &>(node);

    GTEST_ASSERT_TRUE(node.insert("key", TomlNode<toml::value<int64_t>>(42)));
    GTEST_ASSERT_EQ(node.size(), 1);
    GTEST_ASSERT_EQ(node.get("key").lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(cnode.get("key").lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(node.tryGet("key").lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(cnode.tryGet("key").lock()->asInteger(), 42);

    GTEST_ASSERT_FALSE(node.insert("key", TomlNode<toml::value<int64_t>>(420)));
    GTEST_ASSERT_EQ(node.size(), 1);
    GTEST_ASSERT_EQ(node.get("key").lock()->asInteger(), 42);
}

TEST(TomlObjectNode, InsertOrAssign)
{
    TomlObjectNode node;

    GTEST_ASSERT_TRUE(node.insertOrAssign("key", TomlNode<toml::value<int64_t>>(42)));
    GTEST_ASSERT_EQ(node.size(), 1);
    GTEST_ASSERT_EQ(node.get("key").lock()->asInteger(), 42);

    GTEST_ASSERT_FALSE(node.insertOrAssign("key", TomlNode<toml::value<int64_t>>(420)));
    GTEST_ASSERT_EQ(node.size(), 1);
    GTEST_ASSERT_EQ(node.get("key").lock()->asInteger(), 420);
}

TEST(TomlObjectNode, EraseContains)
{
    TomlObjectNode node;

    GTEST_ASSERT_TRUE(node.insert("keya", TomlNode<toml::value<int64_t>>(42)));
    GTEST_ASSERT_TRUE(node.insert("keyb", TomlNode<toml::value<int64_t>>(420)));
    GTEST_ASSERT_TRUE(node.insert("keyc", TomlNode<toml::value<int64_t>>(4)));
    GTEST_ASSERT_EQ(node.size(), 3);

    /// Does nothing
    node.erase("");
    GTEST_ASSERT_EQ(node.size(), 3);

    GTEST_ASSERT_EQ(node.get("keyb").lock()->asInteger(), 420);
    GTEST_ASSERT_TRUE(node.contains("keyb"));
    node.erase("keyb");
    GTEST_ASSERT_EQ(node.size(), 2);
    GTEST_ASSERT_FALSE(node.contains("keyb"));
}

TEST(TomlObjectNode, Clear)
{
    TomlObjectNode node;

    GTEST_ASSERT_TRUE(node.insert("keya", TomlNode<toml::value<int64_t>>(42)));
    GTEST_ASSERT_TRUE(node.insert("keyb", TomlNode<toml::value<int64_t>>(420)));
    GTEST_ASSERT_TRUE(node.insert("keyc", TomlNode<toml::value<int64_t>>(4)));
    GTEST_ASSERT_EQ(node.size(), 3);

    /// Does nothing
    node.clear();
    GTEST_ASSERT_EQ(node.size(), 0);
}

TEST(TomlObjectNode, Iterator)
{
    TomlObjectNode node;
    std::map<std::string, int> expect = {{"keya", 42}, {"keyb", 420}, {"keyc", 4}};

    GTEST_ASSERT_TRUE(node.insert("keya", TomlNode<toml::value<int64_t>>(42)));
    GTEST_ASSERT_TRUE(node.insert("keyb", TomlNode<toml::value<int64_t>>(420)));
    GTEST_ASSERT_TRUE(node.insert("keyc", TomlNode<toml::value<int64_t>>(4)));

    for (auto it : node)
        GTEST_ASSERT_EQ(it.second.lock()->asInteger(), expect[it.first]);
    for (auto it : const_cast<const TomlObjectNode &>(node))
        GTEST_ASSERT_EQ(it.second.lock()->asInteger(), expect[it.first]);
}
