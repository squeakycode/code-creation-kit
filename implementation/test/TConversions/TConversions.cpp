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

#define BOOST_TEST_MAIN
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>
#include <string>
#include "Conversions.h"

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
    BOOST_CHECK(v[0] == STRING_LITERAL("&amp;, &lt, &gt, &quot, &#39;, &#96;, &#40;, &#41;, &#123;, &#125;, &#91;, &#93;, &#33;, &#64;, &#36;, &#37;, &#61;, &#43;,"));
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
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("ab123456"), STRING_LITERAL("abc"), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("abc12345\nabcabca6\nabcabcab"), STRING_LITERAL("abc"), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("aba12345\na6\nab"), STRING_LITERAL("ab"), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadLeftConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\na6\nab"), STRING_LITERAL("abc"), STRING_LITERAL("4"), STRING_LITERAL("2"));

    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("123456  "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345   \n6       \n        "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345   \n6 \n  "), STRING_LITERAL(" "), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n6 \n  "), STRING_LITERAL(" "), STRING_LITERAL("4"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("123456"), STRING_LITERAL("123456ab"), STRING_LITERAL("abc"), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345abc\n6abcabca\nabcabcab"), STRING_LITERAL("abc"), STRING_LITERAL("8"), STRING_LITERAL(""));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345aba\n6a\nab"), STRING_LITERAL("ab"), STRING_LITERAL("8"), STRING_LITERAL("2"));
    testPad<StringT, CPadRightConversion<StringT> >(STRING_LITERAL("12345\n6\n"), STRING_LITERAL("12345\n6a\nab"), STRING_LITERAL("abc"), STRING_LITERAL("4"), STRING_LITERAL("2"));

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
}

