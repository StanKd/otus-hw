//
// Created by Stanislav on 01.04.2026.
//

#include <iostream>
#include <map>

#include <allocator_lib.hpp>

constexpr long long factorial(int n)
{
    long long res = 1;
    for (int i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
    std::map<int, int> defaultMap_;

    for (int i = 0; i < 10; ++i) {
        defaultMap_[i] = static_cast<int>(factorial(i));
    }

    std::map<int, int, std::less<>, MyAllocator<std::pair<const int, int>, 10>> customMap_;

    try {
        for (int i = 0; i < 10; ++i) {
            customMap_[i] = static_cast<int>(factorial(i));
        }
    } catch (const std::bad_alloc&) {
        std::cout << "too much elems" << std::endl;
    }

    for (auto & elem : customMap_) {
        std::cout << elem.first << " " << elem.second << std::endl;
    }

//    std::cout << customMap_.get_allocator().state_->currentNum_ << " " << std::endl;
    return 0;
}