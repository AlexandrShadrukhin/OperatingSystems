#include <chrono>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
//./dedup_benchmark 1000 100000
void dedup(std::vector<std::string> &data) {
    std::unordered_set<std::string> unique_elements;
    std::vector<std::string> deduplicated;

    for (const auto &elem : data) {
        if (unique_elements.insert(elem).second) {
            deduplicated.push_back(elem);
        }
    }

    data = std::move(deduplicated);
}

//int main(int argc, char *argv[]) {
//  if (argc < 3) {
//    std::cerr << "Введите: " << argv[0]
//              << " <количество итераций> <размер массива>" << std::endl;
//    return 1;
//  }
//
//  int iterations = std::stoi(argv[1]);
//  int size = std::stoi(argv[2]);
//
//  std::vector<std::string> data;
//  for (int i = 0; i < size; ++i) {
//    data.push_back("элемент" + std::to_string(i % 10));
//  }
//
//  auto start = std::chrono::high_resolution_clock::now();
//
//  for (int i = 0; i < iterations; ++i) {
//    dedup(data);
//  }
//
//  auto end = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> elapsed = end - start;
//
//  std::cout << "Выполнено за " << iterations
//            << " итераций и: " << elapsed.count() << " с" << std::endl;
//
//  return 0;
//}