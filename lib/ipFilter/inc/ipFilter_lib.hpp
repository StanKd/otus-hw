//
// Created by Stanislav on 23.03.2026.
//

#ifndef IP_FILTER_IPFILTER_LIB_HPP
#define IP_FILTER_IPFILTER_LIB_HPP

#include <cstdint>

union IPv4
{
    uint32_t addr; // 32 bit notation
    uint8_t bytes[4];
};

enum filterReqType {
    B0,
    B1,
    B2,
    B3,
    ANY,
    NONE
};

struct fltReq {
    filterReqType posType = NONE;
    std::uint8_t target = 0U;
};

bool parseLine(const std::string& str, IPv4& ip);
void printIP(const IPv4& ip);

template <typename... TArgs>
void filter(const std::vector<IPv4>& v, TArgs... req){
    filterByReq(v, req...);
}
void filterByReq(const std::vector<IPv4>& v,
                 fltReq req0 = {NONE, 0}, fltReq req1 = {NONE, 0},
                 fltReq req2 = {NONE, 0}, fltReq req3 = {NONE, 0});

#endif //IP_FILTER_IPFILTER_LIB_HPP
