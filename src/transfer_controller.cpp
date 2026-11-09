#include "transfer_controller.h"

#include <stdexcept>
#include <utility>

TransferController::DispatchAwaiter::DispatchAwaiter(TransferController& controller,
                                                     std::uint32_t value) noexcept
    : controller_(controller), value_(value) {}

void TransferController::DispatchAwaiter::await_suspend(
    std::coroutine_handle<> generator) noexcept {
    controller_.generator_ = generator;
    controller_.currentValue_ = value_;
    ++controller_.step_;
    controller_.next_ = controller_.handlerFor(value_);
}

TransferController::ReturnAwaiter::ReturnAwaiter(TransferController& controller) noexcept
    : controller_(controller) {}

void TransferController::ReturnAwaiter::await_suspend(std::coroutine_handle<>) const noexcept {
    controller_.next_ = controller_.generator_;
}

void TransferController::setEvenHandler(std::coroutine_handle<> handler) noexcept {
    evenHandler_ = handler;
}

void TransferController::setOddHandler(std::coroutine_handle<> handler) noexcept {
    oddHandler_ = handler;
}

TransferController::DispatchAwaiter TransferController::dispatch(std::uint32_t value) {
    if (!handlerFor(value)) {
        throw std::logic_error("Не задано сопрограму-обробника для числа");
    }
    return DispatchAwaiter(*this, value);
}

TransferController::ReturnAwaiter TransferController::returnToGenerator() noexcept {
    return ReturnAwaiter(*this);
}

void TransferController::run(std::coroutine_handle<> generator) {
    next_ = generator;
    while (next_ && !next_.done()) {
        std::exchange(next_, nullptr).resume();
    }
}

std::coroutine_handle<> TransferController::handlerFor(std::uint32_t value) const noexcept {
    return value % 2 == 0 ? evenHandler_ : oddHandler_;
}
