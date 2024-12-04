#ifndef MONOLITH_EXIT_COMMAND_H
#define MONOLITH_EXIT_COMMAND_H

#include "Command.h"
#include <string>
#include <vector>

class ExitCommand : public Command {
public:
  void execute(const std::vector<std::string>& args) override;
  std::string getName() const override;
  std::string getDescription() const override;
};

#endif