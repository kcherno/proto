#define BOOST_TEST_MODULE icmp_echo_test

#include <string_view>
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

    BOOST_REQUIRE_EQUAL(pointer->type(),            8);
    BOOST_REQUIRE_EQUAL(pointer->code(),            0);
    BOOST_REQUIRE_EQUAL(pointer->identifier(),      identifier);
    BOOST_REQUIRE_EQUAL(pointer->sequence_number(), sequence_number);

    auto echo_message = std::string_view {
	pointer->data(),
	buffer.size() - proto::net::icmp_echo::header_length()
    };

    BOOST_REQUIRE_EQUAL(echo_message, message);
}

BOOST_AUTO_TEST_SUITE(make_echo_request_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::string echo_request;

    echo_request = proto::net::make_echo_request();

    test_echo_request(echo_request, 0, 0, "");

    echo_request = proto::net::make_echo_request(1, 1);

    test_echo_request(echo_request, 1, 1, "");

    echo_request = proto::net::make_echo_request(0, 0, "ECHO");

    test_echo_request(echo_request, 0, 0, "ECHO");
}
BOOST_AUTO_TEST_SUITE_END();
