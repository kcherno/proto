#define BOOST_TEST_MODULE icmp_echo_test

#include <string_view>
#include <string>
#include <array>

#include <cstdint>

#include <boost/test/unit_test.hpp>

#include "tool/const_buffer.hpp"
#include "net/icmp_echo.hpp"

void test_echo_request(proto::tool::const_buffer buffer,
		       uint16_t                  identifier,
		       uint16_t                  sequence_number,
		       std::string_view          message) noexcept
{
    auto pointer =
	reinterpret_cast<const proto::net::icmp_echo*>(buffer.data());

    BOOST_CHECK_EQUAL(pointer->type(),            8);
    BOOST_CHECK_EQUAL(pointer->code(),            0);
    BOOST_CHECK_EQUAL(pointer->identifier(),      identifier);
    BOOST_CHECK_EQUAL(pointer->sequence_number(), sequence_number);

    auto echo_message = std::string_view {
	pointer->data(),
	buffer.size() - proto::net::icmp_echo::header_length()
    };

    BOOST_CHECK_EQUAL(echo_message, message);
}

BOOST_AUTO_TEST_SUITE(make_echo_request_test);

BOOST_AUTO_TEST_SUITE(with_reserved_memory);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, 32> buffer;

    auto mutable_buffer = proto::tool::mutable_buffer {
	buffer.data(),
	proto::net::icmp_echo::header_length()
    };

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer),
		      proto::net::icmp_echo::header_length());

    test_echo_request(mutable_buffer, 0, 0, "");

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer, 1, 1),
		      proto::net::icmp_echo::header_length());

    test_echo_request(mutable_buffer, 1, 1, "");
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0>  small_buffer;
    std::array<char, 8>  medium_buffer;
    std::array<char, 32> large_buffer;

    auto mutable_buffer = proto::tool::mutable_buffer {small_buffer};

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer), 0);

    mutable_buffer = proto::tool::mutable_buffer {medium_buffer};

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer, 65536, 65536),
		      medium_buffer.size());

    test_echo_request(mutable_buffer, 0, 0, "");

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer, -1, -1),
		      medium_buffer.size());

    test_echo_request(mutable_buffer, 65535, 65535, "");

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer,
						    0,
						    0,
						    "0123456789"),
		      medium_buffer.size());

    test_echo_request(mutable_buffer, 0, 0, "");

    mutable_buffer = proto::tool::mutable_buffer {large_buffer};

    BOOST_CHECK_EQUAL(proto::net::make_echo_request(mutable_buffer,
						    0,
						    0,
						    "abcdefghijklmnopqrstuvwxyz"),
		      large_buffer.size());

    test_echo_request(mutable_buffer, 0, 0, "abcdefghijklmnopqrstuvwx");
}
BOOST_AUTO_TEST_SUITE_END(); // with_reserved_memory

BOOST_AUTO_TEST_SUITE(with_dynamic_memory);
BOOST_AUTO_TEST_CASE(success)
{
    std::string echo_request;

    echo_request = proto::net::make_echo_request();

    test_echo_request(echo_request, 0, 0, "");

    echo_request = proto::net::make_echo_request(1, 1);

    test_echo_request(echo_request, 1, 1, "");

    echo_request = proto::net::make_echo_request(0, 0, "0123456789");

    test_echo_request(echo_request, 0, 0, "0123456789");
}
BOOST_AUTO_TEST_SUITE_END(); // with_dynamic_memory

BOOST_AUTO_TEST_SUITE_END(); // make_echo_request_test
