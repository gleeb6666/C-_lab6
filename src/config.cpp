#include "config.h"

#include <charconv>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>

namespace {

constexpr char kCommentMark = '#';

std::vector<std::string> readTokens(std::istream& in) {
    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(in, line)) {
        line = line.substr(0, line.find(kCommentMark));
        std::istringstream lineStream(line);
        std::string token;
        while (lineStream >> token) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

unsigned long long parseNatural(const std::string& token, const std::string& name,
                                unsigned long long min, unsigned long long max) {
    unsigned long long value = 0;
    const char* begin = token.data();
    const char* end = begin + token.size();
    const auto [ptr, error] = std::from_chars(begin, end, value);
    if (error == std::errc::result_out_of_range) {
        throw ConfigError(name + " \"" + token + "\" занадто велике");
    }
    if (error != std::errc() || ptr != end) {
        throw ConfigError(name + " \"" + token + "\" не є цілим невід'ємним числом");
    }
    if (value < min || value > max) {
        throw ConfigError(name + " має бути в межах [" + std::to_string(min) + ", " +
                          std::to_string(max) + "], отримано " + token);
    }
    return value;
}

}

Config readConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw ConfigError("не вдалося відкрити файл \"" + path + "\"");
    }

    const std::vector<std::string> tokens = readTokens(file);
    if (file.bad()) {
        throw ConfigError("помилка читання файлу \"" + path + "\"");
    }
    if (tokens.empty()) {
        throw ConfigError("файл не містить кількості чисел");
    }
    if (tokens.size() > 2) {
        throw ConfigError("зайві дані у файлі: \"" + tokens[2] + "\"");
    }

    Config config;
    config.count = parseNatural(tokens[0], "Кількість чисел", 1, kMaxCount);
    if (tokens.size() == 2) {
        config.seed = static_cast<std::uint32_t>(
            parseNatural(tokens[1], "Seed", 0, std::numeric_limits<std::uint32_t>::max()));
        config.seedFromFile = true;
    }
    return config;
}
