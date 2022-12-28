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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <string>
#include "Conversions.h"
#include <iostream>

using namespace code_creation_kit;

template <typename StringT, typename ReplaceConversionT>
void testReplace()
{
    { //replace no ignore case
        std::vector<StringT> v;
        v.push_back( "abc");
        v.push_back( "aBc");

        ReplaceConversionT replace( "b", "BB");

        replace.modify( v);
        BOOST_CHECK( v[0] == "aBBc");
        BOOST_CHECK( v[1] == "aBc");
    }

    { //replace ignore case
        std::vector<StringT> v;
        v.push_back( "abc");
        v.push_back( "aBc");

        ReplaceConversionT replace( "b", "BB");
        replace.ignoreCase(true);
        replace.modify( v);
        BOOST_CHECK( v[0] == "aBBc");
        BOOST_CHECK( v[1] == "aBBc");
    }

    { //test compare
        ReplaceConversionT replaceA( "b", "BB");
        ReplaceConversionT replaceB( "a", "AA");
        ReplaceConversionT replaceC( "b", "BB");

        IConversion<StringT>* modA = &replaceA;
        IConversion<StringT>* modB = &replaceB;
        IConversion<StringT>* modC = &replaceC;

        BOOST_CHECK( !(*modA == *modB));
        BOOST_CHECK( *modA == *modA);
        BOOST_CHECK( *modA == *modC);
    }

}

template <typename StringT, typename MergeConversionT>
void testMerge()
{
    { //replace no ignore case
        std::vector<StringT> v;
        v.push_back( "abc");
        v.push_back( "aBc");
        v.push_back( "1");
        MergeConversionT replace( ";");

        replace.modify( v);
        BOOST_CHECK( v[0] == "abc;aBc;1");
    }

    { //test compare
        MergeConversionT mergeA( "b");
        MergeConversionT mergeB( "a");
        MergeConversionT mergeC( "b");

        IConversion<StringT>* modA = &mergeA;
        IConversion<StringT>* modB = &mergeB;
        IConversion<StringT>* modC = &mergeC;

        BOOST_CHECK( !(*modA == *modB));
        BOOST_CHECK( *modA == *modA);
        BOOST_CHECK( *modA == *modC);
    }

}

template <typename StringT, typename ToCStringConversionT>
void testToCString()
{
    typedef typename StringT::value_type CharT;
    ToCStringConversionT toCString;

    std::vector<StringT> v;
    v.push_back( STRING_LITERAL("abc\'\"\?\\\a\b\f\n\r\t\v123"));

    toCString.modify( v);
    BOOST_CHECK( v[0] == STRING_LITERAL("abc\\'\\\"\\?\\\\\\a\\b\\f\\n\\r\\t\\v123"));
}

template <typename StringT, typename ConversionT>
void testHtmlEscape()
{
    typedef typename StringT::value_type CharT;
    ConversionT htmlEscape;

    std::vector<StringT> v;
    v.push_back(STRING_LITERAL("&, <, >, \", ', `, (, ), {, }, [, ], !, @, $, %, =, +,"));

    htmlEscape.modify(v);
    BOOST_CHECK(v[0] == STRING_LITERAL("&amp;, &lt;, &gt;, &quot;, &#39;, &#96;, &#40;, &#41;, &#123;, &#125;, &#91;, &#93;, &#33;, &#64;, &#36;, &#37;, &#61;, &#43;,"));
}

template <typename StringT, typename ConversionT>
void testPad(const StringT& in, const StringT& out, const StringT& padText, const StringT& padUpToWidth1, const StringT& padUpToWidth2)
{
    {
        std::vector<StringT> dummy;
        if (!padUpToWidth2.empty())
        {
            dummy.push_back(padUpToWidth2);
        }
        ConversionT pad(padText, padUpToWidth1, dummy.begin(), dummy.end());

        std::vector<StringT> v;
        v.push_back(in);

        pad.modify(v);
        if (v[0] != out)
        {
            BOOST_CHECK(v[0] == out);
        }
    }
}

template <typename StringT>
void testPad()
{
    typedef typename StringT::value_type CharT;
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("  123456"), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("   12345\n       6\n        "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("   12345\n 6\n  "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n 6\n  "), STRING_LITERAL(" "), STRING_LITERAL("4"), STRING_LITERAL("2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("abc123456"), STRING_LITERAL("abc "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("abc   123456"), STRING_LITERAL("abc "), STRING_LITERAL("12"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("abc12345\nabc    6\nabc     "), STRING_LITERAL("abc "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("abb12345\na6\nab"), STRING_LITERAL("ab"), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("ab12345\nab6\nab"), STRING_LITERAL("abc"), STRING_LITERAL("4"), STRING_LITERAL("2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("abc     12345\nabc6\nabc"), STRING_LITERAL("abc "), STRING_LITERAL("+8"), STRING_LITERAL("+2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n6\n"), STRING_LITERAL("abc "), STRING_LITERAL("0"), STRING_LITERAL("+0"));

    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("123456  "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345   \n6       \n        "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345   \n6 \n  "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n6 \n  "), STRING_LITERAL(" "), STRING_LITERAL("4"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("123456   abc"), STRING_LITERAL(" abc"), STRING_LITERAL("12"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345abc\n6    abc\n     abc"), STRING_LITERAL(" abc"), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345aab\n6b\nab"), STRING_LITERAL("ab"), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345bc\n6bc\nbc"), STRING_LITERAL("abc"), STRING_LITERAL("4"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345     abc\n6abc\nabc"), STRING_LITERAL(" abc"), STRING_LITERAL("+8"), STRING_LITERAL("+2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n6\n"), STRING_LITERAL(" abc"), STRING_LITERAL("0"), STRING_LITERAL("+0"));

    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("1\t6"), STRING_LITERAL("  1\t6"), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
}

template <typename StringT>
void testBlockFormat(const StringT& in, const StringT& out, const StringT& blockWidth)
{
    {
        CBlockFormatConversion<StringT> bf(blockWidth);

        std::vector<StringT> v;
        v.push_back(in);

        bf.modify(v);
        if (v[0] != out)
        {
            BOOST_CHECK(v[0] == out);
        }
    }
}

template <typename StringT>
void testBlockFormat()
{
    typedef typename StringT::value_type CharT;
    testBlockFormat<StringT>(STRING_LITERAL("123456"), STRING_LITERAL("123456"), STRING_LITERAL("0"));
    testBlockFormat<StringT>(STRING_LITERAL("123456"), STRING_LITERAL("12\n34\n56"), STRING_LITERAL("2"));
    testBlockFormat<StringT>(STRING_LITERAL("123456"), STRING_LITERAL("1\n2\n3\n4\n5\n6"), STRING_LITERAL("1"));
    testBlockFormat<StringT>(STRING_LITERAL("1234\n56"), STRING_LITERAL("12\n34\n56"), STRING_LITERAL("2"));
    testBlockFormat<StringT>(STRING_LITERAL("1234 5 6"), STRING_LITERAL("1234\n5 6"), STRING_LITERAL("5"));
    testBlockFormat<StringT>(STRING_LITERAL("1234\t5 6"), STRING_LITERAL("1234\n5 6"), STRING_LITERAL("5"));
    testBlockFormat<StringT>(STRING_LITERAL("1234 6 8 0 2"), STRING_LITERAL("1234 6 8\n0 2"), STRING_LITERAL("8"));
    testBlockFormat<StringT>(STRING_LITERAL("1234 6 8\t0 2"), STRING_LITERAL("1234 6 8\n0 2"), STRING_LITERAL("8"));
    testBlockFormat<StringT>(STRING_LITERAL("1234 6 8 0 2"), STRING_LITERAL("1234 6 8 0 2"), STRING_LITERAL("80"));
    testBlockFormat<StringT>(STRING_LITERAL("1234 6\n 8 0 2"), STRING_LITERAL("1234 6\n 8 0 2"), STRING_LITERAL("80"));
}

void toErrorStream(std::string text1, std::string text2)
{
    std::cerr << text1 << text2 << std::endl;
}

void toErrorStream(std::wstring text1, std::wstring text2)
{
    std::wcerr << text1 << text2 << std::endl;
}

template <typename StringT>
void testCalculation(const std::string& in, const std::string& out, const std::string& expression)
{
    typedef typename StringT::value_type CharT;
    {
        CCalcConversion<StringT> calc(StringConvert<StringT>(expression));

        std::vector<StringT> v;
        v.push_back(StringConvert<StringT>(in));

        calc.modify(v);

        bool result = v[0] == StringConvert<StringT>(out);
        if (!result)
        {
            toErrorStream(STRING_LITERAL("Expression: "), StringConvert<StringT>(expression));
            toErrorStream(STRING_LITERAL("Expected: "), StringConvert<StringT>(out));
            toErrorStream(STRING_LITERAL("Result: "), v[0]);
        }
        BOOST_CHECK(result);
    }
}


template <typename StringT>
void testCalculation()
{
    typedef typename StringT::value_type CharT;
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "a/0"), CCalcConversionExceptions::ExDivisionByZero);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "a%0"), CCalcConversionExceptions::ExDivisionByZero);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "a+02"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "#a+2"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "#a+2"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "a#+2"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "a+2#"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "b+2#"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("a1", "3", "a+1"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "b*"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "b**"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);
    BOOST_CHECK_THROW(testCalculation<StringT>("1", "3", "*b"), CCalcConversionExceptions::ExArithmeticExpressionSyntaxError);

    testCalculation<StringT>("1", "3", "a+2");
    testCalculation<StringT>("1", "304", "a+2+300+1");
    testCalculation<StringT>("10", "-25", "a-35");
    testCalculation<StringT>("142", "71", "a/2");
    testCalculation<StringT>("142", "284", "a*2");
    testCalculation<StringT>("142", "2", "a%10");
    testCalculation<StringT>("142", "-142", "-a");
    testCalculation<StringT>("142", "-420", "(-a+2)*3");
    testCalculation<StringT>("142", "-420", "(((-a+2))*3)");
    testCalculation<StringT>("2", "22", "a+2*10");
    testCalculation<StringT>("2", "22", "2*10+a");
    testCalculation<StringT>("xyz", "20", "2*10");
    testCalculation<StringT>("xyz", "20", "-2*-10");
    testCalculation<StringT>("xyz", std::to_string(312 % 100 / 2 * 5 - 4 + 1), "312%100/2*5-4+1");
    testCalculation<StringT>("xyz", std::to_string(1 + 2 - 5 * 100 / 52 % 10), "1+2-5*100/52%10");
    testCalculation<StringT>("xyz", std::to_string(1 + 2 - 5 * 100 / 52 % 10), " 1 + 2 - 5 * 100 / 52 % 10 ");
    testCalculation<StringT>("xyz", std::to_string(+ 1 + + 2 - + 5 * + 100 / + 52 % + 10), " + 1 + + 2 - + 5 * + 100 / + 52 % + 10 ");
    testCalculation<StringT>("xyz", std::to_string( - 1 + - 2 - - 5 * - 100 / - 52 % - 10 ), " - 1 + - 2 - - 5 * - 100 / - 52 % - 10 ");
    testCalculation<StringT>("4", std::to_string(4 + 10 / 5 * 3- -6 % 2 + 1), "a+10/5*3--6%2+1");
    testCalculation<StringT>("1+2-5*100/52%10", std::to_string((1 + 2 - 5 * 100 / 52 % 10) + 10 / 5 * 3 - -6 % 2 + 1), "a+10/5*3--6%2+1");
    testCalculation<StringT>("xyz", std::to_string(-(4 + 5) * +(-2 * -10)), "-(4+5)*+(-2*-10)");
}


template <typename StringT>
void testToCsv(const StringT& in, const StringT& out, const StringT& csvDelimiterChars, const StringT& csvQuoteChars, bool single = false)
{
    std::vector<StringT> v;
    v.push_back(in);

    typedef typename StringT::value_type CharT;
    {
        if (single)
        {
            CToCsvConversion<StringT> toCsv(csvDelimiterChars);
            toCsv.modify(v);
        }
        else
        {
            CToCsvConversion<StringT> toCsv(csvDelimiterChars, csvQuoteChars);
            toCsv.modify(v);
        }

        bool result = v[0] == out;
        if (!result)
        {
            toErrorStream(STRING_LITERAL("CsvDelimiterChars: "), csvDelimiterChars);
            toErrorStream(STRING_LITERAL("csvQuoteChars: "), csvQuoteChars);
            toErrorStream(STRING_LITERAL("Expected: "), out);
            toErrorStream(STRING_LITERAL("Result: "), v[0]);
        }
        BOOST_CHECK(result);
    }
}


template <typename StringT>
void testToCsv()
{
    typedef typename StringT::value_type CharT;
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("\"a;b\""), STRING_LITERAL(";"), STRING_LITERAL(""), true);
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("\"a;b\""), STRING_LITERAL(";"), STRING_LITERAL(""), true);
    testToCsv<StringT>(STRING_LITERAL("ab"), STRING_LITERAL("ab"), STRING_LITERAL(";"), STRING_LITERAL(""), true);

    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("\"a;b\""), STRING_LITERAL(";"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("\"a;b\""), STRING_LITERAL(";"), STRING_LITERAL("\"ab"));
    testToCsv<StringT>(STRING_LITERAL("a\nb"), STRING_LITERAL("\"a\nb\""), STRING_LITERAL(";"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;\nb"), STRING_LITERAL("\"a;\nb\""), STRING_LITERAL(";"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("a;b"), STRING_LITERAL("#"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("\"a;b\""), STRING_LITERAL("#b"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;\"b"), STRING_LITERAL("\"a;\"\"b\""), STRING_LITERAL("#b"), STRING_LITERAL("\""));
    testToCsv<StringT>(STRING_LITERAL("a;b"), STRING_LITERAL("a;b"), STRING_LITERAL(";"), STRING_LITERAL("")); //no quote no action
    testToCsv<StringT>(STRING_LITERAL("a\nb"), STRING_LITERAL("\"a\nb\""), STRING_LITERAL(""), STRING_LITERAL("\"")); //no delimiter but new line
    testToCsv<StringT>(STRING_LITERAL("ab"), STRING_LITERAL("babbb"), STRING_LITERAL(""), STRING_LITERAL("b"));
    testToCsv<StringT>(STRING_LITERAL("abc"), STRING_LITERAL("#abc#"), STRING_LITERAL(""), STRING_LITERAL("#c"));
}

BOOST_AUTO_TEST_CASE( TConversions)
{
    testReplace<std::string, CReplaceConversion<std::string> >();
    testReplace<std::string, CRegexReplaceConversion<std::string> >();

    testMerge<std::string, CMergeConversion<std::string> >();

    testToCString<std::string, CToCStringConversion<std::string> >();
    testToCString<std::wstring, CToCStringConversion<std::wstring> >();

    testHtmlEscape<std::string, CHtmlEscapeConversion<std::string> >();
    testHtmlEscape<std::wstring, CHtmlEscapeConversion<std::wstring> >();

    testPad<std::string>();
    testPad<std::wstring>();

    testBlockFormat<std::string>();
    testBlockFormat<std::wstring>();

    testCalculation<std::string>();
    testCalculation<std::wstring>();

    testToCsv<std::string>();
    testToCsv<std::wstring>();
}

