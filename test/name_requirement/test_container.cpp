#define BOOST_TEST_MODULE container_name_requirement_test

#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <span>
#include <map>
#include <set>

#include <boost/test/unit_test.hpp>

#include "name_requirement/container.hpp"

using namespace proto::name_requirement;

BOOST_AUTO_TEST_SUITE(container_name_requirement);
BOOST_AUTO_TEST_CASE(success)
{
    static_assert(Container<std::unordered_multimap<int, int>>);
    static_assert(Container<std::unordered_multiset<int, int>>);
    static_assert(Container<std::unordered_map<int, int>>);
    static_assert(Container<std::unordered_set<int, int>>);
    static_assert(Container<std::multimap<int, int>>);
    static_assert(Container<std::multiset<int, int>>);
    static_assert(Container<std::string>);
    static_assert(Container<std::vector<int>>);
    static_assert(Container<std::array<int, 0>>);
    static_assert(Container<std::deque<int>>);
    static_assert(Container<std::list<int>>);
    static_assert(Container<std::map<int, int>>);
    static_assert(Container<std::set<int>>);

    BOOST_TEST((Container<std::unordered_multimap<int, int>>));
    BOOST_TEST((Container<std::unordered_multiset<int, int>>));
    BOOST_TEST((Container<std::unordered_map<int, int>>));
    BOOST_TEST((Container<std::unordered_set<int, int>>));
    BOOST_TEST((Container<std::multimap<int, int>>));
    BOOST_TEST((Container<std::multiset<int, int>>));
    BOOST_TEST((Container<std::string>));
    BOOST_TEST((Container<std::vector<int>>));
    BOOST_TEST((Container<std::array<int, 0>>));
    BOOST_TEST((Container<std::deque<int>>));
    BOOST_TEST((Container<std::list<int>>));
    BOOST_TEST((Container<std::map<int, int>>));
    BOOST_TEST((Container<std::set<int>>));
}

BOOST_AUTO_TEST_CASE(failure)
{
    static_assert(not Container<std::priority_queue<int>>);
    static_assert(not Container<std::forward_list<int>>);
    static_assert(not Container<std::queue<int>>);
    static_assert(not Container<std::stack<int>>);
    static_assert(not Container<std::span<int>>);

    BOOST_TEST((not Container<std::priority_queue<int>>));
    BOOST_TEST((not Container<std::forward_list<int>>));
    BOOST_TEST((not Container<std::queue<int>>));
    BOOST_TEST((not Container<std::stack<int>>));
    BOOST_TEST((not Container<std::span<int>>));
}
BOOST_AUTO_TEST_SUITE_END();
