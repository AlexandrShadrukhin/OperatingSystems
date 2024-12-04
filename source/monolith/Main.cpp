#include <sys/wait.h>
#include "commands/Command.h"
#include "commands/CdCommand.h"
#include "commands/ExitCommand.h"
#include "commands/HelpCommand.h"
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <unistd.h>   // For fork() and execvp()
#include <vector>

using namespace std;
using namespace chrono;

namespace monolith::app {

string readLine() {
  string str;
  cout << "shell >> ";
  getline(cin, str);
  return str;
}

vector<string> parseStr(const string& line) {
  vector<string> tokens;
  string token;
  istringstream stream(line);

  while (stream >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

void executeCommand(const std::vector<std::string>& args) {
  map<string, shared_ptr<Command> > commands;
  commands["cd"] = make_shared<CdCommand>();
  commands["exit"] = make_shared<ExitCommand>();
  commands["help"] = make_shared<HelpCommand>(commands);

  if (args.empty()) {
    return;
  }

  auto it = commands.find(args[0]);
  if (it != commands.end()) {
    it->second->execute(args);
  } else {
    pid_t pid = fork();
    if (pid == 0) {
      vector<const char*> c_args;
      for (const auto& arg : args) {
        c_args.push_back(arg.c_str());
      }
      c_args.push_back(nullptr);
      if (execvp(c_args[0], const_cast<char* const*>(c_args.data())) == -1) {
        perror("\nexecvp");
        exit(1);
      }
    } else if (pid > 0) {
      int status = 0;
      waitpid(pid, &status, 0);
    } else {
      perror("fork");
    }
  }
}

void loop() {
  while (true) {
    string line = readLine();
    vector<string> args = parseStr(line);

    for (size_t i = 0; i < args.size(); ++i) {
      cout << "Args[" << i << "] " << args[i] << "\n";
    }

    auto start = high_resolution_clock::now();
    executeCommand(args);
    auto end = high_resolution_clock::now();
    const duration<double> work_time = end - start;

    cout << "Время работы: " << work_time.count() << " с\n";
  }
}

}

int main() {
    cout << "Добро пожаловать в мою оболочку командной строки!!! \n" ;
    monolith::app::loop();
}