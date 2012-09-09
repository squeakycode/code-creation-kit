//   Copyright (C) 2011-2012 Andreas Gau
//
//   This file is part of the code-creation-kit.
//
//   The code-creation-kit is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   The code-creation-kit is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_SYSTEM_H_1264154
#define INCLUDED_SYSTEM_H_1264154

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/regex.hpp> 
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
        typedef boost::basic_regex<CharT, boost::regex_traits<CharT> > RegexT;

        RegexT searchExpression( STRING_LITERAL( "\\$\\(([_a-zA-Z]+[_a-zA-Z0-9]*)\\)"));
        boost::match_results<typename StringT::const_iterator> what; 
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

#endif /* INCLUDED_SYSTEM_H_1264154 */
