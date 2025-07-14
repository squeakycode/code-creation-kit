// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "cpptokenfinder.hpp"
#include "StringLiteral.h"

namespace code_creation_kit
{
    // A simple formatter that works for tags %1% through %9%
    template <typename StringT>
    class SimpleFormat
    {
        typedef typename StringT::value_type CharT;
        typedef std::basic_stringstream<CharT> StringStreamT;

        class Comparer
        {
        public:
            bool operator()(CharT character_of_token, CharT character_of_provided_text) const
            {
                if (character_of_token == 'd' && character_of_provided_text >= '1' && character_of_provided_text <= '9')
                {
                    return true;
                }
                else
                {
                    return character_of_token == character_of_provided_text;
                }
            }
        };
    public:
        SimpleFormat(const CharT* formatText)
            : m_formatText(formatText)
        {
        }

        template <typename T>
        SimpleFormat& operator %(const T& arg)
        {
            StringStreamT stringsstream;
            stringsstream << arg;
            m_args.push_back(stringsstream.str());
            return *this;
        }

        StringT str() const
        {
            StringT result;

            cpptokenfinder::token_finder<CharT, int, int, -1, Comparer> tokenfinder;
            tokenfinder.add_token(STRING_LITERAL("%d%"), 0);

            const CharT* tokenbegin = nullptr;
            const CharT* tokenend = nullptr;
            int tokenid = -1; //not needed there is only one token
            const CharT* text = m_formatText.c_str();
            while (tokenfinder.find_token(text, tokenbegin, tokenend, tokenid))
            {
                // process format text before the token if any
                if (text != tokenbegin)
                {
                    result += StringT(text, tokenbegin);
                }
                //process token
                size_t argIndex = static_cast<size_t>(*(tokenbegin + 1)) - static_cast<size_t>('1');
                if (argIndex < m_args.size())
                {
                    result += m_args[argIndex];
                }
                //continue with the rest of the format text
                text = tokenend;
            }
            // process the trailing format text if any
            if (text && *text)
            {
                result += text;
            }
            return result;
        }
    private:
        StringT m_formatText;
        std::vector<StringT> m_args;
    };
}
