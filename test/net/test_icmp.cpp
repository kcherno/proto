#define BOOST_TEST_MODULE icmp_test

#include <stdexcept>
#include <array>

#include <boost/test/unit_test.hpp>

#include "net/icmp.hpp"

using namespace proto::net;

BOOST_AUTO_TEST_SUITE(make_header_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, icmp::header_length()> buffer;

    BOOST_CHECK_NO_THROW(icmp::fill_header(buffer, icmp::types::echo_replay, 0));

    auto pointer = reinterpret_cast<const icmp*>(buffer.data());

    BOOST_TEST((pointer->type() == icmp::types::echo_replay));
    BOOST_CHECK_EQUAL(pointer->code(), 0);
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0> buffer;

    BOOST_CHECK_THROW(icmp::fill_header(buffer, icmp::types::echo_replay, 0),
		      std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END();
