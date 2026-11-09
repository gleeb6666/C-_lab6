#include "coroutines.h"

#include <iomanip>
#include <random>

namespace {

constexpr int kStepWidth = 7;
constexpr int kNumberWidth = 3;

void printStep(std::ostream& out, std::size_t step, char name) {
    out << std::setw(kStepWidth) << step << ". " << name << ": ";
}

}

Task generateNumbers(TransferController& controller, std::size_t count, std::uint32_t seed) {
    std::mt19937 engine(seed);
    std::uniform_int_distribution<std::uint32_t> distribution(kMinNumber, kMaxNumber);
    for (std::size_t i = 0; i < count; ++i) {
        co_await controller.dispatch(distribution(engine));
    }
}

Task printNumber(TransferController& controller, std::ostream& out) {
    for (;;) {
        printStep(out, controller.step(), 'A');
        out << std::setw(kNumberWidth) << controller.currentValue() << '\n';
        co_await controller.returnToGenerator();
    }
}

Task printSquare(TransferController& controller, std::ostream& out) {
    for (;;) {
        const std::uint32_t value = controller.currentValue();
        printStep(out, controller.step(), 'B');
        out << std::setw(kNumberWidth) << value << "^2 = " << value * value << '\n';
        co_await controller.returnToGenerator();
    }
}
