/*=============================================================================
Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <map>

#include <boost/spirit/home/qi.hpp>
#include <mxc/qitoo/qitoo.hpp>

namespace qi = boost::spirit::qi;
namespace qitoo = mxc::qitoo;

using iterator_type = std::string::const_iterator;
using result_type = std::string;

template<typename Parser>
void parse(const std::string message, const std::string& input, const std::string& rule, const Parser& parser) {
    iterator_type iter = input.begin(), end = input.end();

    std::vector<result_type> parsed_result;

    std::cout << "-------------------------\n";
    std::cout << message << "\n";
    std::cout << "Rule: " << rule << std::endl;
    std::cout << "Parsing: \"" << input << "\"\n";

    bool result = qi::phrase_parse(iter, end, parser, qi::space, parsed_result);
    if (result)
    {
        std::cout << "Parser succeeded.\n";
        std::cout << "Parsed " << parsed_result.size() << " elements:";
        for (const auto& str : parsed_result)
            std::cout << "[" << str << "]";
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Parser failed" << std::endl;
    }
    if (iter == end) {
        std::cout << "EOI reached." << std::endl;
    }
    else {
        std::cout << "EOI not reached. Unparsed: \"" << std::string(iter, end) << "\"" << std::endl;
    }
    std::cout << "-------------------------\n";

}

int main() {

    qi::rule < iterator_type, std::string(), qi::space_type>
        id = (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'));

    parse("plain, two comma separated ids"
        , "id1, id2"
        , "id >> ',' >> id"
        , id >> ',' >> id);

    parse("probe, two comma separated ids"
        , "id1, id2"
        , "probe[id >> ',' >> id] "
        , qitoo::probe[id >> ',' >> id]);


    parse("probe fail, two comma separated ids"
        , "id1, 1234 "
        , "probe[id >> ',' >> id] >> id >> ',' >> qi::uint_"
        , qitoo::probe[id >> ',' >> id]);
    
    return 0;
}