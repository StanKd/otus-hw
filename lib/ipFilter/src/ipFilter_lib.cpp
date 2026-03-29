//
// Created by Stanislav on 23.03.2026.
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "ipFilter_lib.hpp"

bool parseLine(const std::string &str, IPv4 &ip) {
    IPv4 ip_{};
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of('.');
    unsigned int num{};

    while (stop != std::string::npos && num < std::size(ip.bytes)) {
        const auto ip_part = std::strtol(str.substr(start, stop - start).c_str(), nullptr, 10);
        if (ip_part < 0 || ip_part > 255 || start == stop) {
            return false;
        }
        ip_.bytes[num] = ip_part;

        ++num;
        start = stop + 1;
        stop = str.find_first_of('.', start);

        if (stop == std::string::npos) {
            stop = str.find_first_of('\t', start);
        }
    }

    if (num == std::size(ip.bytes)) {
        ip = ip_;
        return true;
    }

    return false;
}

void printIP(const IPv4 &ip) {
    std::cout << static_cast<unsigned int>(ip.bytes[0]) << '.'
              << static_cast<unsigned int>(ip.bytes[1]) << '.'
              << static_cast<unsigned int>(ip.bytes[2]) << '.'
              << static_cast<unsigned int>(ip.bytes[3]) << std::endl;
}

void filterByReq(const std::vector<IPv4> &v, const fltReq req0, const fltReq req1,
                 const fltReq req2, const fltReq req3) {
    const std::vector<fltReq> reqs{req0, req1, req2, req3};

    for (auto &ip: v) {
        bool match = false;
        for (const auto &[posType, target]: reqs) {
            switch (posType) {
                default:
                case NONE:
                    match = true;
                    break;
                case B0:
                case B1:
                case B2:
                case B3:
                    match = target == ip.bytes[posType];
                    break;
                case ANY:
                    match = ip.bytes[0] == target || ip.bytes[1] == target ||
                            ip.bytes[2] == target || ip.bytes[3] == target;
                    break;
            }
            if (!match) {
                break;
            }
        }
        if (match) {
            printIP(ip);
        }
    }
}