#define BOOST_TEST_MODULE icmp_echo_test

#include <string_view>
#include <stdexcept>
#include <string>
#include <array>

#include <cstdint>

#include <boost/test/unit_test.hpp>

#include "tool/const_buffer.hpp"
#include "net/icmp_echo.hpp"

using namespace proto::net;
using namespace proto;

std::string_view get_message(const void* data, std::size_t size) noexcept
{
    if (size < icmp_echo::header_length())
    {
	BOOST_REQUIRE(false);
    }

    const auto& icmp_echo = icmp_echo::representation(data, size);

    return std::string_view {
	reinterpret_cast<const char*>(icmp_echo.data()),
	size - icmp_echo::header_length()
    };
}

std::string_view get_message(tool::const_buffer buffer) noexcept
{
    return get_message(buffer.data(), buffer.size());
}

BOOST_AUTO_TEST_SUITE(icmp_echo_replay_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, icmp_echo::header_length()> buffer;

    auto size = icmp_echo::fill_echo_replay(buffer, 0, 0, "");

    BOOST_CHECK_EQUAL(size, icmp_echo::header_length());

    const auto& view_1 = icmp_echo::representation(tool::const_buffer {buffer});

    BOOST_TEST((view_1.type() == icmp::types::echo_replay));

    BOOST_CHECK_EQUAL(view_1.code(),               0);
    BOOST_CHECK_EQUAL(view_1.identifier(),         0);
    BOOST_CHECK_EQUAL(view_1.sequence_number(),    0);
    BOOST_CHECK_EQUAL(get_message(buffer).empty(), true);

    size = icmp_echo::fill_echo_replay(buffer, 0, 0, "0123456789");

    BOOST_CHECK_EQUAL(size, icmp_echo::header_length());

    const auto& view_2 = icmp_echo::representation(tool::const_buffer {buffer});

    BOOST_TEST((view_2.type() == icmp::types::echo_replay));

    BOOST_CHECK_EQUAL(view_2.code(),               0);
    BOOST_CHECK_EQUAL(view_2.identifier(),         0);
    BOOST_CHECK_EQUAL(view_2.sequence_number(),    0);
    BOOST_CHECK_EQUAL(get_message(buffer).empty(), true);

    /////////////////////////////////////////////////////////////////////////////////

    auto string_1 = icmp_echo::make_echo_replay(0, 0, "");

    BOOST_CHECK_EQUAL(string_1.size(), icmp_echo::header_length());

    const auto& view_3 =
	icmp_echo::representation(tool::const_buffer {string_1});

    BOOST_TEST((view_3.type() == icmp::types::echo_replay));

    BOOST_CHECK_EQUAL(view_3.code(),                 0);
    BOOST_CHECK_EQUAL(view_3.identifier(),           0);
    BOOST_CHECK_EQUAL(view_3.sequence_number(),      0);
    BOOST_CHECK_EQUAL(get_message(string_1).empty(), true);

    auto string_2 = icmp_echo::make_echo_replay(0, 0, "0123456789");

    BOOST_CHECK_EQUAL(string_2.size(),
		      icmp_echo::header_length() +
		      std::string_view("0123456789").size());

    const auto& view_4 =
	icmp_echo::representation(tool::const_buffer {string_2});

    BOOST_TEST((view_4.type() == icmp::types::echo_replay));

    BOOST_CHECK_EQUAL(view_4.code(),            0);
    BOOST_CHECK_EQUAL(view_4.identifier(),      0);
    BOOST_CHECK_EQUAL(view_4.sequence_number(), 0);
    BOOST_CHECK_EQUAL(get_message(string_2),    "0123456789");
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0> buffer;

    BOOST_CHECK_THROW(icmp_echo::fill_echo_replay(buffer, 0, 0, ""), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(icmp_echo_representation_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, icmp_echo::header_length()> buffer;

    BOOST_CHECK_NO_THROW(icmp_echo::representation(tool::const_buffer   {buffer}));
    BOOST_CHECK_NO_THROW(icmp_echo::representation(tool::mutable_buffer {buffer}));
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0> buffer;

    BOOST_CHECK_THROW(icmp_echo::representation(tool::const_buffer   {buffer}),
		      std::out_of_range);

    BOOST_CHECK_THROW(icmp_echo::representation(tool::mutable_buffer {buffer}),
		      std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(icmp_echo_request_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, icmp_echo::header_length()> buffer;

    auto size = icmp_echo::fill_echo_request(buffer, 0, 0, "");

    BOOST_CHECK_EQUAL(size, icmp_echo::header_length());

    const auto& view_1 = icmp_echo::representation(tool::const_buffer {buffer});

    BOOST_TEST((view_1.type() == icmp::types::echo_request));

    BOOST_CHECK_EQUAL(view_1.code(),               0);
    BOOST_CHECK_EQUAL(view_1.identifier(),         0);
    BOOST_CHECK_EQUAL(view_1.sequence_number(),    0);
    BOOST_CHECK_EQUAL(get_message(buffer).empty(), true);

    size = icmp_echo::fill_echo_request(buffer, 0, 0, "0123456789");

    BOOST_CHECK_EQUAL(size, icmp_echo::header_length());

    const auto& view_2 = icmp_echo::representation(tool::const_buffer {buffer});

    BOOST_TEST((view_2.type() == icmp::types::echo_request));

    BOOST_CHECK_EQUAL(view_2.code(),               0);
    BOOST_CHECK_EQUAL(view_2.identifier(),         0);
    BOOST_CHECK_EQUAL(view_2.sequence_number(),    0);
    BOOST_CHECK_EQUAL(get_message(buffer).empty(), true);

    /////////////////////////////////////////////////////////////////////////////////

    auto string_1 = icmp_echo::make_echo_request(0, 0, "");

    BOOST_CHECK_EQUAL(string_1.size(), icmp_echo::header_length());

    const auto& view_3 =
	icmp_echo::representation(tool::const_buffer {string_1});

    BOOST_TEST((view_3.type() == icmp::types::echo_request));

    BOOST_CHECK_EQUAL(view_3.code(),                 0);
    BOOST_CHECK_EQUAL(view_3.identifier(),           0);
    BOOST_CHECK_EQUAL(view_3.sequence_number(),      0);
    BOOST_CHECK_EQUAL(get_message(string_1).empty(), true);

    auto string_2 = icmp_echo::make_echo_request(0, 0, "0123456789");

    BOOST_CHECK_EQUAL(string_2.size(),
		      icmp_echo::header_length() +
		      std::string_view("0123456789").size());

    const auto& view_4 =
	icmp_echo::representation(tool::const_buffer {string_2});

    BOOST_TEST((view_4.type() == icmp::types::echo_request));

    BOOST_CHECK_EQUAL(view_4.code(),            0);
    BOOST_CHECK_EQUAL(view_4.identifier(),      0);
    BOOST_CHECK_EQUAL(view_4.sequence_number(), 0);
    BOOST_CHECK_EQUAL(get_message(string_2),    "0123456789");
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0> buffer;

    BOOST_CHECK_THROW(icmp_echo::fill_echo_request(buffer, 0, 0, ""), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END();
