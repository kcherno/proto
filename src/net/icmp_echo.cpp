#include <string_view>
#include <algorithm>
#include <cstdint>

#include "net/icmp_echo.hpp"
#include "net/icmp.hpp"

namespace proto::net
{
    std::size_t make_echo_request(void*            data,
				  std::size_t      size,
				  uint16_t         identifier,
				  uint16_t         sequence_number,
				  std::string_view message) noexcept
    {
	if (size < icmp_echo::header_length())
	{
	    return 0;
	}

	auto pointer = reinterpret_cast<icmp_echo*>(data);

	pointer->identifier(identifier);
	pointer->sequence_number(sequence_number);

	const auto length =
	    std::min(size - icmp_echo::header_length(), message.length());

	std::copy_n(message.cbegin(), length, pointer->data());

	return make_icmp_header(data, size, 8, 0);
    }
}
