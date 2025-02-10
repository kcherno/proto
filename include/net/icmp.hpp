#ifndef NET_ICMP_HPP__
#define NET_ICMP_HPP__

#include <string_view>

#include <cstdint>

#include "tool/mutable_buffer.hpp"

namespace proto::net
{
    class icmp {
    public:
	enum class types {
	    echo_replay   = 0,
	    echo_request  = 8,
	    time_exceeded = 11
	};

	constexpr static std::string_view to_string(types) noexcept;

	constexpr icmp(types type, uint8_t code) noexcept :
	    type_     {static_cast<uint8_t>(type)},
	    code_     {code},
	    checksum_ {0}
	{}

	constexpr types type() const noexcept
	{
	    return types {type_};
	}

	constexpr void type(types other) noexcept
	{
	    type_ = static_cast<uint8_t>(other);
	}

	constexpr uint8_t code() const noexcept
	{
	    return code_;
	}

	constexpr void code(uint8_t other) noexcept
	{
	    code_ = other;
	}

	void calculate_checksum(std::size_t) noexcept;

	consteval static std::size_t header_length() noexcept
	{
	    return sizeof(icmp);
	}

	static std::size_t fill_header(void*, std::size_t, types, uint8_t);

	static std::size_t fill_header(tool::mutable_buffer buffer,
				       types                type,
				       uint8_t              code)
	{
	    return fill_header(buffer.data(), buffer.size(), type, code);
	}

    private:
	uint8_t  type_;
	uint8_t  code_;
	uint16_t checksum_;
    };

    constexpr std::string_view icmp::to_string(icmp::types type) noexcept
    {
	using enum types;

	switch (type)
	{
	case echo_replay:   return "echo replay message";
	case echo_request:  return "echo message";
	case time_exceeded: return "time exceeded message";

	default: return "unspecified";
	}
    }
}

#endif // NET_ICMP_HPP__
