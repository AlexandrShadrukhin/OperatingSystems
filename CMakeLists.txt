cmake_minimum_required(VERSION 3.29)
project(MonolithShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

get_filename_component(MONOLITH_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(MONOLITH_SOURCE_PATH "./source" ABSOLUTE)

set(MONOLITH_LIB_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/commands/Command.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/CdCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/CdCommand.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/ExitCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/ExitCommand.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/HelpCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/HelpCommand.h
)

set(MONOLITH_APP_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/Main.cpp
)

set(BENCHMARK_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/dedup_benchmark.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/search_name_benchmark.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/multi_benchmark.cpp
)

add_library(MonolithShellLib STATIC ${MONOLITH_LIB_SOURCES})
target_include_directories(MonolithShellLib PUBLIC ${MONOLITH_INCLUDE_PATH})

add_executable(Shell ${MONOLITH_APP_SOURCES})
add_executable(dedup_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/dedup_benchmark.cpp)
add_executable(search_name_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/search_name_benchmark.cpp)
add_executable(multi_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/multi_benchmark.cpp)

include_directories(/usr/include)

target_link_libraries(Shell PRIVATE MonolithShellLib readline)
target_link_libraries(dedup_benchmark-app PRIVATE MonolithShellLib)
target_link_libraries(search_name_benchmark-app PRIVATE MonolithShellLib)
target_link_libraries(multi_benchmark-app PRIVATE MonolithShellLib)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
endif()

add_custom_target(
        clang-format
        COMMAND clang-format -i -style=file ${MONOLITH_LIB_SOURCES} ${MONOLITH_APP_SOURCES} ${BENCHMARK_SOURCES}
        COMMENT "Running clang-format on source files"
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(benchmark_tests tests/benchmark_tests.cpp)
target_link_libraries(benchmark_tests gtest_main)

add_test(NAME DedupBenchmarkTest COMMAND benchmark_tests --gtest_filter=DedupBenchmarkTest.*)
add_test(NAME SearchNameBenchmarkTest COMMAND benchmark_tests --gtest_filter=SearchNameBenchmarkTest.*)
set_tests_properties(SearchNameBenchmarkTest PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)