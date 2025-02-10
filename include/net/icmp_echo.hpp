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

	inline const uint8_t* data() const noexcept;

	uint8_t* data() noexcept
	{
	    using const_this = const icmp_echo*;

	    return const_cast<uint8_t*>(const_cast<const_this>(this)->data());
	}

	consteval static std::size_t header_length() noexcept
	{
	    return sizeof(icmp_echo);
	}

	static std::size_t replay(void*,
				  std::size_t,
				  uint16_t,
				  uint16_t,
				  std::string_view);

	static std::size_t replay(tool::mutable_buffer buffer,
				  uint16_t             identifier,
				  uint16_t             sequence_number,
				  std::string_view     message)
	{
	    return replay(buffer.data(),
			  buffer.size(),
			  identifier,
			  sequence_number,
			  message);
	}

	static std::string replay(uint16_t             identifier,
				  uint16_t             sequence_number,
				  std::string_view     message)
	{
	    std::string echo_replay(icmp_echo::header_length() + message.size(), '\0');

	    replay(echo_replay, identifier, sequence_number, message);

	    return echo_replay;
	}

	static const icmp_echo& representation(const void*, std::size_t);

	static const icmp_echo& representation(tool::const_buffer buffer)
	{
	    return representation(buffer.data(), buffer.size());
	}

	static icmp_echo& representation(void* data, std::size_t size)
	{
	    auto const_data = const_cast<const void*>(data);

	    return const_cast<icmp_echo&>(representation(const_data, size));
	}

	static icmp_echo& representation(tool::mutable_buffer buffer)
	{
	    return representation(buffer.data(), buffer.size());
	}

	static std::size_t request(void*,
				   std::size_t,
				   uint16_t,
				   uint16_t,
				   std::string_view);

	static std::size_t request(void*       data,
				   std::size_t size,
				   uint16_t    identifier,
				   uint16_t    sequence_number)
	{
	    return request(data, size, identifier, sequence_number, "");
	}

	static std::size_t request(void* data, std::size_t size)
	{
	    return request(data, size, 0, 0);
	}

	static std::size_t request(tool::mutable_buffer buffer,
				   uint16_t             identifier,
				   uint16_t             sequence_number,
				   std::string_view     message)
	{
	    return request(buffer.data(),
			   buffer.size(),
			   identifier,
			   sequence_number,
			   message);
	}

	static std::size_t request(tool::mutable_buffer buffer,
				   uint16_t             identifier,
				   uint16_t             sequence_number)
	{
	    return request(buffer, identifier, sequence_number, "");
	}

	static std::size_t request(tool::mutable_buffer buffer)
	{
	    return request(buffer, 0, 0);
	}

	static std::string request(uint16_t         identifier,
				   uint16_t         sequence_number,
				   std::string_view message)
	{
	    std::string echo_request(icmp_echo::header_length() + message.size(), '\0');

	    request(echo_request, identifier, sequence_number, message);

	    return echo_request;
	}

	static std::string request(uint16_t identifier,
				   uint16_t sequence_number)
	{
	    return request(identifier, sequence_number, "");
	}

	static std::string request()
	{
	    return request(uint16_t {}, uint16_t {});
	}

    private:
	uint16_t identifier_;
	uint16_t sequence_number_;
    };

    inline icmp_echo::icmp_echo(uint16_t identifier,
				uint16_t sequence_number) noexcept :
	icmp             {types::echo_request, 0},
	identifier_      {::htons(identifier)},
	sequence_number_ {::htons(sequence_number)}
    {
	calculate_checksum(header_length());
    }

    inline const uint8_t* icmp_echo::data() const noexcept
    {
	return reinterpret_cast<const uint8_t*>(this) + header_length();
    }
}

#endif // NET_ICMP_ECHO_HPP__
