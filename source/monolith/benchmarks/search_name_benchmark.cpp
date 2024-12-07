#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
// ./search_name_benchmark 2 /Users/AlexandrShadrukhin/Downloads gogol.txt
namespace fs = std::filesystem;

bool searchName(const fs::path& directory, const std::string& filename) {
    bool found = false;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (entry.path().filename() == filename) {
                std::cout << "Найдено: " << entry.path() << std::endl;
                found = true;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return false;
    }

    return found;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Введите: " << argv[0] << " <количество итераций> <путь> <имя файла>" << std::endl;
        return 1;
    }

    int iterations = std::stoi(argv[1]);
    fs::path directory = argv[2];
    std::string filename = argv[3];

    auto start = std::chrono::high_resolution_clock::now();

    bool found = false;

    for (int i = 0; i < iterations; ++i) {
        if (searchName(directory, filename)) {
            found = true;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (!found) {
        std::cerr << "Ошибка: Файл \"" << filename << "\" не найден в директории \"" << directory << "\"." << std::endl;
    } else {
        std::cout << "Файл успешно найден." << std::endl;
    }

    std::cout << "Выполнено за " << iterations << " итераций и: " << elapsed.count() << " с" << std::endl;

    return found ? 0 : 1;
}