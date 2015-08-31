//   Copyright (C) 2011 Andreas Gau
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
#include "boost/test/unit_test.hpp"
#include "CVerticalTableBuilder.h"
#include <string>
#include <fstream>
#include <vector>

BOOST_AUTO_TEST_CASE( TVerticalTableBuilder)
{
    //test data array
    const unsigned int rows = 5;
    const unsigned int columns = 5;
    const char* itemTable[rows][columns] =
    {
        {"a1","b1","c1","d1","e1"},
        {"","","","",""},
        {"\";","b2;\nb2","\"b3;\nb3\"","\"b3\"\nb3\"",";"},
        {"","","","",""},
        {"a5","b5","c5","d5","e5"}
    };

    //create builder and container
    typedef std::vector<std::vector<std::string> >  TableT;
    TableT table;
    CVerticalTableBuilder<TableT> tableBuilder(table);

    //fill in the test data
    for ( unsigned int row = 0; row < rows; ++row)
    {
        for ( unsigned int col = 0; col < columns; ++col)
        {
            tableBuilder.addItem( itemTable[row][col]);
        }
        tableBuilder.addRow();
    }
    tableBuilder.finished();

    //check if the data in the table and the array is the same
    BOOST_REQUIRE( table.size() == columns );
    for ( unsigned int row = 0; row < rows; ++row)
    {
        for ( unsigned int col = 0; col < columns; ++col)
        {
            BOOST_REQUIRE( table[col].size() == rows );
            BOOST_CHECK( table[col][row] == itemTable[row][col] );
        }
    }
}

