#ifndef TOOL_MUTABLE_BUFFER_HPP__
#define TOOL_MUTABLE_BUFFER_HPP__

#include "name_requirement/contiguous_container.hpp"
#include "tool/const_buffer.hpp"

namespace proto::tool
{
    class mutable_buffer final : public const_buffer {
    public:
	constexpr mutable_buffer() noexcept :
	    const_buffer {}
	{}

	constexpr mutable_buffer(void* data, std::size_t size) noexcept :
	    const_buffer {data, size}
	{}

	template<name_requirement::ContiguousContainer C>
	mutable_buffer(C& container) noexcept :
	    const_buffer {container}
	{}

	void* data() noexcept
	{
	    using const_this = const mutable_buffer*;

	    return const_cast<void*>(const_cast<const_this>(this)->data());
	}

	mutable_buffer subbuffer(std::size_t n, std::size_t start = 0) const
	{
	    return const_buffer::subbuffer(n, start);
	}

    private:
	mutable_buffer(const_buffer buffer) noexcept :
	    const_buffer {buffer}
	{}

	const void* data() const noexcept
	{
	    return const_buffer::data();
	}
    };
}

#endif // TOOL_MUTABLE_BUFFER_HPP__
