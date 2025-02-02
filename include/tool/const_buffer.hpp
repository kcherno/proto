#ifndef TOOL_CONST_BUFFER_HPP__
#define TOOL_CONST_BUFFER_HPP__

#include "name_requirement/contiguous_container.hpp"

namespace proto::tool
{
    class const_buffer {
    public:
	constexpr const_buffer() noexcept :
	    const_buffer {nullptr, 0}
	{}

	constexpr const_buffer(void* data, std::size_t size) noexcept :
	    data_ {data},
	    size_ {size}
	{}

	template<name_requirement::ContiguousContainer C>
	const_buffer(C& container) noexcept :
	    data_ {
		container.empty() ?
		nullptr :
		reinterpret_cast<const void*>(&*container.cbegin())
	    },

	    size_ {container.size()}
	{}

	const void* data() const noexcept
	{
	    return data_;
	}

	constexpr std::size_t size() const noexcept
	{
	    return size_;
	}

    private:
	const void*       data_;
	const std::size_t size_;
    };
}

#endif // TOOL_CONST_BUFFER_HPP__
