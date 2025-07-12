// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "cppstringx.hpp"

namespace code_creation_kit
{
    ///handles prefixing of special regular expression characters with /
    class CSpecialRegexCharacterPrefixer
    {
    public:
        ///handles prefixing in the string
        template <typename StringT>
        static void prefixSpecialCharacters( StringT& text)
        {
            StringT result;
            for (typename StringT::value_type c : text)
            {
                if ( cppstringx::utility::is_any_of<const char*>(".[]{}()\\*+?|^$")(c))
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
            for (typename ContainerT::value_type& item : container)
            {
                prefixSpecialCharacters( item);
            }
        }
    };
}
