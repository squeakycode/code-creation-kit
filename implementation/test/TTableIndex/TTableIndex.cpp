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
#include "CTableIndex.h"
#include <string>
#include <vector>

BOOST_AUTO_TEST_CASE( TTableIndex)
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

    BOOST_CHECK( tableIndex.getHeaderIndex() == 1);
    BOOST_REQUIRE( tableIndex.lookUp( "a1").size() == 2);
    BOOST_CHECK( tableIndex.lookUp( "a1").at(0) == 0);
    BOOST_CHECK( tableIndex.lookUp( "a1").at(1) == 2);
    BOOST_REQUIRE( tableIndex.lookUp( "d1").size() == 1);
    BOOST_CHECK( tableIndex.lookUp( "d1").at(0) == 3);

    tableIndex.readTable(0);

    BOOST_CHECK( tableIndex.getHeaderIndex() == 0);
    BOOST_REQUIRE( tableIndex.lookUp( "1").size() == 1);
    BOOST_CHECK( tableIndex.lookUp( "1").at(0) == 0);
    BOOST_REQUIRE( tableIndex.lookUp( "3").size() == 1);
    BOOST_CHECK( tableIndex.lookUp( "3").at(0) == 2);
}
