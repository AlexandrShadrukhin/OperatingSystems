#include "ExitCommand.h"
#include <iostream>
#include <cstdlib> // Для exit()

void ExitCommand::execute(const std::vector<std::string>& args) {
  std::exit(0);
}

std::string ExitCommand::getName() const {
  return "exit";
}

std::string ExitCommand::getDescription() const {
  return "Выход из программы.";
}