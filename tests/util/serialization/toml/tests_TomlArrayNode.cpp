#include <gtest/gtest.h>
#include "util/serialization/toml/TomlArrayNode.hpp"

using namespace util::serialization;

TEST(TomlArrayNode, InitialState)
{
    TomlArrayNode node;

    GTEST_ASSERT_TRUE(node.empty());
    GTEST_ASSERT_EQ(node.size(), 0);
    GTEST_ASSERT_EQ(node.getType(), INode::Type::Array);
}

TEST(TomlArrayNode, tomlArrayInitialisation)
{
    using namespace std::string_view_literals;

    toml::table table = toml::parse(R"(array = [1, 2, 3, 5, 9])"sv);
    TomlArrayNode node(*table.get_as<toml::array>("array"));

    GTEST_ASSERT_EQ(node.size(), 5);
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 1);
    GTEST_ASSERT_EQ(node.get(1).lock()->asInteger(), 2);
    GTEST_ASSERT_EQ(node.get(2).lock()->asInteger(), 3);
    GTEST_ASSERT_EQ(node.get(3).lock()->asInteger(), 5);
    GTEST_ASSERT_EQ(node.get(4).lock()->asInteger(), 9);
}

TEST(TomlArrayNode, EmptyGet)
{
    TomlArrayNode node;
    const TomlArrayNode &cnode = const_cast<const TomlArrayNode &>(node);

    EXPECT_THROW(node.get(0), std::out_of_range);
    EXPECT_THROW(cnode.get(0), std::out_of_range);
    GTEST_ASSERT_EQ(node.tryGet(0).use_count(), 0);
    GTEST_ASSERT_EQ(cnode.tryGet(0).use_count(), 0);
}

TEST(TomlArrayNode, InsertOutOfBounds)
{
    TomlArrayNode node;

    ASSERT_THROW(node.insert(1, TomlNode<toml::value<int64_t>>(42)), std::out_of_range);
}

TEST(TomlArrayNode, InsertEnd)
{
    TomlArrayNode node;

    node.insert(0, TomlNode<toml::value<int64_t>>(42));
    GTEST_ASSERT_FALSE(node.empty());
    GTEST_ASSERT_EQ(node.size(), 1);
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(node.tryGet(0).lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(const_cast<const TomlArrayNode &>(node).tryGet(0).lock()->asInteger(), 42);
}

TEST(TomlArrayNode, InsertStart)
{
    TomlArrayNode node;

    node.insert(0, TomlNode<toml::value<int64_t>>(42));
    node.insert(0, TomlNode<toml::value<int64_t>>(420));

    GTEST_ASSERT_EQ(node.size(), 2);
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 420);
    GTEST_ASSERT_EQ(node.get(1).lock()->asInteger(), 42);
}

TEST(TomlArrayNode, InsertRandom)
{
    TomlArrayNode node;

    /// [1]
    node.insert(0, TomlNode<toml::value<int64_t>>(1));
    /// [2, 1]
    node.insert(0, TomlNode<toml::value<int64_t>>(2));
    /// [2, 3, 1]
    node.insert(1, TomlNode<toml::value<int64_t>>(3));
    /// [2, 3, 4, 1]
    node.insert(2, TomlNode<toml::value<int64_t>>(4));
    /// [2, 5, 3, 4, 1]
    node.insert(1, TomlNode<toml::value<int64_t>>(5));

    GTEST_ASSERT_EQ(node.size(), 5);
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 2);
    GTEST_ASSERT_EQ(node.get(1).lock()->asInteger(), 5);
    GTEST_ASSERT_EQ(node.get(2).lock()->asInteger(), 3);
    GTEST_ASSERT_EQ(node.get(3).lock()->asInteger(), 4);
    GTEST_ASSERT_EQ(node.get(4).lock()->asInteger(), 1);
}

TEST(TomlArrayNode, PushBack)
{
    TomlArrayNode node;

    node.pushBack(TomlNode<toml::value<int64_t>>(1));
    node.pushBack(TomlNode<toml::value<int64_t>>(2));
    node.pushBack(TomlNode<toml::value<int64_t>>(3));

    GTEST_ASSERT_EQ(node.size(), 3);
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 1);
    GTEST_ASSERT_EQ(node.get(1).lock()->asInteger(), 2);
    GTEST_ASSERT_EQ(node.get(2).lock()->asInteger(), 3);
}

TEST(TomlArrayNode, Replace)
{
    TomlArrayNode node;

    for (int i = 0; i < 10; i++)
        node.pushBack(TomlNode<toml::value<int64_t>>(i));

    GTEST_ASSERT_EQ(node.size(), 10);
    for (int i = 0; i < 10; i++)
        GTEST_ASSERT_EQ(node.get(i).lock()->asInteger(), i);

    EXPECT_THROW(node.replace(15, TomlNode<toml::value<int64_t>>(15)), std::out_of_range);

    node.replace(2, TomlNode<toml::value<int64_t>>(42));
    node.replace(6, TomlNode<toml::value<int64_t>>(84));
    GTEST_ASSERT_EQ(node.get(2).lock()->asInteger(), 42);
    GTEST_ASSERT_EQ(node.get(6).lock()->asInteger(), 84);
}

TEST(TomlArrayNode, PopBack)
{
    TomlArrayNode node;

    for (int i = 0; i < 10; i++)
        node.pushBack(TomlNode<toml::value<int64_t>>(i));

    GTEST_ASSERT_EQ(node.size(), 10);
    node.popBack();
    node.popBack();
    GTEST_ASSERT_EQ(node.size(), 8);
}

TEST(TomlArrayNode, Erase)
{
    TomlArrayNode node;

    for (int i = 0; i < 10; i++)
        node.pushBack(TomlNode<toml::value<int64_t>>(i));

    GTEST_ASSERT_EQ(node.size(), 10);
    node.erase(0);
    node.erase(5);
    /// [1, 2, 3, 4, 5, 7, 8, 9]
    GTEST_ASSERT_EQ(node.get(0).lock()->asInteger(), 1);
    GTEST_ASSERT_EQ(node.get(4).lock()->asInteger(), 5);
    GTEST_ASSERT_EQ(node.get(5).lock()->asInteger(), 7);
    GTEST_ASSERT_EQ(node.size(), 8);
    // no effect
    node.erase(10);
    GTEST_ASSERT_EQ(node.size(), 8);
}

TEST(TomlArrayNode, Clear)
{
    TomlArrayNode node;

    for (int i = 0; i < 10; i++)
        node.pushBack(TomlNode<toml::value<int64_t>>(i));

    GTEST_ASSERT_EQ(node.size(), 10);
    node.clear();
    GTEST_ASSERT_EQ(node.size(), 0);
}

TEST(TomlArrayNode, Iterator)
{
    TomlArrayNode node;

    for (int i = 0; i < 10; i++)
        node.pushBack(TomlNode<toml::value<int64_t>>(i));

    auto iter = node.begin();
    GTEST_ASSERT_EQ((*iter).lock()->asInteger(), 0);
    auto iter2 = iter++;
    GTEST_ASSERT_EQ((*iter2).lock()->asInteger(), 1);

    size_t i = 0;
    for (auto it : node)
        GTEST_ASSERT_EQ(it.lock()->asInteger(), i++);
    i = 0;
    for (const auto it : const_cast<const TomlArrayNode &>(node))
        GTEST_ASSERT_EQ(it.lock()->asInteger(), i++);
}