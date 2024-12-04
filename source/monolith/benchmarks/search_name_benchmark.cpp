#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
// ./search_name_benchmark 10 /Users/AlexandrShadrukhin/Downloads gogol.txt
namespace fs = std::filesystem;

void searchName(const fs::path& directory, const std::string& filename) {
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

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Введите: " << argv[0] << " <количество итераций> <путь> <имя файла>" << std::endl;
    return 1;
  }

  int iterations = std::stoi(argv[1]);
  fs::path directory = argv[2];
  std::string filename = argv[3];

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; ++i) {
    searchName(directory, filename);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Выполнено за " << iterations << " итераций и: " << elapsed.count() << " с" << std::endl;

  return 0;
}