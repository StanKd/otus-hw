//
// Created by Stanislav on 13.03.2026.
//

#ifndef IP_FILTER_HPP
#define IP_FILTER_HPP

union IPv4
{
    uint32_t addr; // 32 bit notation
    uint8_t bytes[4];
};

void filter (std::optional<std::uint8_t> any,
             std::optional<std::uint8_t> b0,
             std::optional<std::uint8_t> b1,
             std::optional<std::uint8_t> b2,
             std::optional<std::uint8_t> b3,
             const std::vector<IPv4> &v);

#endif //IP_FILTER_HPP
