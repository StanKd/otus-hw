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

template<typename T, std::size_t NMax>
struct MyAllocator {
    using value_type = T;

    template<typename U, std::size_t M> friend
    struct MyAllocator;

    std::shared_ptr<MyAllocatorState> state_;

    MyAllocator() : state_(std::make_shared<MyAllocatorState>()) {}

    template<typename U>
    MyAllocator(const MyAllocator<U, NMax> &other) noexcept
            : state_(other.state_) {}

    template<typename U>
    struct rebind {
        using other = MyAllocator<U, NMax>;
    };

    template<typename U>
    bool operator==(const MyAllocator<U, NMax> &other) const noexcept {
        return state_ == other.state_;
    }

    template<typename U>
    bool operator!=(const MyAllocator<U, NMax> &other) const noexcept {
        return *this != other;
    }

    value_type *allocate(std::size_t n) {
        if (state_->currentNum_ + n > NMax) {
            throw std::bad_alloc();
        }
        void* p = ::operator new(n * sizeof(value_type));
        state_->currentNum_ += n;
        return static_cast<value_type*>(p);
    }

    void deallocate(value_type *p, std::size_t n) noexcept {
        delete(p);
        state_->currentNum_ -= n;
    }
};

template<typename T, typename Allocator = std::allocator<T>>
class MyContainer {
    struct Node {
        T value;
        Node *next;
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using AllocTraits = std::allocator_traits<NodeAlloc>;

    NodeAlloc alloc_;
    Node *head_ = nullptr;

public:
    MyContainer() = default;

    ~MyContainer() {
        while (head_) {
            Node *next = head_->next;
            AllocTraits::destroy(alloc_, head_);
            AllocTraits::deallocate(alloc_, head_, 1);
            head_ = next;
        }
    }

    class Iterator {
        Node *current_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        Iterator(Node *node) : current_(node) {}

        reference operator*() const { return current_->value; }
        pointer operator->() { return &(current_->value); }

        Iterator &operator++() {
            if (current_) current_ = current_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const { return current_ == other.current_; }
        bool operator!=(const Iterator &other) const { return current_ != other.current_; }
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }

    void push_front(const T &value) {
        Node *newNode = AllocTraits::allocate(alloc_, 1);
        AllocTraits::construct(alloc_, newNode, Node{value, head_});
        head_ = newNode;
    }
};

#endif //ALLOCATOR_LIB_HPP
