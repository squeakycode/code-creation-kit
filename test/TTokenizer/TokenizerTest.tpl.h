//  Copyright (c) 2011-2015 Andreas Gau
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
