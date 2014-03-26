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
#include "CSpecialRegexCharacterPrefixer.h"
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE( TSpecialRegexCharacterPrefixer)
{
    {
        std::string s( "start.[{()\\*+?|^$end");
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( s);
        BOOST_CHECK( s == "start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
    {
        std::wstring s( L"start.[{()\\*+?|^$end");
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( s);
        BOOST_CHECK( s == L"start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
    {
        std::vector<std::string > list;
        list.push_back( std::string( "0start.[{()\\*+?|^$end"));
        list.push_back( std::string( "1start.[{()\\*+?|^$end"));
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( list);
        BOOST_REQUIRE( list.size() == 2);
        BOOST_CHECK( list[0] == "0start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
        BOOST_CHECK( list[1] == "1start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
}
