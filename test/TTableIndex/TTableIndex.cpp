// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "TableIndex.h"
#include <string>
#include <vector>

using namespace code_creation_kit;

TEST_CASE( "TTableIndex", "[TTableIndex]")
{
    //test data array
    const unsigned int rows = 1;
    const unsigned int columns = 5;
    const char* itemTable[rows][columns] =
    {
        {"a1","b1","a1","d1","e1"},
    };

    //create the container
    typedef std::vector<std::vector<std::string> >  TableT;
    TableT table;

    //fill a test table
    table.resize( columns);
    for ( unsigned int col = 0; col < columns; ++col)
    {
        table[col].resize( rows);
        for ( unsigned int row = 0; row < rows; ++row)	
        {
            table[col][row] = itemTable[row][col];
        }
    }

    //create
    CTableIndex<TableT, std::exception> tableIndex;
    //connect test table
    tableIndex.connectTable( &table);
    tableIndex.readTable(1);

    CHECK( tableIndex.getHeaderIndex() == 1);
    REQUIRE( tableIndex.lookUp( "a1").size() == 2);
    CHECK( tableIndex.lookUp( "a1").at(0) == 0);
    CHECK( tableIndex.lookUp( "a1").at(1) == 2);
    REQUIRE( tableIndex.lookUp( "d1").size() == 1);
    CHECK( tableIndex.lookUp( "d1").at(0) == 3);

    tableIndex.readTable(0);

    CHECK( tableIndex.getHeaderIndex() == 0);
    REQUIRE( tableIndex.lookUp( "1").size() == 1);
    CHECK( tableIndex.lookUp( "1").at(0) == 0);
    REQUIRE( tableIndex.lookUp( "3").size() == 1);
    CHECK( tableIndex.lookUp( "3").at(0) == 2);
}
