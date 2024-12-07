#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../source/monolith/commands/utility.h"

TEST(ParseStrTest, SingleTest) {
    std::string input = "hello world";
    std::vector<std::string> expected = {"hello", "world"};
    EXPECT_EQ(monolith::app::parseStr(input), expected);
}