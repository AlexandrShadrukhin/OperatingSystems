#include <sstream>
#include <vector>
#include <string>

namespace monolith::app {

std::vector<std::string> parseStr(const std::string &line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(line);

    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace monolith::app