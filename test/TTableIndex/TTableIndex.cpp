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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "CTableIndex.h"
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
