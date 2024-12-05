cmake_minimum_required(VERSION 3.29)
project(MonolithShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(MONOLITH_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(MONOLITH_SOURCE_PATH "./source" ABSOLUTE)
get_filename_component(TEST_PATH "./test" ABSOLUTE)

set(MONOLITH_LIB_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/commands/Command.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/CdCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/CdCommand.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/ExitCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/ExitCommand.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/HelpCommand.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/HelpCommand.h
        ${MONOLITH_SOURCE_PATH}/monolith/commands/utility.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/commands/utility.h
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
target_link_libraries(Shell PRIVATE MonolithShellLib readline)

add_executable(dedup_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/dedup_benchmark.cpp)
add_executable(search_name_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/search_name_benchmark.cpp)
add_executable(multi_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/multi_benchmark.cpp)

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

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${TEST_PATH}/*.cpp)

add_executable(MonolithShellTests ${TEST_SOURCES})

target_include_directories(MonolithShellTests PRIVATE ${MONOLITH_INCLUDE_PATH} ${TEST_PATH})
target_link_directories(MonolithShellTests PRIVATE /opt/homebrew/opt/readline/lib)
target_include_directories(MonolithShellTests PRIVATE /opt/homebrew/opt/readline/include)

target_compile_definitions(MonolithShellTests PRIVATE TESTING_MODE)

target_link_libraries(
        MonolithShellTests PRIVATE
        MonolithShellLib # Ваша библиотека
        gtest # Google Test
        gtest_main # Main для Google Test
        pthread # Для многопоточности
        readline # Подключаем readline
)

add_test(NAME MonolithShellTests COMMAND MonolithShellTests)