#include <system_error>
#include <string_view>
#include <stdexcept>
#include <string>
#include <array>

#include <cstdint>
#include <cerrno>

extern "C"
{

#include <netinet/in.h>
#include <arpa/inet.h>

}

#include "tool/mutable_buffer.hpp"
#include "net/ipv4.hpp"

namespace
{
    std::uint32_t to_binary(std::string_view address,
			    std::error_code& error) noexcept
    {
	std::uint32_t addr;

	if (::inet_pton(AF_INET, address.data(), &addr) == -1)
	{
	    error = std::make_error_code(std::errc {errno});
	}

	return addr;
    }

    std::string to_string(std::uint32_t address, std::error_code& error)
    {
	static thread_local std::array<char, INET_ADDRSTRLEN> buffer;

	auto retval =
	    ::inet_ntop(AF_INET,
			reinterpret_cast<::in_addr_t*>(&address),
			buffer.data(),
			buffer.size());

	if (retval == nullptr)
	{
	    error = std::make_error_code(std::errc {errno});
	}

	return std::string(buffer.data());
    }
}

namespace proto::net
{
    void ipv4::calculate_checksum() noexcept
    {
	ip.check = 0;

	std::uint32_t sum = 0;

	auto pointer = reinterpret_cast<const std::uint16_t*>(this);

	auto header_length = this->header_length() / 2;

	for (; header_length; --header_length)
	{
	    sum += *pointer++;
	}

	while (sum >> 16)
	{
	    sum = (sum & 0xFFFF) + (sum >> 16);
	}

	ip.check = static_cast<std::uint16_t>(~sum);
    }

    std::string ipv4::source_address() const
    {
	std::error_code error;

	if (auto string = to_string(ip.saddr, error); error)
	{
	    throw std::system_error {error, "ipv4::source_address"};
	}

	else
	{
	    return string;
	}
    }

    void ipv4::source_address(std::string_view address)
    {
	std::error_code error;

	if (auto addr = to_binary(address, error); error)
	{
	    throw std::system_error {error, "ipv4::source_address"};
	}

	else
	{
	    ip.saddr = addr;
	}
    }

    std::string ipv4::destination_address() const
    {
	std::error_code error;

	if (auto string = to_string(ip.daddr, error); error)
	{
	    throw std::system_error {error, "ipv4::source_address"};
	}

	else
	{
	    return string;
	}
    }

    void ipv4::destination_address(std::string_view address)
    {
	std::error_code error;

	if (auto addr = to_binary(address, error); error)
	{
	    throw std::system_error {error, "ipv4::destination_address"};
	}

	else
	{
	    ip.daddr = addr;
	}
    }

    std::size_t ipv4::fill_header(void*            data,
				  std::size_t      size,
				  std::uint16_t    identifier,
				  std::uint8_t     time_to_live,
				  protocols        protocol,
				  std::string_view source_address,
				  std::string_view destination_address)
    {
	if (size < minimum_header_length())
	{
	    constexpr auto what =
		"ipv4::fill_header: size < minimum_header_length";

	    throw std::out_of_range {what};
	}

	auto& ip_header = representation(data, size);

	ip_header.version(4);
	ip_header.header_length(minimum_header_length());
	ip_header.type_of_service(0);
	ip_header.total_length(minimum_header_length());
	ip_header.identifier(identifier);
	ip_header.flags(0);
	ip_header.fragment_offset(0);
	ip_header.time_to_live(time_to_live);
	ip_header.protocol(protocol);

	ip_header.source_address(source_address);
	ip_header.destination_address(destination_address);

	return minimum_header_length();
    }

    const ipv4& ipv4::representation(const void* data, std::size_t size)
    {
	if (size < minimum_header_length())
	{
	    constexpr auto what =
		"ipv4::representation: size < minimum_header_length";

	    throw std::out_of_range {what};
	}

	return *reinterpret_cast<const ipv4*>(data);
    }
}
