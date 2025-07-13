// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Tokenizer.gen.h"
#include "BackEndTokenizer.gen.h"
#include "ETokens.gen.h"
#include "Token.h"
#include "TokenizerTest.gen.h"
#include <string>
#include <vector>
#include "StringLiteral.h"

using namespace code_creation_kit;

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
        REQUIRE( result.size() == 1);
        CHECK((  result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text")) //when frontend
            || result[0] == TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("text")) //when backend
            ));
    }

    {//comment
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT%a comment  \n");
        CHECK( result.size() == 0);
    }

    {//trim
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM% \n");
        REQUIRE( result.size() == 2);
        CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text")));
        CHECK( result[1] == TokenT( TokenT::eNewLine));
    }

    {//new line
        result.clear();
        tokenizer << STRING_LITERAL( "\n");
        REQUIRE( result.size() == 1);
        CHECK((  result[0] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")) //when frontend
            || result[0] == TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("\n")) //when backend
            ));
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
        REQUIRE( result.size() == 3);
        CHECK( expected == result);
    }

    {//comment and bypass mode
        tokenizer.setBypassMode( true);
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT%text \n");
        REQUIRE( result.size() == 4);
        CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%COMMENT%")));
        CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text ")));
        CHECK( result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        result.clear();
        tokenizer << STRING_LITERAL( " %COMMENT.%text \n");
        REQUIRE( result.size() == 5);
        CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%COMMENT")));
        CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%")));
        CHECK( result[3] == TokenT( TokenT::eTextFragment, STRING_LITERAL("text ")));
        CHECK( result[4] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        tokenizer.setBypassMode( false);
    }

    {//trim and bypass mode
        tokenizer.setBypassMode( true);
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM% \n");
        REQUIRE( result.size() == 4);
        CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" text")));
        CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM%")));
        CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        CHECK( result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        result.clear();
        tokenizer << STRING_LITERAL( " text%TRIM.% \n");
        REQUIRE( result.size() == 5);
        CHECK( result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" text")));
        CHECK( result[1] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM")));
        CHECK( result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL("%")));
        CHECK( result[3] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" ")));
        CHECK( result[4] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
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
        REQUIRE( result.size() == 1);
        CHECK( expected == result);
    }

    //full line without tags, no new line when end of file stream
    {
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));

        result.clear();
        tokenizer << STRING_LITERAL("start");
        REQUIRE( result.size() == 1);
        CHECK( expected == result);
    }

    //full line without tags
    {
        std::vector<TokenT> expected;

        expected.push_back( TokenT( TokenT::eFullLineWithoutTags, STRING_LITERAL("start\n")));

        result.clear();
        tokenizer << STRING_LITERAL("start\n");
        REQUIRE( result.size() == 1);
        CHECK( expected == result);
    }

    //generated tests
    {
        testRemoveDelayMarks<TokenizerT, OutputT, TokenT, StringT>(tokenizer, false);
    }

    //generated tests
    {
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

    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ * "), STRING_LITERAL("* /"), STRING_LITERAL("// $")), TokenizerExceptions::ExInlineMarkupWhiteSpace);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ *"), STRING_LITERAL("* / "), STRING_LITERAL("// $")), TokenizerExceptions::ExInlineMarkupWhiteSpace);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/ *"), STRING_LITERAL("* /"), STRING_LITERAL("// $ ")), TokenizerExceptions::ExInlineMarkupWhiteSpace);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL(""), STRING_LITERAL("*/"), STRING_LITERAL("//$")), TokenizerExceptions::ExInlinePrefixEmpty);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("*/"), STRING_LITERAL("")), TokenizerExceptions::ExInlineGeneratedPostfixEmpty);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("**/"), STRING_LITERAL("*/")), TokenizerExceptions::ExBadInlineGeneratedPostfix);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*"), STRING_LITERAL("*/"), STRING_LITERAL("**/")), TokenizerExceptions::ExBadInlineGeneratedPostfix);
    CHECK_THROWS_AS( tokenizer.setInlineTemplateMarkup( STRING_LITERAL("/*/"), STRING_LITERAL(""), STRING_LITERAL("*/")), TokenizerExceptions::ExBadInlineGeneratedPostfix);

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

        REQUIRE( result.size() == 2);
        CHECK(   result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("  macro  ")));
        CHECK(   result[1] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        REQUIRE( result2.size() == 3);
        CHECK(   result2[0] == STRING_LITERAL("text\n"));
        CHECK(   result2[1] == STRING_LITERAL(" /* macro */ \n"));
        CHECK(   result2[2] == STRING_LITERAL(" /*/ \n"));
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

        REQUIRE( result.size() == 4);
        CHECK(   result[0] == TokenT( TokenT::eTextFragment, STRING_LITERAL("  macro */ ")));
        CHECK(   result[1] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        CHECK(   result[2] == TokenT( TokenT::eTextFragment, STRING_LITERAL(" / ")));
        CHECK(   result[3] == TokenT( TokenT::eNewLine, STRING_LITERAL("\n")));
        REQUIRE( result2.size() == 3);
        CHECK(   result2[0] == STRING_LITERAL("text\n"));
        CHECK(   result2[1] == STRING_LITERAL(" /* macro */ \n"));
        CHECK(   result2[2] == STRING_LITERAL(" /*/ \n"));
    }


}

TEST_CASE("TTokenizer", "[TTokenizer]")
{
    {
        typedef std::string StringT;
        typedef Token<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef Tokenizer<TokenT, StringT, OutputT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testInlineTemplateProcessing<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::string StringT;
        typedef Token<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef BackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef Token<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef Tokenizer<TokenT, StringT, OutputT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testInlineTemplateProcessing<TokenizerT, OutputT, TokenT, StringT>();
    }

    {
        typedef std::wstring StringT;
        typedef Token<Tokens, StringT> TokenT;    
        typedef CTokenizerTestHelper<TokenT, StringT> OutputT;
        typedef BackEndTokenizer<TokenT, StringT, OutputT> TokenizerT;

        test<TokenizerT, OutputT, TokenT, StringT>();
        testBackEndFeatures<TokenizerT, OutputT, TokenT, StringT>();
    }

}

