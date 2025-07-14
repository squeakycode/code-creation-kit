// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <sstream>

#include "CsvParser.h"
#include "PositionTracker.h"

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

    const char* itemTableEmptyLastItemOnEof[2][2] =
    {
        {"a1","b1"},
        {"x" ,""}
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
    PositionTracker positionTracker;
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

///table builder test stub 
struct TCsvParserTableBuilderEmptyLastItemOnEof
{
    TCsvParserTableBuilderEmptyLastItemOnEof() : row(0), col(0), items(0), finishedCount(0) {}

    ///checks data and positions
    void addItem(const std::string& item)
    {
        REQUIRE(row < 2);
        REQUIRE(col < 2);

        CHECK(item == test_data::itemTableEmptyLastItemOnEof[row][col]);

        col++;
        items++;
    }

    ///checks row handling
    void addRow()
    {
        CHECK(col == 2);
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
    const char* fileData =
R"('comment
a1;b1;c1;d1;e1
;;;;
""";";"b2;
b2";"""b3;
b3""";"""b3""
b3""";";"
;;;;
#comment
a5;b5;c5;d5;"e5"
)";

    //open test file
    std::istringstream file(fileData);

    //create parser
    TCsvParserTableBuilder helper;

    //parse the file
    CsvParser::parse( file, helper, ";", "\"", "#'", helper.positionTracker);

    //check parsing ok
    CHECK( helper.row == test_data::rows );
    CHECK( helper.items == test_data::columns * test_data::rows );
    CHECK( helper.finishedCount == 1);

    //check file format error handling
    {
        std::stringstream s;
        s << "a\"a"; //a"a
        CHECK_THROWS_AS( CsvParser::parse( s, helper, ";", "\"", ""), CsvParser::ExUnexpectedQuote);
    }
    {
        std::stringstream s;
        s << "\"a\"a"; //"a"a
        CHECK_THROWS_AS( CsvParser::parse( s, helper, ";", "\"", ""), CsvParser::ExRequireDelimitingChar);
    }
    {
        std::stringstream s;
        CHECK_THROWS_AS(CsvParser::parse( s, helper, ";", "\"", "\n"), CsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Commenting, "a", "b;", "c;"), CsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Commenting, "a;", "b", "c;"), CsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Commenting, "a", "b", "\n"), CsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Commenting, "a", "b", "\r"), CsvParser::ExBadCommentChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Delimiter, "a;", "b;", "c"), CsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Delimiter, "a;", "b", "c;"), CsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Delimiter, "\n", "b", "c"), CsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Delimiter, "\r", "b", "c"), CsvParser::ExBadDelimiter);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Quote, "a", "b;", "c;"), CsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Quote, "a;", "b;", "c"), CsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Quote, "a", "\n", "c"), CsvParser::ExBadQuoteChars);
        CHECK_THROWS_AS(CsvParser::checkCharsUsedForCsvParsing<std::string>(CsvParser::UsedCsvCharsCheck_Quote, "a", "\r", "c"), CsvParser::ExBadQuoteChars);
    }

    //check ignore double quotes
    {
        TCsvParserTableBuilderDoubleQuote helperDoubleQuote;
        std::stringstream s;
        s << "\"a1;b\"1;c1\"\n\"x;y\";z";
        CsvParser::parse( s, helperDoubleQuote, ";", "", "");

        //check parsing ok
        CHECK( helperDoubleQuote.row == 1 );
        CHECK( helperDoubleQuote.items == 6 );
        CHECK( helperDoubleQuote.finishedCount == 1);
    }

    //check empty last item when eof
    {
        TCsvParserTableBuilderEmptyLastItemOnEof helperEmptyLastItemOnEof;
        std::stringstream s;
        s << "a1;b1\nx;";
        CsvParser::parse(s, helperEmptyLastItemOnEof, ";", "", "");

        //check parsing ok
        CHECK(helperEmptyLastItemOnEof.row == 1);
        CHECK(helperEmptyLastItemOnEof.items == 4);
        CHECK(helperEmptyLastItemOnEof.finishedCount == 1);
    }
}

TEST_CASE( "TCsvParser with UTF-8 BOM", "[TCsvParser]")
{
    const char* fileData = "\xEF\xBB\xBF"
R"('comment
a1;b1;c1;d1;e1
;;;;
""";";"b2;
b2";"""b3;
b3""";"""b3""
b3""";";"
;;;;
#comment
a5;b5;c5;d5;"e5"
)";

    //open test file
    std::istringstream file(fileData);

    //create parser
    TCsvParserTableBuilder helper;

    //parse the file
    CsvParser::parse( file, helper, ";", "\"", "#'", helper.positionTracker);

    //check parsing ok
    CHECK( helper.row == test_data::rows );
    CHECK( helper.items == test_data::columns * test_data::rows );
    CHECK( helper.finishedCount == 1);
}
