#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <string>
#include <filesystem>
#include <thread>
// ./multi_benchmark 1000 100000 10 /Users/AlexandrShadrukhin/Downloads gogol.txt
namespace fs = std::filesystem;

// Функция для поиска файла
void searchName(const fs::path& directory, const std::string& filename, int iterations) {
  for (int i = 0; i < iterations; ++i) {
    try {
      for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.path().filename() == filename) {
          std::cout << "Найдено: " << entry.path() << std::endl;
        }
      }
    } catch (const std::exception& e) {
      std::cerr << "Ошибка: " << e.what() << std::endl;
    }
  }
}

// Функция для дедупликации
void dedup(std::vector<std::string>& data, int iterations) {
  std::unordered_set<std::string> unique_elements;
  std::vector<std::string> deduplicated;

  for (int i = 0; i < iterations; ++i) {
    for (const auto& elem : data) {
      if (unique_elements.insert(elem).second) {
        deduplicated.push_back(elem);
      }
    }
    unique_elements.clear();
    deduplicated.clear();
  }
}

int main(int argc, char* argv[]) {
  if (argc < 6) {
    std::cerr << "Введите: " << argv[0]
              << " <количество итераций дедупликации> <размер массива> <количество итераций поиска> <путь> <имя файла>"
              << std::endl;
    return 1;
  }

  int dedup_iterations = std::stoi(argv[1]);
  int array_size = std::stoi(argv[2]);

  int search_iterations = std::stoi(argv[3]);
  fs::path directory = argv[4];
  std::string filename = argv[5];

  std::vector<std::string> data;
  for (int i = 0; i < array_size; ++i) {
    data.push_back("элемент" + std::to_string(i % 10));
  }

  auto dedup_start = std::chrono::high_resolution_clock::now();
  std::thread dedup_thread([&]() { dedup(data, dedup_iterations); });
  auto dedup_end = std::chrono::high_resolution_clock::now();

  auto search_start = std::chrono::high_resolution_clock::now();
  std::thread search_thread([&]() { searchName(directory, filename, search_iterations); });
  auto search_end = std::chrono::high_resolution_clock::now();

  dedup_thread.join();
  search_thread.join();

  std::chrono::duration<double> dedup_elapsed = dedup_end - dedup_start;
  std::chrono::duration<double> search_elapsed = search_end - search_start;

  std::cout << "Дедупликация: выполнено за " << dedup_iterations << " итераций и: " << dedup_elapsed.count() << " с"
            << std::endl;
  std::cout << "Поиск файла: выполнено за " << search_iterations << " итераций и: " << search_elapsed.count() << " с"
            << std::endl;

  return 0;
}
