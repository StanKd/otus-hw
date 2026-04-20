//
// Created by Stanislav on 01.04.2026.
//
#include <cstdint>
#include <gtest/gtest.h>
#include <allocator_lib.hpp>

TEST (allocator, testAlloc) {
    static constexpr std::size_t TEST_ELEM_NUM {10U};

    MyAllocator<std::uint32_t, TEST_ELEM_NUM> allocator;

    auto * pData = allocator.allocate(TEST_ELEM_NUM);
    EXPECT_EQ(TEST_ELEM_NUM, allocator.state_->currentNum_);

    allocator.deallocate(pData, TEST_ELEM_NUM);

    EXPECT_EQ(0u, allocator.state_->currentNum_);
}