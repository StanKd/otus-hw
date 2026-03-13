//
// Created by Stanislav on 13.03.2026.
//

#include <gtest/gtest.h>
#include <optional>
#include <string>

#include "ip_filter.hpp"

TEST(filterTest, noValues)
{
    constexpr IPv4 ip{0x1020304};
    std::vector<IPv4> ips;
    ips.push_back(ip);

    testing::internal::CaptureStdout();
    filter(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, ips);
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_STREQ (output.c_str(), "");
}

TEST(filterTest, b0)
{
    constexpr IPv4 ip{0x1020304};
    std::vector<IPv4> ips;
    ips.push_back(ip);

    testing::internal::CaptureStdout();
    filter(std::nullopt, 1, std::nullopt, std::nullopt, std::nullopt, ips);
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_STREQ(output.c_str(), "1.2.3.4\n");
}

// TODO any, b1,b2,b3, b1+b2, b1+b3 ... etc 