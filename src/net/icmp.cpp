#include <string_view>

#include <cstdint>

#include "net/icmp.hpp"

namespace proto::net
{
    std::string_view icmp::to_string(icmp::type_enumerator enumerator) noexcept
    {
	using enum type_enumerator;

	switch (enumerator)
	{
	case echo_replay:   return "echo replay message";
	case echo:          return "echo message";
	case time_exceeded: return "time exceeded message";

	default: return "unspecified";
	}
    }

    void icmp::calculate_checksum(std::size_t size) noexcept
    {
	checksum_ = 0;

	uint32_t sum = 0;

	auto pointer = reinterpret_cast<const uint16_t*>(this);

	for (; size; size -= 2)
	{
	    sum += *pointer++;
	}

	if (size == 1)
	{
	    sum += *reinterpret_cast<const uint8_t*>(pointer);
	}

	while (sum >> 16)
	{
	    sum += (sum >> 16) + (sum & 0xFFFF);
	}

	checksum_ = static_cast<uint16_t>(~sum);
    }

    std::size_t make_icmp_header(void*       data,
				 std::size_t size,
				 uint8_t     type,
				 uint8_t     code) noexcept
    {
	if (size < icmp::header_length())
	{
	    return 0;
	}

	auto pointer = reinterpret_cast<icmp*>(data);

	pointer->type(type);
	pointer->code(code);
	pointer->calculate_checksum(size);

	return icmp::header_length();
    }
}
