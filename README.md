MxcQitoo
=============
Version 1.0.0 created by Frank Hein and the mxc-commons team.

MxcQitoo is part of the maxence openBeee initiative by [maxence business consulting gmbh, Germany](http://www.maxence.de). 

Introduction
------------

Spirit is a set of C++ libraries for parsing and output generation implemented as Domain Specific Embedded Languages (DSEL) using Expression templates and Template Meta-Programming. The Spirit libraries enable a target grammar to be written exclusively in C++. Inline grammar specifications can mix freely with other C++ code and, thanks to the generative power of C++ templates, are immediately executable.

Spirit is part of [Boost C++ Libraries](http://www.boost.org), a peer-reviewed, open collaborative development effort.

MxcQitoo provides some additional parser components for the Boost-Spirit-Qi (currently v2.5.2) parser library.
[Boost Spirit Qi documentation](http://www.boost.org/doc/libs/1_61_0/libs/spirit/doc/html/index.html) can be found here.


MxcQitoo parser objects live in the namespace `mxc::qitoo` (reads qi, too ;)

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
2. Add the `qitoo` directory to your include path.
3. The examples directory contains what it says.


#### Post installation

1. MxcQitoo parser objects live in the namespace `mxc::qitoo` (reads qi, too ;). Use like
 
        #include <qitoo.qitoo.hpp>
        #include <boost/spirit/home/qi.hpp>

        namespace qitoo=mxc::qitoo; 
        namespace qi = boost::spirit::qi;
        
        // just to show 
        qi::rule<iterator_type, result_type(), skipper_type> rule = qi::raw["X"] >> qitoo::probe[id];        
       
	      

Directives
-------

Provided are the directives:

- **expect** - `qitoo::expect` works similar to the qi expect operator >. Other than the qi operator the qitoo expect
  directive throws an exception even if the first operand fails. Usage:

       
        qitoo::expect[parser-expression]        
       



- **probe** - `qitoo::probe` works exactly as `qi::hold`. Other than `qi::hold` does `qitoo::probe` not require to
  implement swap for the ast containers. This convenience gets payed for with runtime, because `probe` does lookahead
  parsing to work. Usage:

       
        qitoo::probe[parser-expression]        
       
  
  Note: One important design goal of `spirit::qi` is speed. Keep in mind, that `probe` parses twice if successful, so
  take care not to apply it to parser-expressions which consume MB of input.
 

Operators
-------

  
- **operator +=** - This operator acts like the `qi::operator %`, but the right hand side operand contributes to the attribute vector also.
  `qi::operator %` discards the rhs operand's attribute.  `id % char_(';,')]` does the same as `id += qi::omit[char_(';,')]`

  Note: `qitoo::operator +=` needs to lookahead both operands to detect if the list continues and to make sure, that the returned attribute is
  not polluted with temporary results. 


        
        id = (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'));
        qualified_id = id += qi::string("::");
        


License
-------

MxcQitoo is distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt). (See link or accompanying
file LICENSE.txt).

Acknowledgements
----------------

Boost Spirit would not be here for us without the work of [Joel de Guzman, Hartmut Kaiser and many other developers](http://boost-spirit.com/home/people/).  