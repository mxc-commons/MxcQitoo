/*=============================================================================
Copyright (c) 2001-2011 Joel de Guzman
Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(MXC_QITOO_DIRECTIVE_PROBE)
#define MXC_QITOO_DIRECTIVE_PROBE

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>


namespace mxc { namespace qitoo {
    BOOST_SPIRIT_TERMINAL(probe);
}}

namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_directive<qi::domain, mxc::qitoo::tag::probe> // enable probe[]
        : mpl::true_ {};
}}

namespace mxc { namespace qitoo {

    namespace spirit    = boost::spirit;
    namespace qi        = spirit::qi;

    template <typename Subject>
    struct probe_directive : qi::unary_parser<probe_directive<Subject> >
    {
        using subject_type = Subject;

        probe_directive(Subject const& subject_) : subject(subject_) {}

        template <typename Context, typename Iterator>
        struct attribute
        {
            using type = typename spirit::traits::attribute_of<subject_type, Context, Iterator>::type;
        };

        template <typename Iterator, typename Context, typename Skipper, typename Attribute>
            bool parse(Iterator& first, Iterator const& last
                , Context& context, Skipper const& skipper, Attribute& attr_) const
        {
            Iterator save = first;
            if (subject.parse(first, last, context, skipper, qi::unused))
            {
                first = save;
                return subject.parse(first, last, context, skipper, attr_);
            }
            return false;
        }

        template <typename Context>
        qi::info what(Context& context) const
        {
            return qi::info("probe", subject.what(context));
        }

        Subject subject;
    };

}}

namespace boost {  namespace spirit { 
    namespace qi 
    {
        ///////////////////////////////////////////////////////////////////////////
        // Parser generators: make_xxx function (objects)
        ///////////////////////////////////////////////////////////////////////////
        template <typename Subject, typename Modifiers>
        struct make_directive<mxc::qitoo::tag::probe, Subject, Modifiers>
        {
            using result_type = mxc::qitoo::probe_directive<Subject>;
            result_type operator()(unused_type, Subject const& subject, unused_type) const
            {
                return result_type(subject);
            }
        };
    }
    namespace traits
    {
        ///////////////////////////////////////////////////////////////////////////
        template <typename Subject>
        struct has_semantic_action<mxc::qitoo::probe_directive<Subject> >
            : unary_has_semantic_action<Subject> {};
        ///////////////////////////////////////////////////////////////////////////
        template <typename Subject, typename Attribute, typename Context, typename Iterator>
        struct handles_container<mxc::qitoo::probe_directive<Subject>, Attribute, Context, Iterator> 
            : unary_handles_container<Subject, Attribute, Context, Iterator> {};
    }
}}

#endif
