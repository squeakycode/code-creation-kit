// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

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
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["1"][IF][ENTRY]["Parameter Format"][EQUALS]["UInt"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%[20]end");
        expected[1] = TokenT(TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("20"));
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["2"][IF][ENTRY]["Parameter Format"][EQUALS]["Regex"][TRIM]
        tokenizer << STRING_LITERAL("start%[ENTRY]["Tag Name"]%['parameter1','parameter2']end");
        expected[1] = TokenT( TokenT::e[ENTRY]["Tag Name Capital"], STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        [OR][IF][ENTRY]["Parameter Count"][EQUALS]["2"][IF][ENTRY]["Parameter Format"][EQUALS]["Combi1CStyle1UIntRepeatUIntOptional"][TRIM]
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
        CHECK( result.size() == 3);
        CHECK( expected == result);
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
        CHECK( expected == result);
    }

    [MACRO_END][TRIM]
}
