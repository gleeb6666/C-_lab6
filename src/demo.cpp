#include "demo.h"

#include "coroutines.h"
#include "transfer_controller.h"

#include <random>

namespace {

std::uint32_t resolveSeed(const Config& config) {
    return config.seedFromFile ? config.seed : std::random_device{}();
}

void printHeader(std::ostream& out, const Config& config, std::uint32_t seed) {
    out << "Кількість чисел: " << config.count << '\n'
        << "Seed: " << seed << (config.seedFromFile ? "" : " (випадковий)") << '\n'
        << "Діапазон: [" << kMinNumber << ", " << kMaxNumber << "]\n"
        << "A - парні числа, B - квадрати непарних\n\n";
}

}

void runDemo(const Config& config, std::ostream& out) {
    const std::uint32_t seed = resolveSeed(config);
    printHeader(out, config, seed);

    TransferController controller;
    const Task coroutineA = printNumber(controller, out);
    const Task coroutineB = printSquare(controller, out);
    controller.setEvenHandler(coroutineA.handle());
    controller.setOddHandler(coroutineB.handle());

    const Task generator = generateNumbers(controller, config.count, seed);
    controller.run(generator.handle());

    for (const Task* task : {&generator, &coroutineA, &coroutineB}) {
        task->rethrowIfFailed();
    }

    out << "\nГенерацію завершено, виконано кроків: " << controller.step() << '\n';
}
