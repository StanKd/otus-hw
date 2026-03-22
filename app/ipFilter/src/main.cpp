//
// Created by Stanislav on 23.03.2026.
//
#include <iostream>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "ipFilter_lib.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
    try
    {
        std::vector<IPv4> ips;

        for (std::string line; std::getline(std::cin, line);)
        {
            if (IPv4 ip{}; parseLine(line, ip))
            {
                ips.push_back(ip);
            }
        }

        std::sort(ips.begin(), ips.end(), [](const IPv4& a, const IPv4& b)
        {
            return ntohl(a.addr) > ntohl(b.addr);
        });

        for_each(ips.begin(), ips.end(), [](const IPv4& ip){printIP(ip);});

        filter(ips, fltReq{B0,1U});
        filter(ips, fltReq{B0,46U}, fltReq{B1,70U});
        filter(ips, fltReq{ANY,46U});
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}