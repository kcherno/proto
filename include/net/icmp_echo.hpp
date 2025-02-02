#ifndef NET_ICMP_ECHO_HPP__
#define NET_ICMP_ECHO_HPP__

#include <string_view>
#include <cstdint>
#include <string>

extern "C"
{

#include <arpa/inet.h>

}

#include "tool/mutable_buffer.hpp"
#include "net/icmp.hpp"

namespace proto::net
{
    class icmp_echo final : public icmp {
    public:
	icmp_echo() noexcept :
	    icmp_echo {0, 0}
	{}

	inline icmp_echo(uint16_t, uint16_t) noexcept;

	uint16_t identifier() const noexcept
	{
	    return ::ntohs(identifier_);
	}

	void identifier(uint16_t other) noexcept
	{
	    identifier_ = ::htons(other);
	}

	uint16_t sequence_number() const noexcept
	{
	    return ::ntohs(sequence_number_);
	}

	void sequence_number(uint16_t other) noexcept
	{
	    sequence_number_ = ::htons(other);
	}

	inline const char* data() const noexcept;

	char* data() noexcept
	{
	    using const_this = const icmp_echo*;

	    return const_cast<char*>(const_cast<const_this>(this)->data());
	}

	consteval static std::size_t header_length() noexcept
	{
	    return sizeof(icmp_echo);
	}

    private:
	uint16_t identifier_;
	uint16_t sequence_number_;
    };

    inline icmp_echo::icmp_echo(uint16_t identifier,
				uint16_t sequence_number) noexcept :
	icmp             {8, 0},
	identifier_      {::htons(identifier)},
	sequence_number_ {::htons(sequence_number)}
    {
	calculate_checksum(header_length());
    }

    inline const char* icmp_echo::data() const noexcept
    {
	return reinterpret_cast<const char*>(this) + header_length();
    }

    /////////////////////////////////////////////////////////////////////////////////

    std::size_t make_echo_request(void*,
				  std::size_t,
				  uint16_t,
				  uint16_t,
				  std::string_view) noexcept;

    inline std::size_t make_echo_request(void*       buffer,
					 std::size_t size,
					 uint16_t    identifier,
					 uint16_t    sequence_number) noexcept
    {
	return make_echo_request(buffer, size, identifier, sequence_number, "");
    }

    inline std::size_t make_echo_request(void* buffer, std::size_t size) noexcept
    {
	return make_echo_request(buffer, size, 0, 0);
    }

    /////////////////////////////////////////////////////////////////////////////////

    inline std::size_t make_echo_request(tool::mutable_buffer buffer,
					 uint16_t             identifier,
					 uint16_t             sequence_number,
					 std::string_view     message) noexcept
    {
	return make_echo_request(buffer.data(),
				 buffer.size(),
				 identifier,
				 sequence_number,
				 message);
    }

    inline std::size_t make_echo_request(tool::mutable_buffer buffer,
					 uint16_t             identifier,
					 uint16_t             sequence_number)
	noexcept
    {
	return make_echo_request(buffer, identifier, sequence_number, "");
    }

    inline std::size_t make_echo_request(tool::mutable_buffer buffer) noexcept
    {
	return make_echo_request(buffer, 0, 0);
    }

    /////////////////////////////////////////////////////////////////////////////////

    inline std::string make_echo_request(uint16_t         identifier,
					 uint16_t         sequence_number,
					 std::string_view message)
    {
	std::string echo_request(icmp_echo::header_length() + message.size(), '\0');

	make_echo_request(echo_request, identifier, sequence_number, message);

	return echo_request;
    }

    inline std::string make_echo_request(uint16_t identifier,
					 uint16_t sequence_number)
    {
	return make_echo_request(identifier, sequence_number, "");
    }

    inline std::string make_echo_request()
    {
	return make_echo_request(uint16_t {}, uint16_t {});
    }
}

#endif // NET_ICMP_ECHO_HPP__
