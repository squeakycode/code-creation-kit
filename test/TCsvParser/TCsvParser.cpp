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

#include <string>
#include <fstream>
#include <sstream>
#include "CCsvParser.h"
#include "CPositionTracker.h"
#include "TCsvParserTestFiles.h"

using namespace code_creation_kit;

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
    TCsvParserTableBuilder() : row(0), col(0), items(0), finishedCount(0) {}

    ///checks data and positions
    void addItem( const std::string& item)
    {
        REQUIRE( row < test_data::rows );
        REQUIRE( col < test_data::columns );

        CHECK( item == test_data::itemTable[row][col]);
        CHECK( positionTracker.getLine() == test_data::lineTable[row][col]);
        CHECK( positionTracker.getColumn() == test_data::columnTable[row][col]);

        col++;
        items++;
    }

    ///checks row handling
    void addRow()
    {
        CHECK( col == 5);
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
    TCsvParserTableBuilderDoubleQuote() : row(0), col(0), items(0), finishedCount(0) {}

    ///checks data and positions
    void addItem( const std::string& item)
    {
        REQUIRE( row < 2 );
        REQUIRE( col < 3 );

        CHECK( item == test_data::itemTableDoubleQuote[row][col]);

        col++;
        items++;
    }

    ///checks row handling
    void addRow()
    {
        CHECK( col == 3);
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

TEST_CASE( "TCsvParser", "[TCsvParser]")
{
    CHECK_NOTHROW(CreateTCsvParserFiles());

    //open test file
    std::ifstream file( CCK_TEST_INPUT_FILE_PREFIX "TCsvParser.csv");
    CHECK( file.good() );

    //create parser
    CCsvParser parser;
    TCsvParserTableBuilder helper;
    (void) parser;

    //parse the file
    parser.parse( file, helper, ";", "\"", "#'", helper.positionTracker);

    //check parsing ok
    CHECK( helper.row == test_data::rows );
    CHECK( helper.items == test_data::columns * test_data::rows );
    CHECK( helper.finishedCount == 1);

    //check file format error handling
    {
        std::stringstream s;
        s << "a\"a"; //a"a
        CHECK_THROWS_AS( parser.parse( s, helper, ";", "\"", ""), CCsvParser::ExUnexpectedQuote);
    }
    {
        std::stringstream s;
        s << "\"a\"a"; //"a"a
        CHECK_THROWS_AS( parser.parse( s, helper, ";", "\"", ""), CCsvParser::ExRequireDelimitingChar);
    }
    {
        std::stringstream s;
        CHECK_THROWS_AS( parser.parse( s, helper, ";", "\"", "\n"), CCsvParser::ExBadCommentChars);
        CHECK_THROWS_AS( parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Commenting, "a", "b;", "c;"), CCsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Commenting, "a;", "b", "c;"), CCsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Commenting, "a", "b", "\n"), CCsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Commenting, "a", "b", "\r"), CCsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Delimiter, "a;", "b;", "c"), CCsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Delimiter, "a;", "b", "c;"), CCsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Delimiter, "\n", "b", "c"), CCsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Delimiter, "\r", "b", "c"), CCsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Quote, "a", "b;", "c;"), CCsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Quote, "a;", "b;", "c"), CCsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Quote, "a", "\n", "c"), CCsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(parser.checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Quote, "a", "\r", "c"), CCsvParser::ExBadQuoteChars);
    }

    //check ignore double quotes
    {
        TCsvParserTableBuilderDoubleQuote helperDoubleQuote;
        std::stringstream s;
        s << "\"a1;b\"1;c1\"\n\"x;y\";z";
        parser.parse( s, helperDoubleQuote, ";", "", "");

        //check parsing ok
        CHECK( helperDoubleQuote.row == 1 );
        CHECK( helperDoubleQuote.items == 6 );
        CHECK( helperDoubleQuote.finishedCount == 1);
    }
}
