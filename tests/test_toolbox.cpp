#define BOOST_TEST_MODULE toolbox_test_module

#include "filter_predicat.h"
#include "filter_range.h"
#include "ip4.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(toolbox_test_suite)

BOOST_AUTO_TEST_CASE(toolbox_test_extract_ip) {
    IP4Address addr;
    bool rv = extract_ip("1.2.3.4\t xyz", &addr);

    BOOST_CHECK(rv == true);
    BOOST_CHECK(addr == (IP4Address{1, 2, 3, 4}));

    rv = extract_ip("1.2.3", &addr);
    BOOST_CHECK(rv == false);
}

BOOST_AUTO_TEST_CASE(toolbox_test_variadic_range) {
    auto r = IP4Range {};
    BOOST_CHECK(r.minimal == (IP4Address{0, 0, 0, 0}));
    BOOST_CHECK(r.maximal == (IP4Address{255, 255, 255, 255}));

    r = IP4Range {1};
    BOOST_CHECK(r.minimal == (IP4Address{1, 0, 0, 0}));
    BOOST_CHECK(r.maximal == (IP4Address{1, 255, 255, 255}));

    r = IP4Range {1, 2};
    BOOST_CHECK(r.minimal == (IP4Address{1, 2, 0, 0}));
    BOOST_CHECK(r.maximal == (IP4Address{1, 2, 255, 255}));

    r = IP4Range {1, 2, 3, 4};
    BOOST_CHECK(r.minimal == (IP4Address{1, 2, 3, 4}));
    BOOST_CHECK(r.maximal == (IP4Address{1, 2, 3, 4}));
}

// TODO filter_range
// TODO filter_predicat

BOOST_AUTO_TEST_SUITE_END()
