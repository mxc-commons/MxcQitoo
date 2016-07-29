/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <mxc/qitoo/qitoo.hpp>

#include <iostream>
#include <vector>
#include "test.hpp"

using namespace mxc::qitoo;

int
main()
{
    using spirit_test::test;
    using spirit_test::test_attr;
    using boost::spirit::qi::hold;
    using boost::spirit::qi::int_;
    using boost::spirit::ascii::alpha;

    {
        std::vector<int> vec;
        BOOST_TEST(!test_attr("1$", probe[int_ >> ';'], vec));
        BOOST_TEST(vec.size() == 0);
        BOOST_TEST(test_attr("1;", probe[int_ >> ';'], vec));
        BOOST_TEST(vec.size() == 1);
        BOOST_TEST(vec[0] == 1);
    }

    {
        std::string attr;
        BOOST_TEST(
            test_attr(
                "abc;",
                probe[alpha >> ';'] | (+alpha >> ';'),
                attr));
        BOOST_TEST(attr == "abc");

        attr.clear();
        BOOST_TEST(
            test_attr(  // temporaries
                "abc;abc",
                (+alpha >> ';') >> (probe[+alpha >> ';'] | (+alpha))
              ,
                attr));
        BOOST_TEST(attr == "abcabc");
        std::cout << attr;

    }

    return boost::report_errors();
}
