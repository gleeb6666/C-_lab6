#pragma once

#include "task.h"
#include "transfer_controller.h"

#include <cstddef>
#include <cstdint>
#include <ostream>

inline constexpr std::uint32_t kMinNumber = 1;
inline constexpr std::uint32_t kMaxNumber = 256;

Task generateNumbers(TransferController& controller, std::size_t count, std::uint32_t seed);

Task printNumber(TransferController& controller, std::ostream& out);

Task printSquare(TransferController& controller, std::ostream& out);
