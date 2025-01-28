#ifndef CONTIGUOUS_CONTAINER_NAME_REQUIREMENT_HPP__
#define CONTIGUOUS_CONTAINER_NAME_REQUIREMENT_HPP__

#include <iterator>

#include "container.hpp"

namespace proto::name_requirement
{
    template<typename T>
    concept ContiguousContainer =
	Container<T>                                   &&
	std::contiguous_iterator<typename T::iterator> &&
	std::contiguous_iterator<typename T::const_iterator>;
}

#endif // CONTIGUOUS_CONTAINER_NAME_REQUIREMENT_HPP__
