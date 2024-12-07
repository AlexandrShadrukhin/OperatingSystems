cmake_minimum_required(VERSION 3.29)

# Указываем путь к основному проекту
get_filename_component(PROJECT_SOURCE_DIR "../" ABSOLUTE)

# Включаем GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

# Флаги для тестов
set(TEST_SOURCES
        test_dedup_benchmark.cpp
        test_search_name_benchmark.cpp
        test_main.cpp
)

# Создаем цель для тестов
add_executable(MonolithShellTests ${TEST_SOURCES})

# Добавляем пути для include
target_include_directories(MonolithShellTests PRIVATE
        ${PROJECT_SOURCE_DIR}/source/monolith/commands
        ${PROJECT_SOURCE_DIR}/source/monolith/benchmarks
)

# Компилируем тесты с TESTING_MODE
target_compile_definitions(MonolithShellTests PRIVATE TESTING_MODE)

# Линкуем библиотеки
target_link_libraries(MonolithShellTests PRIVATE
        gtest
        gtest_main
        pthread
)

# Добавляем тесты в CTest
enable_testing()
add_test(NAME MonolithShellTests COMMAND MonolithShellTests)