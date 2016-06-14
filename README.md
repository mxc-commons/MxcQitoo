MxcQitoo
=============
Version 1.0.0 created by Frank Hein and the mxc-commons team.

MxcQitoo provides some additional parser components for the Boost-Spirit-Qi (currently v2.5.2) parser library.
[Boost Spirit Qi documentation](http://www.boost.org/doc/libs/1_61_0/libs/spirit/doc/html/index.html) can be found here.

MxcQitoo is part of the maxence Open Source initiative by [maxence business consulting gmbh, Germany](http://www.maxence.de). 

Introduction
------------

Spirit is a set of C++ libraries for parsing and output generation implemented as Domain Specific Embedded Languages (DSEL) using Expression templates and Template Meta-Programming. The Spirit libraries enable a target grammar to be written exclusively in C++. Inline grammar specifications can mix freely with other C++ code and, thanks to the generative power of C++ templates, are immediately executable.

Spirit is part of [Boost C++ Libraries](http://www.boost.org), a peer-reviewed, open collaborative development effort.

Requirements
------------

* [Boost C++ Libraries](http://www.boost.org) (latest)

The parser components are tested with Boost 1.6.0 and 1.6.1, but should work even on older boost installations.

Features / Goals
----------------

* Ease the use of parsers for the implementors in some situations
* Simplification of rules


Installation
------------

### Main Setup

As spirit::qi qitoo is a header only collection of parsers.

#### Cloning project

1. Clone this project to a location you want. 
2. Add the `mxc` directory to your include path.
3. Directory organisation just like Boost directory organisation


#### Post installation

1. MxcQitoo parser objects live in the namespace `mxc::qitoo` (reads qi, too ;). Use like
 
        #include <mxc/qitoo/qitoo.hpp>
        #include <boost/spirit/home/qi.hpp>

        namespace qitoo=mxc::qitoo; 
        namespace qi = boost::spirit::qi;
        
        // just to show 
        qi::rule<iterator_type, result_type(), skipper_type> rule = qi::raw["X"] >> qitoo::probe[id];        
       
2. Please have a look at the provided examples which supplement this small documentation.       

Directives
-------

Provided are the directives:

- **expect** - `qitoo::expect` works similar to the qi expect operator >. See [documentation here](http://www.boost.org/doc/libs/1_61_0/libs/spirit/doc/html/spirit/qi/reference/operator/expect.html) here. Other than the qi operator the qitoo expect
  directive throws an exception even if the first operand fails. Usage:

       
        qitoo::expect[parser-expression]        
       



- **probe** - `qitoo::probe` works exactly as `qi::hold`. See [documentation here](http://www.boost.org/doc/libs/1_61_0/libs/spirit/doc/html/spirit/qi/reference/directive/hold.html) here.O ther than `qi::hold` does `qitoo::probe` not require to
  implement swap for the ast containers. This convenience gets payed for with runtime and possibly increased buffering
  requirements, because `probe` does lookahead parsing to work. Usage:

       
        qitoo::probe[parser-expression]        
       
  
  Note: One important design goal of `spirit::qi` is speed. Keep in mind, that `probe` parses twice if successful, so
  take care not to apply it to parser-expressions if it can consume MB of input. An advantage of 'qitoo::probe'  over
  `qi::hold` is that it can be applied to temporaries, so 
 

        parser-expression >> qitoo::probe[parser-expression] >> ...

 will work as expected without having taking care about that 'temporaray' type and how to swap it..        


Operators
-------

  
- **operator /** - This operator acts like the `qi::operator %`, [see documentation here] (http://www.boost.org/doc/libs/1_61_0/libs/spirit/doc/html/spirit/qi/reference/operator/list.html) but the right hand side operand contributes to the attribute vector also.
  `qi::operator %` discards the rhs operand's attribute.  `id % char_(';,')]` does the same as `id / qi::omit[char_(';,')]`

  Note: `qitoo::operator /` needs to lookahead both operands to detect if the list continues and to make sure, that the returned attribute is
  not polluted with temporary results. Consider runtime effects and increased buffering requirements. For tasks like the following this is
  not relevant, but if you take a serious long string of thousands of chars it would be. 

        
        id = (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'));
        qualified_id = id / qi::string("::");

Given an input of ```ident1::ident2``` qualified_id matches providing the result ```ident1::ident2```.
Given an input of ```ident1``` qualified_id matches providing the result ```ident1```.

License
-------

MxcQitoo is distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt). (See link or accompanying
file LICENSE.txt).

Acknowledgements
----------------

This work relies on Boost Spirit, which lives because of the tremendous amount of time and effort spent by [Joel de Guzman, Hartmut Kaiser and many other developers](http://boost-spirit.com/home/people/) who made Spirit happen.  