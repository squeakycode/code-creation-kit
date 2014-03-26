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
#include <vector>
#include "ParameterParser.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 )
#endif
#include <boost/assign.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

BOOST_AUTO_TEST_CASE( TParameterParser)
{
    using namespace boost::assign;

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
    std::vector<std::string> parameterList;
    parameterList += "c=y", "b=1", "c=z", "a=x", "b=2", "b=3";

    //parse the list
    unsigned int lastProcessed;
    ParameterParser::parse( parameterList, table, lastProcessed,'=');

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
