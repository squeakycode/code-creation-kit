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
#include "CVerticalTableBuilder.h"
#include <string>
#include <vector>

template <typename T>
void testBuilder( T& itemTable, const unsigned int rows, const unsigned int columns, bool pad)
{
    //create builder and container
    typedef std::vector<std::vector<std::string> >  TableT;
    TableT table;
    CVerticalTableBuilder<TableT> tableBuilder(table, pad);

    //fill in the test data
    for ( unsigned int row = 0; row < rows; ++row)
    {
        for ( unsigned int col = 0; col < columns; ++col)
        {
            if ( itemTable[row][col] == NULL)
            {
                break;
            }
            tableBuilder.addItem( itemTable[row][col]);
        }
        if ( row + 1 < rows) //not for last row
        {
            tableBuilder.addRow();
        }
    }
    tableBuilder.finished();

    //check if the data in the table and the array is the same
    BOOST_REQUIRE( table.size() == columns );
    for ( unsigned int row = 0; row < rows; ++row)
    {
        for ( unsigned int col = 0; col < columns; ++col)
        {
            BOOST_REQUIRE( table[col].size() == rows );
            BOOST_CHECK( table[col][row] == (itemTable[row][col] ? itemTable[row][col] : ""));
        }
    }
}

BOOST_AUTO_TEST_CASE( TVerticalTableBuilder)
{
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
        testBuilder( itemTable, rows, columns, false);
    }

    {
        //test data array, auto pad
        const unsigned int rows = 6;
        const unsigned int columns = 5;
        const char* itemTable[rows][columns] =
        {
            {NULL,NULL,NULL,NULL,NULL},
            {"",NULL,NULL,NULL,NULL},
            {"a5","b5","c5",NULL,NULL},
            {"\";","b2;\nb2","\"b3;\nb3\"","\"b3\"\nb3\"",";"},
            {"","","",NULL,NULL},
            {"a5","b5","c5","d5",NULL}
        };
        testBuilder( itemTable, rows, columns, true);
    }

    {
        //test data array, underflow in addRow
        const unsigned int rows = 3;
        const unsigned int columns = 2;
        const char* itemTable[rows][columns] =
        {
            {"a","b"},
            {"c",NULL},
            {"a5","b5"}
        };
        testBuilder( itemTable, rows, columns, true);
        BOOST_CHECK_THROW( testBuilder( itemTable, rows, columns, false), CVerticalTableBuilderExceptions::ExUnderflow);
    }

    {
        //test data array, overflow in addItem
        const unsigned int rows = 3;
        const unsigned int columns = 3;
        const char* itemTable[rows][columns] =
        {
            {"a","b", NULL},
            {"c","a5","b5"},
            {"a5","b5",NULL}
        };
        testBuilder( itemTable, rows, columns, true);
        BOOST_CHECK_THROW( testBuilder( itemTable, rows, columns, false), CVerticalTableBuilderExceptions::ExOverflow);
    }

    {
        //test data array, underflow in finished
        const unsigned int rows = 2;
        const unsigned int columns = 2;
        const char* itemTable[rows][columns] =
        {
            {"a","b"},
            {"c",NULL},
        };
        testBuilder( itemTable, rows, columns, true);
        BOOST_CHECK_THROW( testBuilder( itemTable, rows, columns, false), CVerticalTableBuilderExceptions::ExUnderflow);
    }

}

