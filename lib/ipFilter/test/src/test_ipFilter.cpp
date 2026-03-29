//
// Created by Stanislav on 28.03.2026.
//

#include <gtest/gtest.h>
#include <ipFilter_lib.hpp>
#include <limits>

TEST (parseLine, testIncompleteAddr) {
    std::string s = "1.2.3";
    IPv4 ip{};

    EXPECT_FALSE(parseLine(s, ip));

    s = " .2.3.4";

    EXPECT_FALSE(parseLine(s, ip));
}

TEST (parseLine, testInvalidValue) {
    std::string s = "256.2.3.4\t";
    IPv4 ip{};

    EXPECT_FALSE(parseLine(s, ip));

    s = "1.2.3.256\t";

    EXPECT_FALSE(parseLine(s, ip));

    s = "1. .3.256\t";

    EXPECT_FALSE(parseLine(s, ip));
}

TEST (parseLine, testValidValue) {
    std::string s = "255.255.255.255\t";
    IPv4 ip{};

    EXPECT_TRUE(parseLine(s, ip));
    EXPECT_EQ(ip.addr, std::numeric_limits<std::uint32_t>::max());

    s = "0.0.0.0\t";

    EXPECT_TRUE(parseLine(s, ip));
    EXPECT_EQ(ip.addr, std::numeric_limits<std::uint32_t>::min());

    s = "1.2.3.4\t";
    EXPECT_TRUE(parseLine(s, ip));
    EXPECT_EQ(ip.addr, 0x04030201);

    s = "4.3.2.1\t";
    EXPECT_TRUE(parseLine(s, ip));
    EXPECT_EQ(ip.addr, 0x01020304);

    s = "1.2.3.4.5.6.7.8.9\t";
    EXPECT_TRUE(parseLine(s, ip));
    EXPECT_EQ(ip.addr, 0x04030201);
}

TEST (filter, testOne) {
    std::vector<IPv4> v {{0x01020304}, {0x04030201}};
    fltReq req{B0, 0x01};

    testing::internal::CaptureStdout();
    filter(v, req);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "1.2.3.4\n");

    v.push_back({0x03020100});
    req = {B1, 0x01};

    testing::internal::CaptureStdout();
    filter(v, req);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "0.1.2.3\n");

    v.push_back({0xFFCCAABB});
    req = {B2, 0xCC};

    testing::internal::CaptureStdout();
    filter(v, req);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "187.170.204.255\n");

    req = {B3, 0x04};

    testing::internal::CaptureStdout();
    filter(v, req);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "1.2.3.4\n");
}

TEST (filter, testTwo) {
    std::vector<IPv4> v {{0x01020304}, {0x01040203}, {0x04020301}, {0x04030201}};
    fltReq req1{B0, 0x01};
    fltReq req2{B1, 0x02};

    testing::internal::CaptureStdout();
    filter(v, req1, req2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "1.2.3.4\n");

    testing::internal::CaptureStdout();
    filter(v, req2, req1);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "1.2.3.4\n");

    req1 = {B2, 0x02};
    req2 = {B3, 0x01};

    testing::internal::CaptureStdout();
    filter(v, req2, req1);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "4.3.2.1\n");

    req1 = {B0, 0x01};
    req2 = {B3, 0x04};

    testing::internal::CaptureStdout();
    filter(v, req2, req1);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "1.3.2.4\n1.2.3.4\n");
}

TEST (filter, testAny) {
    std::vector<IPv4> v {{0x01020304}, {0x01040203}, {0x02040203}, {0x05060204}, {0x04020301}, {0x04030201}};
    fltReq req{ANY, 0x01};

    testing::internal::CaptureStdout();
    filter(v, req);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "4.3.2.1\n3.2.4.1\n1.3.2.4\n1.2.3.4\n");

    req = {ANY, 0x05};
    fltReq req1{ANY, 0x02};

    testing::internal::CaptureStdout();
    filter(v, req, req1);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "4.2.6.5\n");

    req = {ANY, 0x04};
    req1 = {ANY, 0x03};
    fltReq req2 = {ANY, 0x02};

    testing::internal::CaptureStdout();
    filter(v, req, req1, req2);
    output = testing::internal::GetCapturedStdout();
    EXPECT_STREQ(output.c_str(), "4.3.2.1\n3.2.4.1\n3.2.4.2\n1.3.2.4\n1.2.3.4\n");
}