#ifndef NET_IPV4_HPP__
#define NET_IPV4_HPP__

#include <cassert>
#include <cstdint>

extern "C"
{

#include <netinet/ip.h>
#include <arpa/inet.h>

}

namespace proto::net
{
    class ipv4 final {
	int version() const noexcept
	{
	    return ip.version;
	}

	void version(int other) noexcept
	{
	    assert(other == 4 && "ip header version must be 4");
	    
	    ip.version = other;
	}

	std::size_t header_length() const noexcept
	{
	    return ip.ihl * 4;
	}

	void header_length(std::size_t other) noexcept
	{
	    assert(other >= 20 && "minimum header length is 20");
	    assert(other <= 60 && "maximum header length is 60");

	    assert((other % 4  == 0) && "ipv4 header length must be multiple of 4");

	    ip.ihl = other / 4;
	}

	// TODO(#1): type of serves

	std::size_t total_length() const noexcept
	{
	    return ::ntohs(ip.tot_len);
	}

	void total_length(std::uint16_t other) noexcept
	{
	    assert(other >= 20 && "minimum total length is 20");

	    ip.tot_len = ::htons(other);
	}

	// TODO(#2): identification, flags, fragment offset

	int time_to_live() const noexcept
	{
	    return ip.ttl;
	}

	void time_to_live(std::uint8_t other) noexcept
	{
	    assert(other > 0 && "minimum time to live is 1");

	    ip.ttl = other;
	}

	int protocol() const noexcept
	{
	    return ip.protocol;
	}

	void protocol(std::uint8_t other) noexcept
	{
	    ip.protocol = other;
	}

	// TODO(#3): checksum, source address, destination address, options

	const char* data() const noexcept
	{
	    return reinterpret_cast<const char*>(this) + header_length();
	}

	char* data() noexcept
	{
	    using const_this = const ipv4*;

	    return const_cast<char*>(const_cast<const_this>(this)->data());
	}

	std::size_t size() const noexcept
	{
	    return total_length() - header_length();
	}

    private:
	::iphdr ip;
    };
}

#endif // NET_IPV4_HPP__
