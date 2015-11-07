//  Copyright (c) 2011-2015 Andreas Gau
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

