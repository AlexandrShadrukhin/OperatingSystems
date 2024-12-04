#include "CdCommand.h"
#include <iostream>
#include <unistd.h>

void CdCommand::execute(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cerr << "Ошибка: укажите директорию.\n";
    return;
  }
  if (chdir(args[1].c_str()) != 0) {
    perror("cd");
  }
}

std::string CdCommand::getName() const {
  return "cd";
}

std::string CdCommand::getDescription() const {
  return "меняем директорию";
}