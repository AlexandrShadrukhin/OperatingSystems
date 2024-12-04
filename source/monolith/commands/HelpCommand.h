#ifndef MONOLITH_HELP_COMMAND_H
#define MONOLITH_HELP_COMMAND_H

#include "Command.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class HelpCommand : public Command {
public:
  explicit HelpCommand(std::map<std::string, std::shared_ptr<Command>>& commands);

  void execute(const std::vector<std::string>& args) override;
  std::string getName() const override;
  std::string getDescription() const override;

private:
  std::map<std::string, std::shared_ptr<Command>>& commands;
};

#endif