#include <string_view>
#include <algorithm>
#include <stdexcept>

#include <cstdint>

#include "net/icmp_echo.hpp"
#include "net/icmp.hpp"

namespace
{
    std::size_t fill_echo_or_replay_request(void*                   data,
					    std::size_t             size,
					    proto::net::icmp::types type,
					    std::uint16_t           identifier,
					    std::uint16_t           sequence_number,
					    std::string_view        message)
    {
	auto pointer = reinterpret_cast<proto::net::icmp_echo*>(data);

	pointer->identifier(identifier);
	pointer->sequence_number(sequence_number);

	const auto length =
	    std::min(size - proto::net::icmp_echo::header_length(), message.size());

	std::copy_n(message.cbegin(), length, pointer->data());

	proto::net::icmp::fill_header(data, size, type, 0);

	return proto::net::icmp_echo::header_length() + length;
    }
}

namespace proto::net
{
    std::size_t icmp_echo::fill_echo_replay(void*            data,
					    std::size_t      size,
					    std::uint16_t    identifier,
					    std::uint16_t    sequence_number,
					    std::string_view message)
    {
	if (size < header_length())
	{
	    constexpr auto what =
		"icmp_echo::fill_echo_replay: size < icmp_echo::header_length";

	    throw std::out_of_range {what};
	}

	return fill_echo_or_replay_request(data,
					   size,
					   types::echo_replay,
					   identifier,
					   sequence_number,
					   message);
    }

    const icmp_echo& icmp_echo::representation(const void* data, std::size_t size)
    {
	if (size < header_length())
	{
	    constexpr auto what =
		"icmp_echo::representation: size < icmp_echo::header_length";

	    throw std::out_of_range {what};
	}

	return *reinterpret_cast<const icmp_echo*>(data);
    }

    std::size_t icmp_echo::fill_echo_request(void*            data,
					     std::size_t      size,
					     std::uint16_t    identifier,
					     std::uint16_t    sequence_number,
					     std::string_view message)
    {
	if (size < header_length())
	{
	    constexpr auto what =
		"icmp_echo::fill_echo_request: size < icmp_echo::header_length";

	    throw std::out_of_range {what};
	}

	return fill_echo_or_replay_request(data,
					   size,
					   types::echo_request,
					   identifier,
					   sequence_number,
					   message);
    }
}
