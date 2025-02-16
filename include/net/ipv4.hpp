#ifndef NET_IPV4_HPP__
#define NET_IPV4_HPP__

#include <string_view>
#include <stdexcept>
#include <string>

#include <cstdint>

extern "C"
{

#include <netinet/ip.h>
#include <arpa/inet.h>

}

#include "tool/mutable_buffer.hpp"

namespace proto::net
{
    class ipv4 final {
    public:
	enum class protocols {
	    icmp = IPPROTO_ICMP,
	    tcp  = IPPROTO_TCP,
	    udp  = IPPROTO_UDP
	};

	ipv4() noexcept :
	    ip {}
	{}

	std::uint8_t version() const noexcept
	{
	    return ip.version;
	}

	void version(std::uint8_t other)
	{
	    if (other != 4)
	    {
		throw std::logic_error {"ipv4::version: version must be 4"};
	    }

	    ip.version = other;
	}

	std::size_t header_length() const noexcept
	{
	    return ip.ihl * 4;
	}

	inline void header_length(std::size_t);

	std::uint8_t type_of_service() const noexcept
	{
	    return ip.tos;
	}

	void type_of_service(std::uint8_t other) noexcept
	{
	    ip.tos = other;
	}

	std::size_t total_length() const noexcept
	{
	    return ::ntohs(ip.tot_len);
	}

	std::uint16_t identifier() const noexcept
	{
	    return ::ntohs(ip.id);
	}

	void identifier(std::uint16_t other) noexcept
	{
	    ip.id = ::htons(other);
	}

	std::uint8_t flags() const noexcept
	{
	    return ::ntohs(ip.frag_off) & 0b111;
	}

	void flags(std::uint8_t other) noexcept
	{
	    ip.frag_off = ::ntohs(ip.frag_off);

	    ip.frag_off |= other;

	    ip.frag_off = ::htons(ip.frag_off);
	}

	std::uint16_t fragment_offset() const noexcept
	{
	    return ::ntohs(ip.frag_off) >> 3;
	}

	void fragment_offset(std::uint16_t other) noexcept
	{
	    const auto flags = this->flags();

	    ip.frag_off  = other << 3;

	    ip.frag_off |= flags;

	    ip.frag_off = ::htons(ip.frag_off);
	}

	std::uint8_t time_to_live() const noexcept
	{
	    return ip.ttl;
	}

	void time_to_live(std::uint8_t other)
	{
	    if (other < 1)
	    {
		constexpr auto what =
		    "ipv4::time_to_live: minimum time to live is 1";

		throw std::logic_error {what};
	    }

	    ip.ttl = other;
	}

	protocols protocol() const noexcept
	{
	    return protocols {ip.protocol};
	}

	void protocol(protocols protocol) noexcept
	{
	    ip.protocol = static_cast<std::uint8_t>(protocol);
	}

	std::string source_address() const;

	void source_address(std::string_view);

	std::string destination_address() const;

	void destination_address(std::string_view);

	// TODO(#1): options

	inline const std::uint8_t* data() const noexcept;

	std::uint8_t* data() noexcept
	{
	    using const_this = const ipv4*;

	    return const_cast<std::uint8_t*>(const_cast<const_this>(this)->data());
	}

	static std::size_t fill_header(void*,
				       std::size_t,
				       std::uint16_t,
				       std::uint8_t,
				       protocols,
				       std::string_view,
				       std::string_view);

	static std::size_t fill_header(tool::mutable_buffer buffer,
				       std::uint16_t        identifier,
				       std::uint8_t         time_to_live,
				       protocols            protocol,
				       std::string_view     source_address,
				       std::string_view     destination_address)
	{
	    return fill_header(buffer.data(),
			       buffer.size(),
			       identifier,
			       time_to_live,
			       protocol,
			       source_address,
			       destination_address);
	}

	static inline std::string make_header(std::uint16_t,
					      std::uint8_t,
					      protocols,
					      std::string_view,
					      std::string_view);

	consteval static std::size_t minimum_header_length() noexcept
	{
	    return 20;
	}

	static const ipv4& representation(const void*, std::size_t);

	static const ipv4& representation(tool::const_buffer buffer)
	{
	    return representation(buffer.data(), buffer.size());
	}

	static ipv4& representation(void* data, std::size_t size)
	{
	    auto const_data = const_cast<const void*>(data);

	    return const_cast<ipv4&>(representation(const_data, size));
	}

	static ipv4& representation(tool::mutable_buffer buffer)
	{
	    return representation(buffer.data(), buffer.size());
	}

    private:
	::iphdr ip;
    };

    inline void ipv4::header_length(std::size_t size)
    {
	if (size < minimum_header_length())
	{
	    constexpr auto what =
		"ipv4::header_length: size < minimum_header_length()";

	    throw std::logic_error {what};
	}

	if (size % 4)
	{
	    constexpr auto what =
		"ipv4::header_length: size must be multiple of 4";

	    throw std::logic_error {what};
	}

	ip.ihl = size / 4;
    }

    inline const std::uint8_t* ipv4::data() const noexcept
    {
	if (header_length() >= minimum_header_length())
	{
	    return reinterpret_cast<const std::uint8_t*>(this) + header_length();
	}

	return nullptr;
    }

    inline std::string ipv4::make_header(std::uint16_t    identifier,
					 std::uint8_t     time_to_live,
					 protocols        protocol,
					 std::string_view source_address,
					 std::string_view destination_address)
    {
	std::string ip_header(minimum_header_length(), '\0');

	fill_header(ip_header,
		    identifier,
		    time_to_live,
		    protocol,
		    source_address,
		    destination_address);

	return ip_header;
    }
}

#endif // NET_IPV4_HPP__
