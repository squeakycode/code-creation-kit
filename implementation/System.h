// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <regex>

#include "StringLiteral.h"
#include <stdlib.h>

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
            typedef std::basic_regex<CharT, std::regex_traits<CharT> > RegexT;

            RegexT searchExpression( STRING_LITERAL( "\\$\\(([_a-zA-Z]+[_a-zA-Z0-9]*)\\)"));
            std::match_results<typename StringT::const_iterator> what;
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
                        result += getenvResult;
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