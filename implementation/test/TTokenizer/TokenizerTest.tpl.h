//   Copyright (C) 2011-2015 Andreas Gau
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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#include "StringLiteral.h"

template <typename TokenizerT, typename StringT> 
void setKeywords( TokenizerT& tokenizer)
{
    typedef typename StringT::value_type CharT;
    tokenizer.setMarkup( STRING_LITERAL("%"), STRING_LITERAL("%"));
}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testSingleTokens()
{
    typedef typename StringT::value_type CharT;
    TokenizerT tokenizer;
    setKeywords<TokenizerT, StringT>( tokenizer);
    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;
    std::vector<TokenT> expected;

    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));
    expected.push_back( TokenT());
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("end")));

    [MACRO_BEGIN][TRIM]
    {//[ENTRY]["Tag Name"][IF][ENTRY]["Tag Name Capital"][NOT][STARTS_WITH]["SetRecursionLevelLimit"]
        result.clear();
        [BEGIN][IF][ENTRY]["Parameter Count"][EQUALS]["1"][IF][ENTRY]["Parameter Format"][EQUALS]["Regex"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%['parameter1']end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1") );
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["2"][IF][ENTRY]["Parameter Format"][EQUALS]["Regex"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%['parameter1','parameter2']end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["2"][IF][ENTRY]["Parameter Format"][EQUALS]["CombiCStyleUIntUIntRepeat"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%[\"parameter1\",42]end");
        expected[1] = TokenT(TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1"), STRING_LITERAL("42"));
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["1"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1"));
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["2"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%[\"parameter1\",\"parameter2\"]end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        [OR][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"]);
        [END][TRIM]
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    [MACRO_END][TRIM]
}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testRemoveDelayMarks( TokenizerT& tokenizer, bool bypassMode)
{
    typedef typename StringT::value_type CharT;
    setKeywords<TokenizerT, StringT>( tokenizer);
    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;

    std::vector<TokenT> expected;
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));
    expected.push_back( TokenT());
    expected.push_back( TokenT( TokenT::eTextFragment, bypassMode ? STRING_LITERAL("%") : STRING_LITERAL(".%")));
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("end")));

    [MACRO_BEGIN][TRIM]
    {//[ENTRY]["Tag Name"][IF][ENTRY]["Tag Name Capital"][NOT][STARTS_WITH]["SetRecursionLevelLimit"]

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%[ENTRY]["Tag Name"].%end") : STRING_LITERAL("start%[ENTRY]["Tag Name"]..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%[ENTRY]["Tag Name"]"));
        BOOST_CHECK( expected == result);
    }

    [MACRO_END][TRIM]
}
