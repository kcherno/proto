#ifndef TOOL_CONST_BUFFER_HPP__
#define TOOL_CONST_BUFFER_HPP__

#include <stdexcept>

#include "name_requirement/contiguous_container.hpp"

namespace proto::tool
{
    class const_buffer {
    public:
	constexpr const_buffer() noexcept :
	    const_buffer {nullptr, 0}
	{}

	constexpr const_buffer(const void* data, std::size_t size) noexcept :
	    data_ {data},
	    size_ {size}
	{}

	template<name_requirement::ContiguousContainer C>
	const_buffer(const C& container) noexcept :
	    data_ {
		container.empty() ? nullptr : &*container.cbegin()
	    },

	    size_ {container.size()}
	{}

	const void* data() const noexcept
	{
	    return data_;
	}

	constexpr bool empty() const noexcept
	{
	    return size() == 0;
	}

	constexpr std::size_t size() const noexcept
	{
	    return size_;
	}

	const_buffer subbuffer(std::size_t n, std::size_t start = 0) const
	{
	    if (start != 0)
	    {
		--start;
	    }

	    if (empty() || ((start + n) > size()))
	    {
		constexpr auto error =
		    "const_buffer::subbuffer(n, start): start + n > size";

		throw std::out_of_range {error};
	    }

	    return const_buffer {
		reinterpret_cast<const char*>(data()) + start,
		n
	    };
	}

    private:
	const mutable void* data_;
	std::size_t         size_;
    };
}

#endif // TOOL_CONST_BUFFER_HPP__
