#include <gtest/gtest.h>

#include "lib.h"

TEST(testVersion, GT)
{
    EXPECT_GT (version(), 0) << "version must be greater then 0";
}