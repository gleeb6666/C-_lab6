#pragma once

#include <coroutine>
#include <exception>
#include <utility>

class Task {
public:
    struct promise_type {
        std::exception_ptr exception;

        Task get_return_object() {
            return Task(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {}

    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            destroy();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

    ~Task() { destroy(); }

    std::coroutine_handle<> handle() const noexcept { return handle_; }

    void rethrowIfFailed() const {
        if (handle_ && handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    explicit Task(std::coroutine_handle<promise_type> handle) noexcept : handle_(handle) {}

    void destroy() noexcept {
        if (handle_) {
            handle_.destroy();
            handle_ = nullptr;
        }
    }

    std::coroutine_handle<promise_type> handle_;
};
