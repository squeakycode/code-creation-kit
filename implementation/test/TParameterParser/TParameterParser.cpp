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

using namespace code_creation_kit;

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
    size_t lastProcessed = 0;
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
