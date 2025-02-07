#ifndef NET_ICMP_HPP__
#define NET_ICMP_HPP__

#include <string_view>

#include <cstdint>

#include "tool/mutable_buffer.hpp"

namespace proto::net
{
    class icmp {
    public:
	enum class type_enumerator {
	    echo_replay   = 0,
	    echo          = 8,
	    time_exceeded = 11
	};

	static std::string_view to_string(type_enumerator) noexcept;

	constexpr icmp() noexcept :
	    icmp {0, 0}
	{}

	constexpr icmp(uint8_t type, uint8_t code) noexcept :
	    type_     {type},
	    code_     {code},
	    checksum_ {0}
	{}

	constexpr uint8_t type() const noexcept
	{
	    return type_;
	}

	constexpr void type(uint8_t other) noexcept
	{
	    type_ = other;
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

    private:
	uint8_t  type_;
	uint8_t  code_;
	uint16_t checksum_;
    };

    std::size_t make_icmp_header(void*, std::size_t, uint8_t, uint8_t) noexcept;

    inline std::size_t make_icmp_header(tool::mutable_buffer buffer,
					uint8_t              type,
					uint8_t              code) noexcept
    {
	return make_icmp_header(buffer.data(), buffer.size(), type, code);
    }
}

#endif // NET_ICMP_HPP__
