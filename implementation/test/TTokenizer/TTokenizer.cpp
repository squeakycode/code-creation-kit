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

#define BOOST_TEST_MAIN
#include "boost/test/unit_test.hpp"
#include "CTokenizer.gen.h"
#include "CBackEndTokenizer.gen.h"
#include "ETokens.gen.h"
#include "CToken.h"
#include "TokenizerTest.gen.h"
#include <string>
#include <vector>
#include "StringLiteral.h"

template <typename TokenT>
class CTokenizerTestHelper
{
public:
    CTokenizerTestHelper<TokenT> operator << ( const TokenT& token)
    {
        result.push_back( token);
        return *this;
    }

    std::vector<TokenT> result;
};

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void test()
{
    typedef typename StringT::value_type CharT;
    TokenizerT tokenizer;
    setKeywords<TokenizerT, StringT>( tokenizer);

    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;

    {//plain text
        result.clear();
        tokenizer << STRING_LITERAL( "text");
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK(   result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text")) //when frontend
            || result[0] == TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("text")) //when backend
            );
    }

    {//comment
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT%a comment  \n");
        BOOST_CHECK( result.size() == 0);
    }

    {//trim
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM% \n");
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text")));        
    }

    {//new line
        result.clear();
        tokenizer << STRING_LITERAL( "\n");
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK(   result[0] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")) //when frontend
            || result[0] == TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("\n")) //when backend
            );
    }

    //generated tests
    testSingleTokens<TokenizerT, OutputT, TokenT, StringT>();
}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testBackEndFeatures()
{
    typedef typename StringT::value_type CharT;
    TokenizerT tokenizer;
    setKeywords<TokenizerT, StringT>( tokenizer);

    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;

    {//BEGIN. handle one dot as not dot
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));
        expected.push_back( TokenT());
        expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("end")));    

        result.clear();
        tokenizer << STRING_LITERAL("start%BEGIN.%end");
        expected[1] = TokenT( TokenT::eBegin);
        BOOST_REQUIRE( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    //full line without tags, no new line when end of file stream
    {
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("start")));

        result.clear();
        tokenizer.close();
        tokenizer << STRING_LITERAL("start");
        tokenizer.open();
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK( expected == result);
    }

    //full line without tags, no new line when end of file stream
    {
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));

        result.clear();
        tokenizer << STRING_LITERAL("start");
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK( expected == result);
    }

    //full line without tags
    {
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("start\n")));

        result.clear();
        tokenizer << STRING_LITERAL("start\n");
        BOOST_REQUIRE( result.size() == 1);
        BOOST_CHECK( expected == result);
    }

    //generated tests
    testRemoveDelayMarks<TokenizerT, OutputT, TokenT, StringT>();
}


BOOST_AUTO_TEST_CASE( TTokenizer)
{
    {
        typedef std::string StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT> OutputT;
        typedef CTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::string StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT> OutputT;
        typedef CBackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT> OutputT;
        typedef CTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT> OutputT;
        typedef CBackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

}

