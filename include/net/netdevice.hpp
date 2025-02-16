#pragma once

#include <system_error>
#include <string_view>
#include <vector>

#include <cstring>
#include <cerrno>

extern "C"
{

#include <net/if.h>

}

#include <boost/asio.hpp>

namespace proto::net
{
    class netdevice final {
    public:
	boost::asio::ip::address_v4 address_v4();

	boost::asio::ip::address_v4 address_v4(std::error_code&) noexcept;

	bool is_loopback()
	{
	    std::error_code error;

	    if (set_flags(error); error)
	    {
		throw std::system_error {error, "netdevice::is_loopback"};
	    }

	    return ifreq.ifr_flags & IFF_LOOPBACK;
	}

	bool is_running()
	{
	    std::error_code error;

	    if (set_flags(error); error)
	    {
		throw std::system_error {error, "netdevice::is_running"};
	    }

	    return ifreq.ifr_flags & IFF_UP;
	}

	std::string_view name() const noexcept
	{
	    return ifreq.ifr_name;
	}

	static std::vector<netdevice> available();

    private:
	enum class field {
	    none = 0,

	    flags,

	    // extendend flags

	    address_v4
	 // address_v6

	    // destination address
	    // broadcast address
	    // network mask
	    // metric
	    // mtu
	};
	
	netdevice(std::string_view name /* std::size_t index */) noexcept :
	    ifreq        {},
	 // in6_ifreq    {.ifr6_addr = {}, .ifr6_prefixlen = {}, .ifr6_ifindex = index},
	    active_field {field::none}
	{
	    std::strncpy(ifreq.ifr_name, name.data(), IF_NAMESIZE);
	}

	void request(int, std::error_code&) noexcept;

	void set_flags(std::error_code& error) noexcept
	{
	    if (active_field != field::flags)
	    {
		if (request(SIOCGIFFLAGS, error); error)
		{
		    return;
		}

		active_field = field::flags;
	    }
	}

	::ifreq     ifreq;
     // ::in6_ifreq in6_ifreq;

	field active_field;
    };

    inline boost::asio::ip::address_v4 host_address_v4()
    {
	for (auto netdevices = netdevice::available(); auto&& netdevice : netdevices)
	{
	    if (netdevice.is_running() && not netdevice.is_loopback())
	    {
		auto address_v4 = netdevice.address_v4();

		if (not address_v4.is_unspecified())
		{
		    return address_v4;
		}
	    }
	}

	return boost::asio::ip::address_v4 {};
    }
}
