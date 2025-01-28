#define BOOST_TEST_MODULE contiguous_container_name_requirement_test

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <map>
#include <set>

#include <boost/test/unit_test.hpp>

#include "name_requirement/contiguous_container.hpp"

using namespace proto::name_requirement;

BOOST_AUTO_TEST_SUITE(contiguous_container_name_requirement);
BOOST_AUTO_TEST_CASE(success)
{
    static_assert(ContiguousContainer<std::string>);
    static_assert(ContiguousContainer<std::vector<int>>);
    static_assert(ContiguousContainer<std::array<int, 0>>);

    BOOST_TEST((ContiguousContainer<std::string>));
    BOOST_TEST((ContiguousContainer<std::vector<int>>));
    BOOST_TEST((ContiguousContainer<std::array<int, 0>>));
}

BOOST_AUTO_TEST_CASE(failure)
{
    static_assert(not ContiguousContainer<std::unordered_multimap<int, int>>);
    static_assert(not ContiguousContainer<std::unordered_multiset<int, int>>);
    static_assert(not ContiguousContainer<std::unordered_map<int, int>>);
    static_assert(not ContiguousContainer<std::unordered_set<int, int>>);
    static_assert(not ContiguousContainer<std::multimap<int, int>>);
    static_assert(not ContiguousContainer<std::multiset<int, int>>);
    static_assert(not ContiguousContainer<std::deque<int>>);
    static_assert(not ContiguousContainer<std::list<int>>);
    static_assert(not ContiguousContainer<std::map<int, int>>);
    static_assert(not ContiguousContainer<std::set<int>>);

    BOOST_TEST(not (ContiguousContainer<std::unordered_multimap<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::unordered_multiset<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::unordered_map<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::unordered_set<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::multimap<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::multiset<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::deque<int>>));
    BOOST_TEST(not (ContiguousContainer<std::list<int>>));
    BOOST_TEST(not (ContiguousContainer<std::map<int, int>>));
    BOOST_TEST(not (ContiguousContainer<std::set<int>>));
}
BOOST_AUTO_TEST_SUITE_END();
