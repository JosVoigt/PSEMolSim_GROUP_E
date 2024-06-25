#include "StringUtils.h"

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;

    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.size();
        end = s.find(delimiter, start);
    }

    tokens.push_back(s.substr(start, end));

    return tokens;
}