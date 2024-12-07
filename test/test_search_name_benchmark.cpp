#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include "../source/monolith/benchmarks/search_name_benchmark.cpp" // Подключаем вашу реализацию

namespace fs = std::filesystem;

// Вспомогательная функция для создания временного файла
void createFile(const fs::path &path) {
    std::ofstream file(path);
    file << "Тестовый контент";
    file.close();
}

// Тест: файл найден в каталоге
TEST(SearchNameTest, FileFound) {
    fs::path tempDir = fs::temp_directory_path() / "test_directory";
    fs::create_directory(tempDir);
    fs::path tempFile = tempDir / "test_file.txt";
    createFile(tempFile);

    testing::internal::CaptureStdout(); // Перехватываем вывод
    searchName(tempDir, "test_file.txt");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Найдено:") != std::string::npos);
    EXPECT_TRUE(output.find(tempFile.string()) != std::string::npos);

    fs::remove_all(tempDir); // Удаляем временные файлы
}

// Тест: файл не найден
TEST(SearchNameTest, FileNotFound) {
    fs::path tempDir = fs::temp_directory_path() / "test_directory";
    fs::create_directory(tempDir);

    testing::internal::CaptureStdout(); // Перехватываем вывод
    searchName(tempDir, "non_existent_file.txt");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.empty()); // Ожидаем пустой вывод, так как файл не найден

    fs::remove_all(tempDir); // Удаляем временные файлы
}

// Тест: вложенные каталоги
TEST(SearchNameTest, NestedDirectories) {
    fs::path tempDir = fs::temp_directory_path() / "test_directory";
    fs::path nestedDir = tempDir / "nested_directory";
    fs::create_directories(nestedDir);

    fs::path tempFile = nestedDir / "nested_file.txt";
    createFile(tempFile);

    testing::internal::CaptureStdout(); // Перехватываем вывод
    searchName(tempDir, "nested_file.txt");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Найдено:") != std::string::npos);
    EXPECT_TRUE(output.find(tempFile.string()) != std::string::npos);

    fs::remove_all(tempDir); // Удаляем временные файлы
}

// Тест: исключение при недоступном каталоге
TEST(SearchNameTest, DirectoryNotAccessible) {
    fs::path nonExistentDir = "/path/to/non_existent_directory";

    testing::internal::CaptureStderr(); // Перехватываем вывод ошибок
    searchName(nonExistentDir, "any_file.txt");
    std::string errorOutput = testing::internal::GetCapturedStderr();

    EXPECT_FALSE(errorOutput.empty()); // Ожидаем сообщение об ошибке
    EXPECT_TRUE(errorOutput.find("Ошибка:") != std::string::npos);
}
