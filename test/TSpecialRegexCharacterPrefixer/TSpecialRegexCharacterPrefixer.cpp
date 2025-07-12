// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "CSpecialRegexCharacterPrefixer.h"
#include <string>
#include <vector>

using namespace code_creation_kit;

TEST_CASE( "TSpecialRegexCharacterPrefixer", "[TSpecialRegexCharacterPrefixer]")
{
    {
        std::string s( "start.[{()\\*+?|^$end");
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( s);
        CHECK( s == "start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
    {
        std::wstring s( L"start.[{()\\*+?|^$end");
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( s);
        CHECK( s == L"start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
    {
        std::vector<std::string > list;
        list.push_back( std::string( "0start.[{()\\*+?|^$end"));
        list.push_back( std::string( "1start.[{()\\*+?|^$end"));
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( list);
        REQUIRE( list.size() == 2);
        CHECK( list[0] == "0start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
        CHECK( list[1] == "1start\\.\\[\\{\\(\\)\\\\\\*\\+\\?\\|\\^\\$end");
    }
}
