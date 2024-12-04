#include "HelpCommand.h"
#include <iostream>

HelpCommand::HelpCommand(std::map<std::string, std::shared_ptr<Command>>& commands)
    : commands(commands) {}

void HelpCommand::execute(const std::vector<std::string>& args) {
  if (args.size() == 2) {
    auto it = commands.find(args[1]);
    if (it != commands.end()) {
      std::cout << it->second->getName() << " - "
                << it->second->getDescription() << "\n";
    } else {
      std::cerr << "Команда " << args[1] << " не найдена.\n";
    }
  } else if (args.size() == 1) {
    for (const auto& pair : commands) {
      std::cout << pair.first << " - " << pair.second->getDescription() << "\n";
    }
  } else {
    std::cerr << "Использование: help [command]\n";
  }
}

std::string HelpCommand::getName() const {
  return "help";
}

std::string HelpCommand::getDescription() const {
  return "Отображает список доступных команд или информацию о конкретной команде.";
}