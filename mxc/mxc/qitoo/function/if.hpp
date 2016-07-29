/*=============================================================================
Copyright (c) 2001-2011 Joel de Guzman
Copyright (c) 2001-2011 Hartmut Kaiser
Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(MXC_QITOO_FUNCTION_IF)
#define MXC_QITOO_FUNCTION_IF

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/spirit/home/qi/detail/fail_function.hpp>
#include <boost/spirit/home/qi/detail/pass_container.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <vector>

namespace mxc { namespace qitoo {
    BOOST_SPIRIT_TERMINAL(if_)
}}

namespace boost { namespace spirit {
    template <>
    struct use_function<qi::domain, mxc::qitoo::tag::if_>
        : mpl::true_ {};
}}

namespace mxc { namespace qitoo {
    namespace fusion = boost::fusion;
    namespace spirit = boost::spirit;
    namespace qi = spirit::qi;

    template <typename Elements>
    struct if_parser : qi::nary_parser<if_parser<Elements>>
    {
        if_parser(Elements elements) : elements(elements) {}

        template <typename Context, typename Iterator>
        struct attribute
        {
            typedef typename fusion::result_of::pop_front<Elements>::type optionals;
            typedef typename spirit::traits::build_variant<optionals>::type type;
        };

        template <typename F>
        bool parse_container(F f) const
        {
            // the condition parser does not contribute to the attribute
            if (fusion::at_c<0>(elements)
                .parse(f.f.first, f.f.last, f.f.context, f.f.skipper, qi::unused))
            {
                return !f(fusion::at_c<1>(elements));
            }
            else {
                return !f(fusion::at_c<2>(elements));
            }
        }

        template <typename Iterator, typename Context, typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
            , Context& context, Skipper const& skipper
            , Attribute& attr_) const
        {
            spirit::traits::make_container(attr_);

            qi::detail::fail_function<Iterator, Context, Skipper>
                f(first, last, context, skipper);

            if (!parse_container(qi::detail::make_pass_container(f, attr_)))
                return false;

            return true;
        }

        template <typename Context>
        qi::info what(Context& context) const
        {
            qi::info result("if_");
            fusion::for_each(elements,
                spirit::detail::what_function<Context>(result, context));

            return result;
        }

        Elements elements;
    };
}}

namespace boost { namespace spirit { 
    namespace qi {
        ///////////////////////////////////////////////////////////////////////////
        // Parser generator: make_xxx function (objects)
        ///////////////////////////////////////////////////////////////////////////
        template <typename Expr, typename Modifiers>
        struct make_composite<mxc::qitoo::tag::if_, Expr, Modifiers>
            : make_nary_composite<Expr, mxc::qitoo::if_parser>
        {
            BOOST_STATIC_ASSERT_MSG(fusion::result_of::size<Expr>::value == boost::mpl::int_<3>::value,
                "qitoo::if_: Invalid number of arguments. Usage qitoo::if_(expr, expr, expr).");
        };
    }

    namespace traits {
        ///////////////////////////////////////////////////////////////////////////
        template <typename Elements>
        struct has_semantic_action<mxc::qitoo::if_parser<Elements> >
            : nary_has_semantic_action<Elements> {};

        ///////////////////////////////////////////////////////////////////////////
        template <typename Elements, typename Attribute, typename Context
            , typename Iterator>
            struct handles_container<mxc::qitoo::if_parser<Elements>, Attribute, Context
            , Iterator>
            : mpl::true_ {};
    }
}}
#endif