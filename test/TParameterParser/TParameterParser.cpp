// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <vector>
#include "ParameterParser.h"

using namespace code_creation_kit;

TEST_CASE( "TParameterParser", "[TParameterParser]")
{
    //expected output data
    const unsigned int rows = 4;
    const unsigned int columns = 3;

    const char* itemTable[rows][columns] =
    {
        {"a","b","c"},
        {"x","1","y"},
        {"","2","z"},
        {"","3",""},
    };

    //types used in test
    typedef std::vector<std::vector<std::string> >  TableT;

    //create table and parameter list
    TableT table;
    std::vector<std::string> parameterList = { "c=y", "b=1", "c=z", "a=x", "b=2", "b=3" };

    //parse the list
    size_t lastProcessed = 0;
    ParameterParser::parse( parameterList, table, lastProcessed,'=');

    //check if the data in the table and the array is the same
    REQUIRE( table.size() == columns );
    for ( unsigned int row = 0; row < rows; ++row)
    {
        for ( unsigned int col = 0; col < columns; ++col)
        {
            REQUIRE( table[col].size() == rows );
            CHECK( table[col][row] == itemTable[row][col] );
        }
    }
}
