#ifndef CONTAINER_NAME_REQUIREMENT_HPP__
#define CONTAINER_NAME_REQUIREMENT_HPP__

#include <concepts>
#include <iterator>
#include <compare>

namespace proto::name_requirement::detail__
{
    template<typename Container>
    struct strong_ordering_if_random_access_iterator final {
	constexpr static bool value =
	    std::random_access_iterator<typename Container::iterator> ?
	    std::totally_ordered<Container> :
	    true;
    };

    template<typename Container>
    constexpr inline bool strong_ordering_if_random_access_iterator_v =
	strong_ordering_if_random_access_iterator<Container>::value;
}

namespace proto::name_requirement
{
    template<typename C>
    concept Container = requires(C a, C b, const C c) {

	typename C::value_type;
	typename C::reference;
	typename C::const_reference;
	typename C::iterator;
	typename C::const_iterator;
	typename C::difference_type;
	typename C::size_type;

	{ C()    } -> std::same_as<C>;
	{ C(a)   } -> std::same_as<C>;
	{ a = b  } -> std::same_as<C&>;
	{ a.~C() } -> std::same_as<void>;

	{ a.begin() } -> std::same_as<typename C::iterator>;
	{ c.begin() } -> std::same_as<typename C::const_iterator>;
	{ a.end()   } -> std::same_as<typename C::iterator>;
	{ c.end()   } -> std::same_as<typename C::const_iterator>;

	{ a.cbegin() } -> std::same_as<typename C::const_iterator>;
	{ a.cend()   } -> std::same_as<typename C::const_iterator>;

	{ a == b } -> std::same_as<bool>;
	{ a != b } -> std::same_as<bool>;

	{ a.swap(b) } -> std::same_as<void>;

	{ a.size()     } -> std::same_as<typename C::size_type>;
	{ a.max_size() } -> std::same_as<typename C::size_type>;
	{ a.empty()    } -> std::same_as<bool>;

    } && detail__::strong_ordering_if_random_access_iterator_v<C>;
}

#endif // CONTAINER_NAME_REQUIREMENT_HPP__
