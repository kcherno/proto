#define BOOST_TEST_MODULE ipv4_test

#include <stdexcept>
#include <array>

#include <boost/test/unit_test.hpp>

#include "net/ipv4.hpp"

using namespace proto::net;

BOOST_AUTO_TEST_SUITE(fill_header_test);
BOOST_AUTO_TEST_CASE(success)
{
    std::array<char, ipv4::minimum_header_length()> buffer;

    ipv4::fill_header(buffer,
		      12345,
		      64,
		      ipv4::protocols::icmp,
		      "192.168.0.0",
		      "196.100.0.0");

    const auto& ipv4 = ipv4::representation(proto::tool::const_buffer {buffer});

    BOOST_CHECK_EQUAL(ipv4.version(),         4);
    BOOST_CHECK_EQUAL(ipv4.header_length(),   ipv4::minimum_header_length());
    BOOST_CHECK_EQUAL(ipv4.type_of_service(), 0);
    BOOST_CHECK_EQUAL(ipv4.total_length(),    ipv4::minimum_header_length());
    BOOST_CHECK_EQUAL(ipv4.identifier(),      12345);
    BOOST_CHECK_EQUAL(ipv4.flags(),           0);
    BOOST_CHECK_EQUAL(ipv4.fragment_offset(), 0);
    BOOST_CHECK_EQUAL(ipv4.time_to_live(),    64);

    BOOST_TEST((ipv4.protocol() == ipv4::protocols::icmp));

    BOOST_CHECK_EQUAL(ipv4.source_address(),      "192.168.0.0");
    BOOST_CHECK_EQUAL(ipv4.destination_address(), "196.100.0.0");

    BOOST_CHECK_EQUAL(ipv4.data(),  nullptr);
    BOOST_CHECK_EQUAL(ipv4.empty(), true);
    BOOST_CHECK_EQUAL(ipv4.size(),  0);
}

BOOST_AUTO_TEST_CASE(failure)
{
    std::array<char, 0> buffer;

    BOOST_CHECK_THROW(ipv4::fill_header(buffer, 0, 64, ipv4::protocols::icmp, "", ""),
		      std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(make_header_test);
BOOST_AUTO_TEST_CASE(success)
{
    const auto ip_header =
	ipv4::make_header(12345,
			  64,
			  ipv4::protocols::icmp,
			  "192.168.0.0",
			  "196.100.0.0");

    BOOST_CHECK_EQUAL(ip_header.size(), ipv4::minimum_header_length());

    const auto& ipv4 = ipv4::representation(ip_header);

    BOOST_CHECK_EQUAL(ipv4.version(),         4);
    BOOST_CHECK_EQUAL(ipv4.header_length(),   ipv4::minimum_header_length());
    BOOST_CHECK_EQUAL(ipv4.type_of_service(), 0);
    BOOST_CHECK_EQUAL(ipv4.total_length(),    ipv4::minimum_header_length());
    BOOST_CHECK_EQUAL(ipv4.identifier(),      12345);
    BOOST_CHECK_EQUAL(ipv4.flags(),           0);
    BOOST_CHECK_EQUAL(ipv4.fragment_offset(), 0);
    BOOST_CHECK_EQUAL(ipv4.time_to_live(),    64);

    BOOST_TEST((ipv4.protocol() == ipv4::protocols::icmp));

    BOOST_CHECK_EQUAL(ipv4.source_address(),      "192.168.0.0");
    BOOST_CHECK_EQUAL(ipv4.destination_address(), "196.100.0.0");

    BOOST_CHECK_EQUAL(ipv4.data(),  nullptr);
    BOOST_CHECK_EQUAL(ipv4.empty(), true);
    BOOST_CHECK_EQUAL(ipv4.size(),  0);
}
BOOST_AUTO_TEST_SUITE_END();
