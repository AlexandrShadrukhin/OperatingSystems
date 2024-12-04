#ifndef MONOLITH_CD_COMMAND_H
#define MONOLITH_CD_COMMAND_H

#include "Command.h"
#include <string>
#include <vector>

class CdCommand : public Command {
public:
  void execute(const std::vector<std::string>& args) override;
  std::string getName() const override;
  std::string getDescription() const override;
};

#endif