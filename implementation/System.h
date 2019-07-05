//  Copyright (c) 2011-2019 Andreas Gau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//      * Neither the name of the copyright holder nor the
//        names of contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#if defined(CCK_USE_STD_REGEX)
#   include <regex>
    namespace regex_namespace = std;
#else
#   include <boost/regex.hpp>
    namespace regex_namespace = boost;
#endif

#include "StringLiteral.h"
#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#include <stdexcept>
namespace code_creation_kit
{
    namespace System
    {
        namespace detail
        {
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

            inline char* systemGetEnv( const char* name)
            {
                return getenv( name);
            }

            inline wchar_t* systemGetEnv( const wchar_t* name)
            {
#ifdef WIN32
                return _wgetenv( name);
#else
                (void)name; //unused
                //TODO
                throw std::runtime_error( "Reading Unicode encoded environment variable is not implemented.");
#endif
            }

#ifdef _MSC_VER
#pragma warning( pop )
#endif
        }



        ///expands environment variable using the $(VARIABLE) syntax
        template <typename StringT>
        StringT expandEnvironmentVariables( const StringT& text)
        {
            typedef typename StringT::value_type CharT;
            typedef regex_namespace::basic_regex<CharT, regex_namespace::regex_traits<CharT> > RegexT;

            RegexT searchExpression( STRING_LITERAL( "\\$\\(([_a-zA-Z]+[_a-zA-Z0-9]*)\\)"));
            regex_namespace::match_results<typename StringT::const_iterator> what;
            typename StringT::const_iterator start = text.begin();
            typename StringT::const_iterator end = text.end(); 
            StringT result;

            while( regex_search(start, end, what, searchExpression)) 
            {
                //add test before variable to result
                result += StringT( start, what[ 0 ].first);

                //place start after found variable
                start = what[ 0 ].second;

                if ( what[ 1 ].matched )
                {
                    StringT variableName( what[ 1 ].first, what[ 1 ].second);

                    CharT* getenvResult = detail::systemGetEnv( variableName.c_str());

                    if ( getenvResult)
                    {
                        //expand
                        result += boost::lexical_cast<StringT>( getenvResult);
                    }
                    else
                    {
                        //variable not found leave untouched
                        result += StringT( what[ 0 ].first, what[ 0 ].second);
                    }
                }
            }

            //add the rest of the text if any
            if ( start != end)
            {
                result += StringT( start, end);
            }
            return result;
        }
    }
}