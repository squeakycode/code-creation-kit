//   Copyright (C) 2011-2014 Andreas Gau
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

BOOST_AUTO_TEST_CASE( TConversions)
{
    testReplace<std::string, CReplaceConversion<std::string> >();
    testReplace<std::string, CRegexReplaceConversion<std::string> >();

    testMerge<std::string, CMergeConversion<std::string> >();

    testToCString<std::string, CToCStringConversion<std::string> >();
    testToCString<std::wstring, CToCStringConversion<std::wstring> >();
}

