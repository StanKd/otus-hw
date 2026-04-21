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
    // создание экземпляра std::map<int, int>
    std::map<int, int> defaultMap_;

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        defaultMap_[i] = static_cast<int>(factorial(i));
    }

    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    std::map<int, int, std::less<>, MyAllocator<std::pair<const int, int>, 10>> customMap_;

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    try {
        for (int i = 0; i < 10; ++i) {
            customMap_[i] = static_cast<int>(factorial(i));
        }
    } catch (const std::bad_alloc&) {
        std::cout << "too much elems" << std::endl;
    }

    // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    for (auto & elem : customMap_) {
        std::cout << elem.first << " " << elem.second << std::endl;
    }

    // создание экземпляра своего контейнера для хранения значений типа int
    MyContainer<int> defaultAllocContainer_;

    // заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        defaultAllocContainer_.push_front(i);
    }

    // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
    MyContainer<int, MyAllocator<int, 10>> customAllocContainer_;

    // заполнение 10 элементами от 0 до 9
    try {
        for (int i = 0; i < 10; ++i) {
            customAllocContainer_.push_front(i);
        }
    } catch (const std::bad_alloc&) {
        std::cout << "too much elems" << std::endl;
    }

    // вывод на экран всех значений, хранящихся в контейнере
    for (auto & elem : customAllocContainer_) {
        std::cout << elem << std::endl;
    }

    return 0;
}