#pragma once

#include <coroutine>
#include <cstddef>
#include <cstdint>

// Контролер очікування: вирішує, якій сопрограмі передати керування.
class TransferController {
public:
    class DispatchAwaiter {
    public:
        DispatchAwaiter(TransferController& controller, std::uint32_t value) noexcept;
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> generator) noexcept;
        void await_resume() const noexcept {}

    private:
        TransferController& controller_;
        std::uint32_t value_;
    };

    class ReturnAwaiter {
    public:
        explicit ReturnAwaiter(TransferController& controller) noexcept;
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> handler) const noexcept;
        void await_resume() const noexcept {}

    private:
        TransferController& controller_;
    };

    void setEvenHandler(std::coroutine_handle<> handler) noexcept;
    void setOddHandler(std::coroutine_handle<> handler) noexcept;

    DispatchAwaiter dispatch(std::uint32_t value);
    ReturnAwaiter returnToGenerator() noexcept;

    void run(std::coroutine_handle<> generator);

    std::uint32_t currentValue() const noexcept { return currentValue_; }
    std::size_t step() const noexcept { return step_; }

private:
    std::coroutine_handle<> handlerFor(std::uint32_t value) const noexcept;

    std::coroutine_handle<> generator_;
    std::coroutine_handle<> next_;
    std::coroutine_handle<> evenHandler_;
    std::coroutine_handle<> oddHandler_;
    std::uint32_t currentValue_ = 0;
    std::size_t step_ = 0;
};
