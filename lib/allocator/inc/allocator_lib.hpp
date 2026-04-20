//
// Created by Stanislav on 01.04.2026.
//

#ifndef ALLOCATOR_LIB_HPP
#define ALLOCATOR_LIB_HPP

#include <cstdint>
#include <iostream>
#include <memory>

struct MyAllocatorState {
    std::size_t currentNum_ = 0;
};

template <typename T, std::size_t NMax>
struct MyAllocator {
    using value_type = T;

    template <typename U, std::size_t M> friend struct MyAllocator;

    std::shared_ptr<MyAllocatorState> state_;

    MyAllocator() : state_(std::make_shared<MyAllocatorState>()) {}

    template <typename U>
    MyAllocator(const MyAllocator<U, NMax>& other) noexcept
            : state_(other.state_) {}

    template <typename U>
    struct rebind {
        using other = MyAllocator<U, NMax>;
    };

    template <typename U>
    bool operator==(const MyAllocator<U, NMax>& other) const noexcept {
        return state_ == other.state_;
    }
    template <typename U>
    bool operator!=(const MyAllocator<U, NMax>& other) const noexcept {
        return *this != other;
    }

    value_type* allocate(std::size_t n) {
        if (state_->currentNum_ + n > NMax) {
            throw std::bad_alloc();
        }
        T* p = std::allocator<T>{}.allocate(n);
        state_->currentNum_ += n;
        return p;
    }

    void deallocate(value_type* p, std::size_t n) noexcept {
        std::allocator<T>{}.deallocate(p, n);
        state_->currentNum_ -= n;
    }
};

#endif //ALLOCATOR_LIB_HPP
