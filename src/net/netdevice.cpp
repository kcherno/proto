#include <system_error>
#include <memory>
#include <vector>

#include <cerrno>

extern "C"
{

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

}

#include <boost/asio.hpp>

#include "net/netdevice.hpp"

namespace proto::net
{
    boost::asio::ip::address_v4 netdevice::address_v4()
    {
	std::error_code error;

	if (auto address = address_v4(error); error)
	{
	    throw std::system_error {error, "netdevice::address_v4"};
	}

	else
	{
	    return address;
	}
    }

    boost::asio::ip::address_v4 netdevice::address_v4(std::error_code& error) noexcept
    {
	if (active_field != field::address_v4)
	{
	    const auto error_holder = error;

	    if (request(SIOCGIFADDR, error); error)
	    {
		if (error == std::errc::address_not_available)
		{
		    error = error_holder;
		}

		return boost::asio::ip::address_v4 {};
	    }

	    else
	    {
		active_field = field::address_v4;
	    }
	}

	auto address_v4 = boost::asio::ip::address_v4 {
	    ::ntohl(reinterpret_cast<const sockaddr_in*>(&ifreq.ifr_addr)->sin_addr.s_addr)
	};

	return address_v4;
    }

    std::vector<netdevice> netdevice::available()
    {
	std::vector<netdevice> netdevices;

	auto deleter = [](auto&& pointer)
	{
	    ::if_freenameindex(pointer);
	};

	auto if_nameindex = std::unique_ptr<struct ::if_nameindex, decltype(deleter)> {
	    ::if_nameindex(),
	    deleter
	};

	if (if_nameindex == nullptr)
	{
	    throw std::system_error {std::make_error_code(std::errc {errno}), "netdevice::available"};
	}

	for (auto pointer = if_nameindex.get(); pointer->if_index; ++pointer)
	{
	    netdevices.emplace_back(netdevice {pointer->if_name /*, pointer->if_index */});
	}

	return netdevices;
    }

    void netdevice::request(int req, std::error_code& error) noexcept
    {
	auto deleter = [](const int* sockfd)
	{
	    if (*sockfd != -1)
	    {
		::close(*sockfd);
	    }

	    delete sockfd;
	};

	thread_local std::unique_ptr<int, decltype(deleter)> helper_socket;

	if (helper_socket == nullptr)
	{
	    helper_socket = decltype(helper_socket) {
		new int {::socket(AF_INET, SOCK_RAW, IPPROTO_RAW)},
		deleter
	    };

	    if (*helper_socket == -1)
	    {
		helper_socket.reset();

		error = std::make_error_code(std::errc {errno});

		return;
	    }
	}

	switch (req)
	{
	case SIOCGIFADDR:
	    if (::ioctl(*helper_socket, req, &ifreq) == -1)
	    {
		error = std::make_error_code(std::errc {errno});
	    }

	    break;

	case SIOCGIFFLAGS:
	    if (::ioctl(*helper_socket, req, &ifreq) == -1)
	    {
		error = std::make_error_code(std::errc {errno});
	    }

	    break;
	}
    }
}
