#include <stdexcept>

#include <cstdint>

#include "net/icmp.hpp"

namespace proto::net
{
    void icmp::calculate_checksum(std::size_t header_length) noexcept
    {
	checksum_ = 0;

	uint32_t sum = 0;

	auto pointer = reinterpret_cast<const uint16_t*>(this);

	for (; header_length; header_length -= 2)
	{
	    sum += *pointer++;
	}

	if (header_length == 1)
	{
	    sum += *reinterpret_cast<const uint8_t*>(pointer);
	}

	while (sum >> 16)
	{
	    sum += (sum >> 16) + (sum & 0xFFFF);
	}

	checksum_ = static_cast<uint16_t>(~sum);
    }

    std::size_t icmp::fill_header(void*       data,
				  std::size_t size,
				  types       type,
				  uint8_t     code)
    {
	if (size < icmp::header_length())
	{
	    constexpr auto what =
		"icmp::fill_header: size < icmp::header_length()";

	    throw std::out_of_range {what};
	}

	auto pointer = reinterpret_cast<icmp*>(data);

	pointer->type(type);
	pointer->code(code);
	pointer->calculate_checksum(size);

	return icmp::header_length();
    }
}
