#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <chrono>
#include <optional>

#ifdef _WIN32
    #include <winsock2.h>
    // #pragma comment(lib, "Ws2_32.lib")
#else
    #include <arpa/inet.h>
#endif

#include"ip_filter.hpp"

static IPv4 parseLine(const std::string& str)
{
    IPv4 ip{};
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of('.');
    unsigned int num{};

    while (stop != std::string::npos && num < std::size(ip.bytes))
    {
        ip.bytes[num] = static_cast<std::uint8_t>(std::strtol(str.substr(start, stop - start).c_str(), nullptr, 10));

        ++num;
        start = stop + 1;
        stop = str.find_first_of('.', start);

        if (stop == std::string::npos)
        {
            stop = str.find_first_of('\t', start);
        }
    }

    return ip;
}

#ifdef PRINT_TIME
template <typename T>
static void printTime(T t1, T t2)
{
    /* Getting number of microseconds as an integer. */
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

    std::cout << "duration time us: "<< us.count() << std::endl;
}
#endif

static void printIP(const IPv4 &ip)
{
    std:: cout << static_cast<unsigned int>(ip.bytes[0]) << '.'
               << static_cast<unsigned int>(ip.bytes[1]) << '.'
               << static_cast<unsigned int>(ip.bytes[2]) << '.'
               << static_cast<unsigned int>(ip.bytes[3]) << std::endl;
}

void filter (const std::optional<std::uint8_t> any,
             const std::optional<std::uint8_t> b0,
             const std::optional<std::uint8_t> b1,
             const std::optional<std::uint8_t> b2,
             const std::optional<std::uint8_t> b3,
             const std::vector<IPv4> &v)
{
    if (any.has_value())
    {
        for (auto ip : v)
        {
            if (ip.bytes[0] == any.value() || ip.bytes[1] == any.value() ||
                ip.bytes[2] == any.value() || ip.bytes[3] == any.value())
            {
                printIP(ip);
            }
        }
    } else if (b0.has_value() || b1.has_value() || b2.has_value() || b3.has_value())
    {
        for (auto ip : v)
        {
            if ((b0.has_value() ? b0.value() == ip.bytes[0] : true) &&
                (b1.has_value() ? b1.value() == ip.bytes[1] : true) &&
                (b2.has_value() ? b2.value() == ip.bytes[2] : true) &&
                (b3.has_value() ? b3.value() == ip.bytes[3] : true))
            {
                printIP(ip);
            }
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[])
{
#ifdef PRINT_TIME
    using std::chrono::high_resolution_clock;
    const auto t1 = high_resolution_clock::now();
#endif

    try
    {
        std::vector<IPv4> ips;

        for (std::string line; std::getline(std::cin, line);)
        {
            ips.push_back(parseLine(line));
        }

        // TODO reverse lexicographically sort - selection
        for (unsigned i = 0U; i < ips.size(); ++i)
        {
            std::uint32_t biggest{};
            unsigned biggestIndex = 0U;
            for (unsigned j = i; j < ips.size(); ++j)
            {
                std::uint32_t ip = ntohl(ips[j].addr);
                if (ip > biggest)
                {
                    biggest = ip;
                    biggestIndex = j;
                }
            }
            std::swap(ips[i], ips[biggestIndex]);

            printIP(ips[i]);
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // std::cout <<"------------filter by first 1------------" << std::endl;
        // TODO filter by first byte and output
        filter(std::nullopt, 1U, std::nullopt, std::nullopt, std::nullopt, ips);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // std::cout <<"------------filter by 46 70------------" << std::endl;

        // ip = filter(46, 70)
        filter(std::nullopt, 46U, 70U, std::nullopt, std::nullopt, ips);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output

        // std::cout <<"------------filter by any 46------------" << std::endl;
        // ip = filter_any(46)

        filter(46u, std::nullopt, std::nullopt, std::nullopt, std::nullopt, ips);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

#ifdef PRINT_TIME
    const auto t2 = high_resolution_clock::now();
    printTime(t1, t2);
#endif

    return 0;
}
