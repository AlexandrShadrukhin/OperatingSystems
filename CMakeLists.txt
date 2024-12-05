cmake_minimum_required(VERSION 3.29)
project(MonolithShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам и тестам
get_filename_component(MONOLITH_INCLUDE_PATH "." ABSOLUTE)
get_filename_component(MONOLITH_SOURCE_PATH "./source" ABSOLUTE)
get_filename_component(TEST_PATH "./test" ABSOLUTE)

# Исходники библиотеки
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

# Приложение
set(MONOLITH_APP_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/Main.cpp
)

# Бенчмарки
set(BENCHMARK_SOURCES
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/dedup_benchmark.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/search_name_benchmark.cpp
        ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/multi_benchmark.cpp
)

# Тесты
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${TEST_PATH}/*.cpp)

# Создаем библиотеку
add_library(MonolithShellLib STATIC ${MONOLITH_LIB_SOURCES})
target_include_directories(MonolithShellLib PUBLIC ${MONOLITH_INCLUDE_PATH})

# Приложение
add_executable(Shell ${MONOLITH_APP_SOURCES})
target_link_libraries(Shell PRIVATE MonolithShellLib readline)

# Бенчмарки
add_executable(dedup_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/dedup_benchmark.cpp)
add_executable(search_name_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/search_name_benchmark.cpp)
add_executable(multi_benchmark-app ${MONOLITH_SOURCE_PATH}/monolith/benchmarks/multi_benchmark.cpp)

target_link_libraries(dedup_benchmark-app PRIVATE MonolithShellLib)
target_link_libraries(search_name_benchmark-app PRIVATE MonolithShellLib)
target_link_libraries(multi_benchmark-app PRIVATE MonolithShellLib)

# Подключение Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(MonolithShellTests ${TEST_SOURCES})

# Настройки для тестов
target_compile_definitions(MonolithShellTests PRIVATE TESTING_MODE) # Определяем TESTING_MODE для тестов
target_include_directories(MonolithShellTests PRIVATE ${MONOLITH_INCLUDE_PATH} ${TEST_PATH})
target_include_directories(MonolithShellTests PRIVATE /opt/homebrew/opt/readline/include)
target_link_directories(MonolithShellTests PRIVATE /opt/homebrew/opt/readline/lib)

# Линковка тестов
target_link_libraries(
        MonolithShellTests PRIVATE
        MonolithShellLib # Основная библиотека
        gtest # Google Test
        gtest_main # Main для Google Test
        pthread # Поддержка потоков
        readline # Подключаем readline
)

# Регистрируем тесты
add_test(NAME MonolithShellTests COMMAND MonolithShellTests)