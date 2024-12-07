#include <gtest/gtest.h>
#include <cstdlib>
#include <filesystem>
#include <string>

TEST(DedupBenchmarkTest, RunsSuccessfully) {
    int ret = std::system("../dedup_benchmark-app 100 10000");
    EXPECT_EQ(ret, 0) << "dedup_benchmark-app не завершился успешно";
}

TEST(SearchNameBenchmarkTest, RunsSuccessfully) {
    std::ostringstream commandStream;
    commandStream << "../search_name_benchmark-app 2 ./ CMakeCache.txt";
    std::string command = commandStream.str();
    int ret = std::system(command.c_str());
    EXPECT_EQ(ret, 0) << "search_name_benchmark-app не завершился успешно";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}