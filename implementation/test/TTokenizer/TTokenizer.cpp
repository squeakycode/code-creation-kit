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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "CTokenizer.gen.h"
#include "CBackEndTokenizer.gen.h"
#include "ETokens.gen.h"
#include "CToken.h"
#include "TokenizerTest.gen.h"
#include <string>
#include <vector>
#include "StringLiteral.h"

template <typename TokenT, typename StringT>
class CTokenizerTestHelper
{
public:
    CTokenizerTestHelper<TokenT,StringT>& operator << ( const StringT& text)
    {
        resultString.push_back( text);
        return *this;
    }

    CTokenizerTestHelper<TokenT,StringT>& operator << ( const TokenT& token)
    {
        result.push_back( token);
        return *this;
    }

    std::vector<TokenT> result;
    std::vector<StringT> resultString;
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
        BOOST_REQUIRE( result.size() == 2);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text")));
        BOOST_CHECK( result[1] == TokenT( TokenT::eNewLine));
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

    {//BEGIN. handle one dot as no dot
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

    {//comment and bypass mode
        tokenizer.setBypassMode( true);
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT%text \n");
        BOOST_REQUIRE( result.size() == 4);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        BOOST_CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%COMMENT%")));
        BOOST_CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text ")));
        BOOST_CHECK( result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT.%text \n");
        BOOST_REQUIRE( result.size() == 5);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        BOOST_CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%COMMENT")));
        BOOST_CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%")));
        BOOST_CHECK( result[3] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text ")));
        BOOST_CHECK( result[4] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        tokenizer.setBypassMode( false);
    }

    {//trim and bypass mode
        tokenizer.setBypassMode( true);
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM% \n");
        BOOST_REQUIRE( result.size() == 4);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" text")));
        BOOST_CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM%")));
        BOOST_CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        BOOST_CHECK( result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM.% \n");
        BOOST_REQUIRE( result.size() == 5);
        BOOST_CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" text")));
        BOOST_CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM")));
        BOOST_CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%")));
        BOOST_CHECK( result[3] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        BOOST_CHECK( result[4] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        tokenizer.setBypassMode( false);
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
    {
        TokenizerT tokenizer;
        testRemoveDelayMarks<TokenizerT, OutputT, TokenT, StringT>(tokenizer, false);
    }

    //generated tests
    {
        TokenizerT tokenizer;
        tokenizer.setBypassMode( true);
        testRemoveDelayMarks<TokenizerT, OutputT, TokenT, StringT>(tokenizer, true);
    }
}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testInlineTemplateProcessing()
{
    typedef typename StringT::value_type CharT;
    TokenizerT tokenizer;
    setKeywords<TokenizerT, StringT>( tokenizer);

    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ * "), STRING_LITERAL("* /"), STRING_LITERAL("// $")), CTokenizerExceptions::ExInlineMarkupWhiteSpace);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ *"), STRING_LITERAL("* / "), STRING_LITERAL("// $")), CTokenizerExceptions::ExInlineMarkupWhiteSpace);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ *"), STRING_LITERAL("* /"), STRING_LITERAL("// $ ")), CTokenizerExceptions::ExInlineMarkupWhiteSpace);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL(""), STRING_LITERAL("*/"), STRING_LITERAL("//$")), CTokenizerExceptions::ExInlinePrefixEmpty);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("*/"), STRING_LITERAL("")), CTokenizerExceptions::ExInlineGeneratedPostfixEmpty);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("**/"), STRING_LITERAL("*/")), CTokenizerExceptions::ExBadInlineGeneratedPostfix);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("*/"), STRING_LITERAL("**/")), CTokenizerExceptions::ExBadInlineGeneratedPostfix);
    BOOST_CHECK_THROW( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*/"), STRING_LITERAL(""), STRING_LITERAL("*/")), CTokenizerExceptions::ExBadInlineGeneratedPostfix);

    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    tokenizer.connectFinalOutputStream( &helper); 
    tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("*/"), STRING_LITERAL("//$"));
    tokenizer.setInlineTemplateMode( true);
    std::vector<TokenT>& result = helper.result;
    std::vector<StringT>& result2 = helper.resultString;

    {//test
        result.clear();
        result2.clear();
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( "text\n");
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( " /* macro */ \n");
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( " /*/ \n");

        BOOST_REQUIRE( result.size() == 2);
        BOOST_CHECK(   result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("  macro  ")));
        BOOST_CHECK(   result[1] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        BOOST_REQUIRE( result2.size() == 3);
        BOOST_CHECK(   result2[0] == STRING_LITERAL("text\n"));
        BOOST_CHECK(   result2[1] == STRING_LITERAL(" /* macro */ \n"));
        BOOST_CHECK(   result2[2] == STRING_LITERAL(" /*/ \n"));
    }

    {//test no postfix
        tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL(""), STRING_LITERAL("//$"));
        result.clear();
        result2.clear();
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( "text\n");
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( " /* macro */ \n");
        tokenizer << STRING_LITERAL( "generated //$");
        tokenizer << STRING_LITERAL( " /*/ \n");

        BOOST_REQUIRE( result.size() == 4);
        BOOST_CHECK(   result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("  macro */ ")));
        BOOST_CHECK(   result[1] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        BOOST_CHECK(   result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" / ")));
        BOOST_CHECK(   result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        BOOST_REQUIRE( result2.size() == 3);
        BOOST_CHECK(   result2[0] == STRING_LITERAL("text\n"));
        BOOST_CHECK(   result2[1] == STRING_LITERAL(" /* macro */ \n"));
        BOOST_CHECK(   result2[2] == STRING_LITERAL(" /*/ \n"));
    }


}


BOOST_AUTO_TEST_CASE( TTokenizer)
{
    {
        typedef std::string StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef CTokenizer<TokenT, StringT, OutputT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testInlineTemplateProcessing<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::string StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef CBackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef CTokenizer<TokenT, StringT, OutputT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testInlineTemplateProcessing<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef CToken<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef CBackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

}

