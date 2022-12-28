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
            typedef typename StringT::value_type CharT;
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
