/*=============================================================================
Copyright (c) 2016 Frank Hein, maxence business consulting gmbh

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef MXC_QITOO_DIRECTIVE_EXPECT
#define MXC_QITOO_DIRECTIVE_EXPECT

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/support/has_semantic_action.hpp>
#include <boost/spirit/home/support/handles_container.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <stdexcept>

namespace mxc {	namespace qitoo {
	BOOST_SPIRIT_TERMINAL(expect);
}}

namespace boost { namespace spirit {
	///////////////////////////////////////////////////////////////////////////
	// Enablers
	///////////////////////////////////////////////////////////////////////////

	template <>
	struct use_directive<qi::domain, mxc::qitoo::tag::expect> // enables expect[p]
		: mpl::true_ {};
}}

namespace mxc {	namespace qitoo {
		
    namespace spirit = boost::spirit;
    namespace qi = spirit::qi;

	template <typename Subject>
	struct expect_directive : qi::unary_parser<expect_directive<Subject> >	
    {
        using subject_type = boost::spirit::result_of::compile<qi::domain, Subject>;

        template <typename Context, typename Iterator>
        struct attribute
        {
            using type = spirit::traits::attribute_of<subject_type, Context, Iterator>;
        };

		expect_directive(Subject const& subject_) : subject(subject_) {}

		template <typename Iterator, typename Context, typename Skipper, typename Attribute>
		bool parse(Iterator& first, Iterator const& last
			, Context& context, Skipper const& skipper, Attribute& attr_) const
		{
            using exception = qi::expectation_failure<Iterator>;
            
            // no need to pre-skip, subject does

            if (!subject.parse(first, last, context, skipper, attr_))
            {
                boost::throw_exception(exception(first, last, subject.what(context)));
#if defined(BOOST_NO_EXCEPTIONS)
                return false;            // for systems not supporting exceptions
#endif
            }
            return true;
		}

		template <typename Context>
		qi::info what(Context& context) const
		{
			return qi::info("expect", subject.what(context));
		}

		Subject subject;
	};
}}

namespace boost { namespace spirit { 
    namespace qi {
	    ///////////////////////////////////////////////////////////////////////////
	    // Parser generators: make_xxx function (objects)
	    ///////////////////////////////////////////////////////////////////////////
	    template <typename Subject, typename Modifiers>
	    struct make_directive<mxc::qitoo::tag::expect, Subject, Modifiers>
	    {
		    using result_type = mxc::qitoo::expect_directive<Subject>;
		    result_type operator()(unused_type, Subject const& subject, unused_type) const
		    {
			    return result_type(subject);
		    }
	    };
    }
    namespace traits {
        ///////////////////////////////////////////////////////////////////////////
        template <typename Subject>
        struct has_semantic_action<mxc::qitoo::expect_directive<Subject> >
            : unary_has_semantic_action<Subject> {};

        ///////////////////////////////////////////////////////////////////////////
        template <typename Subject, typename Attribute, typename Context, typename Iterator>
        struct handles_container<
            mxc::qitoo::expect_directive<Subject>, Attribute, Context, Iterator
        >
            : unary_handles_container<Subject, Attribute, Context, Iterator> {};
    }
}}

#endif