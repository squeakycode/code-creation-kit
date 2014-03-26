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
#include <fstream>
#include <sstream>
#include "CCsvParser.h"
#include "CPositionTracker.h"

namespace test_data
{
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

    const unsigned int lineTable[rows][columns] =
    {
        {2,2,2,2,2},
        {3,3,3,3,3},
        {4,5,6,7,7},
        {8,8,8,8,8},
        {10,10,10,10,10}
    };

    const unsigned int columnTable[rows][columns] =
    {
        {3,6,9,12,15},
        {1,2,3,4,5},
        {6,4,6,6,10},
        {1,2,3,4,5},
        {3,6,9,12,17}
    };

    const char* itemTableDoubleQuote[2][3] =
    {
        {"\"a1","b\"1","c1\""},
        {"\"x","y\"","z"},
    };
}

///table builder test stub 
struct TCsvParserTableBuilder
{
    TCsvParserTableBuilder() : col(0), row(0), items(0), finishedCount(0) {}

    ///checks data and positions
    void addItem( const std::string& item)
    {
        BOOST_REQUIRE( row < test_data::rows );
        BOOST_REQUIRE( col < test_data::columns );

        BOOST_CHECK( item == test_data::itemTable[row][col]);
        BOOST_CHECK( positionTracker.getLine() == test_data::lineTable[row][col]);
        BOOST_CHECK( positionTracker.getColumn() == test_data::columnTable[row][col]);

        col++;
        items++;
    }

    ///checks row handling
    void addRow()
    {
        BOOST_CHECK( col == 5);
        row++;
        col = 0;
    }

    ///checks finished handling
    void finished()
    {
        finishedCount++;
    }

    unsigned int row;
    unsigned int col;
    unsigned int items;
    CPositionTracker positionTracker;
    int finishedCount;

    typedef std::string StringT;
};

///table builder test stub 
struct TCsvParserTableBuilderDoubleQuote
{
    TCsvParserTableBuilderDoubleQuote() : col(0), row(0), items(0), finishedCount(0) {}

    ///checks data and positions
    void addItem( const std::string& item)
    {
        BOOST_REQUIRE( row < 2 );
        BOOST_REQUIRE( col < 3 );

        BOOST_CHECK( item == test_data::itemTableDoubleQuote[row][col]);

        col++;
        items++;
    }

    ///checks row handling
    void addRow()
    {
        BOOST_CHECK( col == 3);
        row++;
        col = 0;
    }

    ///checks finished handling
    void finished()
    {
        finishedCount++;
    }

    unsigned int row;
    unsigned int col;
    unsigned int items;
    int finishedCount;

    typedef std::string StringT;
};

BOOST_AUTO_TEST_CASE( TCsvParser)
{
    //open test file
    std::ifstream file( "TCsvParser.csv");
    BOOST_CHECK( file.good() );

    //create parser
    CCsvParser parser;
    TCsvParserTableBuilder helper;
    (void) parser;

    //parse the file
    parser.parse( file, helper, ';', "#'", false, helper.positionTracker);

    //check parsing ok
    BOOST_CHECK( helper.row == test_data::rows );
    BOOST_CHECK( helper.items == test_data::columns * test_data::rows );
    BOOST_CHECK( helper.finishedCount == 1);

    //check file format error handling
    {
        std::stringstream s;
        s << "a\"a"; //a"a
        BOOST_CHECK_THROW( parser.parse( s, helper, ';', "", false), CCsvParser::ExUnexpectedQuote);
    }
    {
        std::stringstream s;
        s << "\"a\"a"; //"a"a
        BOOST_CHECK_THROW( parser.parse( s, helper, ';', "", false), CCsvParser::ExRequireDelimitingChar);
    }
    {
        std::stringstream s;
        BOOST_CHECK_THROW( parser.parse( s, helper, ';', "\"", false), CCsvParser::ExBadCommentChars);
        BOOST_CHECK_THROW( parser.checkCharsUsedForCommenting<std::string>( ";", ';', false, s), CCsvParser::ExBadCommentChars);
    }

    //check ignore double quotes
    {
        TCsvParserTableBuilderDoubleQuote helperDoubleQuote;
        std::stringstream s;
        s << "\"a1;b\"1;c1\"\n\"x;y\";z";
        parser.parse( s, helperDoubleQuote, ';', "", true);

        //check parsing ok
        BOOST_CHECK( helperDoubleQuote.row == 1 );
        BOOST_CHECK( helperDoubleQuote.items == 6 );
        BOOST_CHECK( helperDoubleQuote.finishedCount == 1);
    }
}
