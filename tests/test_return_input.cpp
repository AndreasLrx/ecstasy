#include <gtest/gtest.h>

int return_input(int input);

TEST(loadConfigFile, unexisting_file)
{
    EXPECT_EQ(return_input(5), 5);
}