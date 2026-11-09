#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

inline constexpr std::size_t kMaxCount = 1'000'000;

struct Config {
    std::size_t count = 0;
    std::uint32_t seed = 0;
    bool seedFromFile = false;
};

class ConfigError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

Config readConfig(const std::string& path);
