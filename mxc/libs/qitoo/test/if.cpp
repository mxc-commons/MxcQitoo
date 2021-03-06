/*=============================================================================
Copyright (c) 2016  Frank Hein, maxence business consulting gmbh

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/detail/lightweight_test.hpp>
#include <mxc/qitoo/qitoo.hpp>

#include <string>
#include <iostream>
#include <vector>
#include "test.hpp"

int main() {
    using mxc::qitoo::if_;
    namespace qi = boost::spirit::qi;
    using namespace boost::spirit::qi;
    using spirit_test::test;
    using spirit_test::test_attr;

    // to test whether if_ works with grammar and rule parameter
    struct test_grammar : qi::grammar<char const*, std::string(), qi::space_type >
    {
        qi::rule<char const*, std::string(), qi::space_type > r;
        test_grammar() : test_grammar::base_type(r)
        {
            r = if_(qi::char_('I'), qi::string("if"), qi::lit('E') >> qi::string("else"));
        }
    };

    std::string s;

    // condition matches, if-parser matches
    // test if condition parser attr gets omitted
    BOOST_TEST(test_attr("I if", if_(qi::char_('I'), qi::string("if"), qi::lit('E') >> qi::string("else")), s, qi::space));
    BOOST_TEST(s == "if");

    //same test using grammar
    test_grammar g;
    s.clear();
    BOOST_TEST(test_attr("I if", g, s, qi::space));
    BOOST_TEST(s == "if");

    //same test using grammar
    s.clear();
    BOOST_TEST(test_attr("I if", g.r, s, qi::space));
    BOOST_TEST(s == "if");

    // condition matches, if-parser fails
    BOOST_TEST(!test("I mismatch", if_(lit('I'), qi::string("if"), qi::lit('E') >> qi::string("else")), qi::space));

    // condition fails, else-parser matches
    s.clear();
    BOOST_TEST(test_attr("E else", if_(lit('I'), qi::string("if"), qi::lit('E') >> qi::string("else")), s, qi::space));
    BOOST_TEST(s == "else");

    // condition fails, else-parser fails
    BOOST_TEST(!test("E mismatch", if_(lit('I'), qi::string("if"), qi::lit('E') >> qi::string("else")), qi::space));

    // if embedded in sequence, string matches, condition matches, if-parser matches
    std::vector<std::string> v;
    BOOST_TEST(test_attr(
        "w I if", 
        qi::string("w") 
            >> if_(qi::lit('I') 
                , qi::raw[qi::string("if")]
                , qi::string("else") >> qi::lit(",;"))
        , v, qi::space));
    BOOST_TEST(v.size() == 2 &&
        v[0] == "w" && v[1] == "if");

    // nested if_, outer condition fails, inner condition matches, string matches
    s.clear();
    BOOST_TEST(test_attr(
        "E else"
        , if_(qi::lit('I') , 
            qi::string("if")
            , if_(qi::lit('E')
                , qi::string("else")
                , qi::string("other")))
            , s, qi::space
            ));
    BOOST_TEST(s == "else");

    return boost::report_errors();
}
