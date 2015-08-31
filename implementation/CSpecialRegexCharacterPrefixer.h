//   Copyright (C) 2011 Andreas Gau
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

#ifndef INCLUDED_CSPECIALREGEXCHARACTERPREFIXER_H_3065454
#define INCLUDED_CSPECIALREGEXCHARACTERPREFIXER_H_3065454

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
#endif
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#include "StringLiteral.h"

#include <boost/foreach.hpp>

///handles prefixing of special regular expression characters with /
class CSpecialRegexCharacterPrefixer
{
public:
    ///handles prefixing in the string
    template <typename StringT>
    static void prefixSpecialCharacters( StringT& text)
    {
        typedef typename StringT::value_type CharT;
        StringT result;
        BOOST_FOREACH( typename StringT::value_type c, text)
        {
            if ( boost::is_any_of( STRING_LITERAL(".[]{}()\\*+?|^$") )(c))
            {
                result += '\\';
            }
            result += c;
        }
        result.swap( text);
    }

    ///handles prefixing for all strings in the container
    template <typename StringT>
    static void prefixSpecialCharacters( std::vector<StringT>& container)
    {
        prefixSpecialCharactersContainer( container);
    }

private:
    ///handles prefixing for all strings in a container
    template <typename ContainerT>
    static void prefixSpecialCharactersContainer( ContainerT& container)
    {
        BOOST_FOREACH( typename ContainerT::value_type& item, container)
        {
            prefixSpecialCharacters( item);
        }
    }
};

#endif /* INCLUDED_CSPECIALREGEXCHARACTERPREFIXER_H_3065454 */
