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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "CTemplateProcessor.h"
#include <sstream>
#include <iostream>

using namespace code_creation_kit;

///represents streams in test
template <typename StringT>
class TStreamHelper
{
public:
    template <typename InputT>
    TStreamHelper<StringT>& operator <<(const InputT& text)
    {
        result.push_back( boost::lexical_cast<StringT>(text));
        return *this;
    }
    std::vector<StringT> result; 
};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 )
#endif
template <typename StringT, typename WrappedT>
class TStreamConversionWrapper
{
public:
    TStreamConversionWrapper( WrappedT& wrapped) : m_wrapped(wrapped) {}

    template <typename InputT>
    TStreamConversionWrapper<StringT, WrappedT>& operator <<(const InputT& text)
    {
        m_wrapped << boost::lexical_cast<StringT>(text);
        return *this;
    }
    WrappedT& m_wrapped; 
};
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

static const unsigned int rows = 5;
static const unsigned int columns = 7;

static const char* cItemTable[rows][columns] =
{
    {"Item","Type","Name","Array Maximum","Default","Description","Description"},
    {"a","int","valueCount","","0","",""},
    {"b","double","values","30","5.4","description","&more"},
    {"c","bool","valid","","false","",""},
    {"d","bool","test","","false","",""}
};

///serves as test template loader
template <typename StringT>
class TestTemplateLoader
{
public:
    TestTemplateLoader(): m_count(0), m_csvCount(0) {}

    ///dummy with no functionality
    void loadTemplateFile( const StringT& filename)
    {
        m_count++;
        m_filename = filename;
    }

    ///used when a CSV table is loaded using TABLE_LOAD
    StringT resolveFileNameForTableToLoad(const StringT& filename)
    {
        m_csvFilename = filename;
        ++m_csvCount;
        return m_csvFilename;
    }

    int m_count;
    StringT m_filename;

    int m_csvCount;
    StringT m_csvFilename;
};

template< typename ProcessorT>
bool test( ProcessorT& processor, const std::string& in, const std::string& out, bool newLineSplit = false)
{
    std::stringstream str;
    processor.connectOutputStream(&str);
    processor.open();
    if (newLineSplit)
    {
        for (boost::algorithm::split_iterator<std::string::const_iterator> it
            = make_split_iterator(in, token_finder(
                boost::algorithm::is_any_of("\n"),
                boost::algorithm::token_compress_off));
            ;
            )
        {
            std::string txt(it->begin(), it->end());
            ++it;
            if (it != boost::algorithm::split_iterator<std::string::const_iterator>())
            {
                processor << (txt + "\n");
            }
            else
            {
                processor << txt;
                break;
            }
        }
    }
    else
    {
        processor << in;
    }
    processor.close();
    std::string resultString = str.str();
    bool result = resultString == out;
    if ( !result )
    {
        std::cerr << "Expected: " << out << std::endl;
        std::cerr << "Result: " << resultString << std::endl;
        return false;
    }
    return result;
}

template< typename StringT>
void testMacroProcessing()
{
    typedef std::vector<std::vector<StringT> > TableT;
    typedef std::stringstream StreamT;
    typedef CTemplateProcessor<TableT, StreamT> ProcessorT;
    typedef std::shared_ptr<TableT> SharedTableT;
    typedef std::shared_ptr<const TableT> SharedConstTableT;

    SharedTableT ptrTable = std::make_shared<TableT>();

    ptrTable->resize(columns);
    for (unsigned int col = 0; col < columns; ++col)
    {
        (*ptrTable)[col].resize(rows);
        for (unsigned int row = 0; row < rows; ++row)
        {
            (*ptrTable)[col][row] = boost::lexical_cast<StringT>(cItemTable[row][col]);
        }
    }

    ProcessorT processor;

    //check too large macro check is working
    if ( processor.getMaxMacroTextSizeBytes() < 20 * 1024 * 1024)
    {
        StringT largeString( processor.getMaxMacroTextSizeBytes() + 1, 'a');
        BOOST_CHECK_THROW( test( processor, largeString, ""), CParserExceptions::ExMacroTooLarge);
    }

    processor.connectTable( ptrTable, "label A", true, true, 1, 1, false);

    //error handling
    BOOST_CHECK_THROW( test( processor, "a[MACRO_BEGIN][END]", ""), CParserExceptions::ExMissingBlockBegin);
    BOOST_CHECK_THROW( test( processor, "a[BEGIN]", ""), CParserExceptions::ExMissingBlockEnd);
    BOOST_CHECK_THROW( test( processor, "a[MACRO_BEGIN]", ""), CParserExceptions::ExMissingMacroEnd);
    BOOST_CHECK_THROW( test( processor, "a[BEGIN][MACRO_END]", ""), CParserExceptions::ExMissingBlockEnd);
    BOOST_CHECK_THROW( test( processor, "a[BEGIN]b[END]c[END]d", ""), CParserExceptions::ExMissingBlockBegin);
    BOOST_CHECK_THROW( test( processor, "a[BEGIN]b[END]c[MACRO_END]d", ""), CParserExceptions::ExMissingMacroBegin);
    BOOST_CHECK_THROW( test( processor, "a[ENTRY][\"Type\"][NOT]", ""), CParserExceptions::ExConstraintExpectedAfterNot);

    BOOST_CHECK_THROW( test( processor, "a[ENTRY][\"Type\"][ANY][READ_TOP_DOWN]", ""), CParserExceptions::ExSubstitutionParsingBadOrder);
    BOOST_CHECK_THROW( test( processor, "a[INDEX][READ_TOP_DOWN]", ""), CParserExceptions::ExCannotApplyDirectiveToSubstitution);
    BOOST_CHECK_THROW( test( processor, "a[ENTRY][\"Type\"][ANY][IGNORE_CASE]", ""), CParserExceptions::ExCannotApplyDirectiveToConstraint);
    BOOST_CHECK_THROW( test( processor, "a[IF][ENTRY][\"Type\"][TO_UPPER][IGNORE_CASE]", ""), CParserExceptions::ExCannotApplyDirectiveToConversion);
    BOOST_CHECK_THROW( test( processor, "a[IF][INDEX][ANY]", ""), CParserExceptions::ExCannotApplyConstraintToSubstitution);
    BOOST_CHECK_THROW( test( processor, "a[INDEX][TO_UPPER]", ""), CParserExceptions::ExCannotApplyConversionToSubstitution);
    BOOST_CHECK_THROW( test( processor, "a[ENTRY][\"Type\"][READ_TOP_DOWN][READ_TOP_DOWN]", ""), CParserExceptions::ExDirectiveAlreadyApplied);
    BOOST_CHECK_THROW( test( processor, "a[LAST_TIME]b", ""), CParserExceptions::ExSubstitutionRequiresIf);

    //parts
    BOOST_CHECK_THROW(test(processor, R"(a[PART_BEGIN]["l"]b)", ""), CParserExceptions::ExMissingPartEnd);
    BOOST_CHECK_THROW(test(processor, R"(a[PART_END]b)", ""), CParserExceptions::ExMissingPartBegin);
    BOOST_CHECK_THROW(test(processor, R"(a[PART_BEGIN]["l1"]b[PART_BEGIN]["l2"]c)", ""), CParserExceptions::ExPartBlocksCannotBeNested);
    BOOST_CHECK_THROW(test(processor, R"(a[PART_BEGIN]["l1"]b[PART_END]c[PART_BEGIN]["l1"]d[PART_END]e)", ""), CParserExceptions::ExPartAlreadyDefined);
    BOOST_CHECK_THROW(test(processor, R"(a[PART_BEGIN]["l1"]b[PART_END]c[PART_REMOVE]["l1"]d[PART]["l1"]e)", ""), CParserExceptions::ExPartNotDefined);
    BOOST_CHECK_THROW(test(processor, R"(a[PART]["l1"]b)", ""), CParserExceptions::ExPartNotDefined);
    BOOST_CHECK_THROW(test(processor, R"(a[PART_BEGIN]["l1"][PART]["l1"][PART_END]c[PART]["l1"]d)", ""), CParserExceptions::ExPossibleInfiniteLoop);
    BOOST_CHECK_THROW(test(processor, R"(a[MACRO_BEGIN]a[PART_BEGIN]["l1"])", ""), CParserExceptions::ExMissingMacroEnd);

    //tables
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz",";","#","unknown-property"]Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CTemplateProvidedTableLoaderExceptions::ExUnexpectedTableProperty);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz",";","#","pad-rows;unknown-property"]Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CTemplateProvidedTableLoaderExceptions::ExUnexpectedTableProperty);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz",";",";"];Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CCsvParser::ExBadCommentChars);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz","\n",";"];Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CCsvParser::ExBadDelimiter);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz"];Numbers;1;2";5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CCsvParser::ExUnexpectedQuote);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz"];Numbers;1;"2"a;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CCsvParser::ExRequireDelimitingChar);
    BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["label A"];Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CMacroProcessorExceptions::ExTableLabelAlreadyDefined);
    BOOST_CHECK_THROW(test(processor, R"(a[TABLE_BEGIN]["l"]b)", ""), CParserExceptions::ExMissingTableEnd);
    BOOST_CHECK_THROW(test(processor, R"(a[TABLE_BEGIN]["l"]b[ENTRY]["c"])", ""), CParserExceptions::ExMissingTableEnd);
    BOOST_CHECK_THROW(test(processor, R"(a[TABLE_END]b)", ""), CParserExceptions::ExMissingTableBegin);
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK_THROW(test(processor, input, "", true), CVerticalTableBuilderExceptions::ExUnderflow);
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;over;flow
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK_THROW(test(processor, input, "", true), CVerticalTableBuilderExceptions::ExOverflow);
    }
    {
        processor.setCanChangeNonTemporaryTableList(false);
        BOOST_CHECK_THROW(test(processor, R"(#[TABLE_BEGIN]["labelxyz",";","#","permanent"]Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", ""), CMacroProcessorExceptions::ExCannotChangeTableList);
        BOOST_CHECK_THROW(test(processor, R"([TABLE_REMOVE]["label A"])", ""), CMacroProcessorExceptions::ExCannotChangeTableList);
        processor.setCanChangeNonTemporaryTableList(true);
    }
    BOOST_CHECK_THROW(test(processor, R"([TABLE_LOAD]["", "label A"])", ""), CTemplatePreprocessorExceptions::ExTableLoadingNotSupported);

    //something still in parser
    BOOST_CHECK_THROW( test( processor, "a[SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);
    //something still in line collector
    BOOST_CHECK_THROW( test( processor, "[MACRO_BEGIN][ENTRY][\"Type\"][MACRO_END][SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);
    //something still in next level
    BOOST_CHECK_THROW( test( processor, "[MACRO_BEGIN.][ENTRY.][\"Type\"]\n[SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);


    class ExCannotApplyConversionToSubstitution : public std::runtime_error 
    { public: ExCannotApplyConversionToSubstitution() : std::runtime_error( "Conversion cannot be applied to this substitution.") {}};

    processor.reset();
    processor.connectTable( ptrTable, "label A", true, true, 1, 1, false);

    //no keyword
    BOOST_CHECK( test( processor, "a", "a"));
    //simple block parsing
    BOOST_CHECK( test( processor, "a[BEGIN]b[END]c", "abc"));
    //empty pipeline on close
    BOOST_CHECK( test( processor, "a[BEGIN][BEGIN.][BEGIN..][BEGIN...]b[END...][END..][END.][END]c", "abc"));
    //advanced block parsing
    BOOST_CHECK( test( processor, "a[MACRO_BEGIN]b[BEGIN]c[BEGIN]d[OR]e[END]f[OR]g[END]h[OR]i[MACRO_END]j", "abcdfhj"));

    //no keyword with no lookup substitute
    BOOST_CHECK( test( processor, "a[IF][FIRST_TIME]", "a"));
    //entry
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"]>", "<int><double><bool><bool>"));
    //any
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][ANY]>", "<><description&more><><>"));
    //last time
    BOOST_CHECK( test( processor, "<[ENTRY][\"a\"][BEGIN][IF][LAST_TIME][OR],[END]>", "<int,><valueCount,><0>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"a\"][BEGIN][IF][NOT][LAST_TIME][OR],[END]>", "<int><valueCount><0,>"));
    //last time empty
    BOOST_CHECK( test( processor, "[BEGIN][IF][LAST_TIME][OR]<[ENTRY][\"Type\"]>[END]", "<int><double><bool>"));
    //count
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][COUNT][BEGIN][IF][LAST_TIME][OR],[END]>", "<int1,><double2,><bool3,><bool4>"));
    //two entries top down        
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"],[ENTRY][\"Array Maximum\"]>", "<double,30>"));
    //entry left to right
    BOOST_CHECK( test( processor, "<[ENTRY][\"a\"]>", "<int><valueCount><0>"));
    //entry list
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"]>", "<description&more>"));
    //entry list with separator
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][MERGE][\";\"]>", "<description;&more>"));
    //if entry
    BOOST_CHECK( test( processor, "<[IF][ENTRY][\"Type\"],[ENTRY][\"Array Maximum\"]>", "<,30>"));
    //if not entry
    BOOST_CHECK( test( processor, "<[IF][NOT][ENTRY][\"Array Maximum\"][ENTRY][\"Type\"]>", "<int><bool><bool>"));
    //first time
    BOOST_CHECK( test( processor, "<[BEGIN]+[IF][FIRST_TIME]+[OR][END][ENTRY][\"a\"]>", "<++int><valueCount><0>"));
    BOOST_CHECK( test( processor, "<[BEGIN]+[IF][NOT][FIRST_TIME]+[OR][END][ENTRY][\"a\"]>", "<int><++valueCount><++0>"));
    //text correctly sorted
    BOOST_CHECK( test( processor, "�[BEGIN]<+[IF][FIRST_TIME]+[ENTRY][\"a\"]>[OR]<[ENTRY][\"a\"][BEGIN][IF][ENTRY][\"a\"]$[IF][ENTRY][\"a\"][OR][END]>[OR][[ENTRY][\"b\"]][END]�[BEGIN][END]", "�<++int>��<valueCount$>��[30]��<0$>��[description]��[&more]�"));
    BOOST_CHECK( test( processor, "<[BEGIN]+[IF][FIRST_TIME]+[END]-[ENTRY][\"a\"]>", "<++-int>"));
    BOOST_CHECK( test( processor, "[BEGIN]    [IF][ENTRY][\"Item\"][READ_TOP_DOWN][EQUALS][\"a\"][ENTRY][\"Name\"][END]\n", "    valueCount\n"));
    //matches
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][EQUALS][\"bool\"]>", "<bool><bool>"));
    //not matches
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][NOT][EQUALS][\"bool\"]>", "<int><double>"));
    //matches ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][EQUALS][\"Bool\"][IGNORE_CASE]>", "<bool><bool>"));
    //not matches ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][NOT][EQUALS][\"Bool\"][IGNORE_CASE]>", "<int><double>"));
    //matches list
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][EQUALS][\"&more\"]>", "<&more>"));
    //matches list + flush 
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][EQUALS][\"&more\"][FLUSH]>", "<description&more>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][EQUALS][\"&more\"][FLUSH][MERGE][\";\"]>", "<description;&more>"));
    //matches list + forall
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][EQUALS][\"&more\"][FOR_ALL]>", ""));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][EQUALS][\"&more\"][FOR_ALL][FLUSH]>", ""));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Description\"][NOT][EQUALS][\"\"][FOR_ALL]>", "<description&more>"));
    //check left to right
    BOOST_CHECK( test( processor, "<[ENTRY][\"Item\"][READ_LEFT_TO_RIGHT]>", "<Type><Name><Array Maximum><Default><Description><Description>"));
    //top down
    BOOST_CHECK( test( processor, "<[ENTRY][\"Item\"][READ_TOP_DOWN]>", "<a><b><c><d>"));
    //no modifier in contrast to direction defined
    BOOST_CHECK( test( processor, "<[ENTRY][\"Item\"]>", "<a><b><c><d><Type><Name><Array Maximum><Default><Description><Description>"));
    //count
    BOOST_CHECK( test( processor, "<[COUNT][ENTRY][\"Item\"]>", "<1a><2b><3c><4d><5Type><6Name><7Array Maximum><8Default><9Description><10Description>"));

    //last time not expanded because a expands and has higher priority
    BOOST_CHECK( test( processor, "<[BEGIN][ENTRY][\"a\"][OR][IF][LAST_TIME]default[END]>", "<int><valueCount><0>"));
    //index
    BOOST_CHECK( test( processor, "<[IF][ENTRY][\"a\"][INDEX]>", "<2><3><5>"));
    //regex matches
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][MATCHES_REGEX]['val.*']>", "<valueCount><values><valid>"));
    //regex matches ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][MATCHES_REGEX]['vAl.*'][IGNORE_CASE]>", "<valueCount><values><valid>"));
    //not matches regex
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][NOT][MATCHES_REGEX]['val.*']>", "<test>"));
    //not matches regex ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][NOT][MATCHES_REGEX]['vAl.*'][IGNORE_CASE]>", "<test>"));
    //replace
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][REPLACE][\"oo\",\"aaa\"]>", "<int><double><baaal><baaal>"));
    //replace ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][REPLACE][\"Oo\",\"aaa\"][IGNORE_CASE]>", "<int><double><baaal><baaal>"));
    //volatile
    BOOST_CHECK( test( processor, "<[BEGIN][ENTRY][\"not existent\"][OR][END][ENTRY][\"Type\"]>", ""));
    BOOST_CHECK( test( processor, "<[BEGIN][ENTRY][\"not existent\"][AS_VOLATILE][OR][END][ENTRY][\"Type\"]>", "<int><double><bool><bool>"));
    //regex replace
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][REGEX_REPLACE]['[aeiou]','_']>", "<_nt><d__bl_><b__l><b__l>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][REGEX_REPLACE]['(b|d)','/$1']>", "<int></dou/ble></bool></bool>"));
    //regex replace ignore case
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][REGEX_REPLACE]['[AEIOU]','_'][IGNORE_CASE]>", "<_nt><d__bl_><b__l><b__l>"));
    //to upper
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][TO_UPPER]>", "<INT><DOUBLE><BOOL><BOOL>"));
    //to lower
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][TO_LOWER]>", "<valuecount><values><valid><test>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Name\"][TO_UPPER][TO_LOWER]>", "<valuecount><values><valid><test>"));
    //starts with
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][STARTS_WITH][\"bo\"]>", "<bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][STARTS_WITH][\"BO\"][IGNORE_CASE]>", "<bool><bool>"));
    //ends with
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][ENDS_WITH][\"ol\"]>", "<bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][ENDS_WITH][\"OL\"][IGNORE_CASE]>", "<bool><bool>"));
    //contains
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][CONTAINS][\"oo\"]>", "<bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][CONTAINS][\"OO\"][IGNORE_CASE]>", "<bool><bool>"));
    //not contains x 2
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][NOT][CONTAINS][\"o\"][NOT][CONTAINS][\"u\"]>", "<int><bool><bool>"));
    //to cstring
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][TO_UPPER][REPLACE][\"I\",\"\\n\"][TO_CSTRING]>", "<\\nNT><DOUBLE><BOOL><BOOL>"));
    //if count and constraints
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][IF][COUNT][EQUALS][\"2\"]+[OR][END]>", "<int><double+><bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][COUNT][EQUALS][\"2\"]+[OR][END]>", "<int><double2+><bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][IF][NOT][COUNT][EQUALS][\"2\"]+[OR][END]>", "<int+><double><bool+><bool+>"));
    //if index and constraints
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][IF][INDEX][EQUALS][\"2\"]+[OR][END]>", "<int+><double><bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][INDEX][EQUALS][\"2\"]+[OR][END]>", "<int2+><double><bool><bool>"));
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"][BEGIN][IF][NOT][INDEX][EQUALS][\"2\"]+[OR][END]>", "<int><double+><bool+><bool+>"));
    //error tag
    BOOST_CHECK( test( processor, "<[ENTRY][\"Type\"]>[OR][ERROR][\"Error Message 1234.\"]", "<int><double><bool><bool>"));
    BOOST_CHECK_THROW( test( processor, "<[ENTRY][\"Description\"]>[OR][ERROR][\"Error Message 1234.\"]", ""), CMacroExpanderExceptions::ExErrorTagExpanded<StringT>);
    //html escape
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][HTML_ESCAPE]>", "<description&amp;more>"));
    try
    {
        test( processor, "<[ENTRY][\"Description\"]>[OR][ERROR][\"Error Message 1234.\"]", "");
    }
    catch( CMacroExpanderExceptions::ExErrorTagExpanded<StringT>& e)
    {
        BOOST_CHECK_EQUAL( e.getMessage(), "Error Message 1234.");
    }
    //trim
    BOOST_CHECK( test( processor, "  <[ENTRY][\"Type\"]>[TRIM] \n", "<int><double><bool><bool>"));
    BOOST_CHECK( test( processor, "  <[ENTRY][\"Type\"]>[TRIM.] \n", "<int><double><bool><bool>"));
    //trim left
    BOOST_CHECK(test(processor, "  <[ENTRY][\"Type\"]>[TRIM_LEFT] ", "<int> <double> <bool> <bool> "));
    BOOST_CHECK(test(processor, "  <[ENTRY][\"Type\"]>[TRIM_LEFT]\n", "<int>\n<double>\n<bool>\n<bool>\n"));
    BOOST_CHECK(test(processor, "  <[ENTRY][\"Type\"]>[TRIM_LEFT] \n", "<int> \n<double> \n<bool> \n<bool> \n"));
    BOOST_CHECK(test(processor, "  <[ENTRY][\"Type\"]>[TRIM_LEFT.] \n", "<int> \n<double> \n<bool> \n<bool> \n"));
    //trim right
    BOOST_CHECK( test( processor, "  <[ENTRY][\"Type\"]>[TRIM_RIGHT] \n", "  <int>  <double>  <bool>  <bool>"));
    BOOST_CHECK( test( processor, "  <[ENTRY][\"Type\"]>[TRIM_RIGHT.] \n", "  <int>  <double>  <bool>  <bool>"));
    //comment
    BOOST_CHECK( test( processor, "  [COMMENT]<[ENTRY][\"Type\"]>[TRIM] \n", ""));
    BOOST_CHECK( test( processor, "[COMMENT.]<[ENTRY][\"Type\"]>[TRIM.] \n", ""));
    //set recursion level limit
    BOOST_CHECK( test( processor, "[SET_RECURSION_LEVEL_LIMIT][MACRO_BEGIN]<[ENTRY][\"Type\"]>[MACRO_END]<[ENTRY.][\"Type\"]>", "<int><double><bool><bool><[ENTRY][\"Type\"]>"));
    //check limit is reseted properly
    BOOST_CHECK( test( processor, "<[ENTRY.][\"Type\"]>", "<int><double><bool><bool>")); 
    //pad left
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][PAD_LEFT][\" \",15]>", "<    description          &more>"));
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][MERGE][\"\n\"][PAD_LEFT][\" \",5,7]>", "<description\n  &more>"));
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][MERGE][\"\n\"][PAD_LEFT][\"x \",+5,7]>", "<x    description\nx &more>"));
    //pad right
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][PAD_RIGHT][\" \",15]>", "<description    &more          >"));
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][MERGE][\"\n\"][PAD_RIGHT][\" \",5,7]>", "<description\n&more  >"));
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][MERGE][\"\n\"][PAD_RIGHT][\" \",5,+7]>", "<description\n&more       >"));

    //check fix for bug #4 [TRIM] breaks multi line macro 
    BOOST_CHECK(test(processor, "\na[MACRO_BEGIN]<[TRIM]\n[ENTRY][\"Type\"][TRIM]\n[REPLACE][\"b\", \"B\"]>\nb[MACRO_END]\nc", "\na<int>\nb<douBle>\nb<Bool>\nb<Bool>\nb\nc", true));

    //block format
    BOOST_CHECK(test(processor, "<[ENTRY][\"Description\"][MERGE][\" \"][BLOCK_FORMAT][5]>", "<descr\niptio\nn\n&more>"));

    //parts
    //standard fragment
    BOOST_CHECK(test(processor, R"(#[PART_BEGIN]["labelxyz"][NOT][INDEX][EQUALS]["2"][PART_END]<[ENTRY]["Type"][BEGIN][IF][PART]["labelxyz"]+[OR][END]>)", "#<int><double+><bool+><bool+>"));
    //part begin forces macro evaluation inside a line just like MACRO_BEGIN
    BOOST_CHECK(test(processor, R"(<[ENTRY]["Type"]>#[PART_BEGIN]["labelxyz"][NOT][INDEX][EQUALS]["2"][PART_END]<[ENTRY]["Type"][BEGIN][IF][PART]["labelxyz"]+[OR][END]>)", "<int>#<double>#<bool>#<bool>#<int><double+><bool+><bool+>"));
    //empty part
    BOOST_CHECK(test(processor, R"(#[PART_BEGIN]["labelxyz"][PART_END]<[ENTRY]["Type"][BEGIN][IF][NOT][INDEX][EQUALS]["2"][PART]["labelxyz"]+[OR][END]>)", "#<int><double+><bool+><bool+>"));
    //part remove does never complain if no part is found
    BOOST_CHECK(test(processor, R"(#[PART_REMOVE]["labelxyz"]>)", "#>"));
    //remove part while expanding, not exactly good style but we handle the case
    BOOST_CHECK(test(processor, R"(#[PART_BEGIN]["labelxyz"][PART_REMOVE]["labelxyz"][NOT][INDEX][EQUALS]["2"][PART_END]<[ENTRY]["Type"][BEGIN][IF][PART]["labelxyz"]+[OR][END]>)", "#<int><double+><bool+><bool+>"));
    //on and a half macro in part, bonus: remove twice
    BOOST_CHECK(test(processor, R"(#[PART_BEGIN]["labelxyz"]a[MACRO_BEGIN]<[ENTRY]["Type"]>[MACRO_END]b[MACRO_BEGIN]{[ENTRY]["Type"]}[PART_END]-[PART]["labelxyz"][MACRO_END]c[PART_REMOVE]["labelxyz"][PART_REMOVE]["labelxyz"]d)", "#-a<int><double><bool><bool>b{int}{double}{bool}{bool}cd"));

    //tables
    BOOST_CHECK(test(processor, R"(#[TABLE_BEGIN]["labelxyz"]Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", "#<1><2><5>"));
    BOOST_CHECK(test(processor, R"(#[TABLE_BEGIN]["labelxyz","#"]Numbers#1#2#5[TABLE_END]<[ENTRY]["Numbers"]>)", "#<1><2><5>"));
    //table begin forces macro evaluation inside a line just like MACRO_BEGIN
    BOOST_CHECK(test(processor, R"(<[ENTRY]["Type"]>#[TABLE_BEGIN]["labelxyz"]Numbers;1;2;5[TABLE_END]<[ENTRY]["Numbers"]>)", "<int>#<double>#<bool>#<bool>#<1><2><5>"));
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz","#","+-"][TRIM]
+This is a comment
Numbers#1#2#5
-This is a comment
[TABLE_END][TRIM]
<[ENTRY]["Numbers"]>)";
    BOOST_CHECK(test(processor, input, "#<1><2><5>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", ""]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK(test(processor, input, "#<row1><row2><row3><col1><col2><col3>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz",";","","top-down"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK(test(processor, input, "#<row1><row2><row3>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", "", "left-to-right"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK(test(processor, input, "#<col1><col2><col3>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", "", "left-to-right;top-down"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK(test(processor, input, "#<row1><row2><row3><col1><col2><col3>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", "", "pad-rows"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["row2"]>)";
    BOOST_CHECK(test(processor, input, "#<2,1>", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", "", "csv-ignore-quotes"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;"2,1";""";"
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["row2"]>)";
    BOOST_CHECK(test(processor, input, R"(#<"2,1"><"""><">)", true));
    }
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", ";", "", "permanent"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["m"]>)";
    BOOST_CHECK(test(processor, input, "#<row1><row2><row3><col1><col2><col3>", true));
    BOOST_CHECK(test(processor, R"(<[ENTRY]["m"]>)", "<row1><row2><row3><col1><col2><col3>"));
    BOOST_CHECK(test(processor, R"(#[TABLE_REMOVE]["labelxyz"]<[ENTRY]["m"]>)", ""));
    //removing twice or non existent table is silent
    BOOST_CHECK(test(processor, R"(#[TABLE_REMOVE]["labelxyz"]<[ENTRY]["m"]>)", ""));
    }
    //can disconnect all tables
    BOOST_CHECK(test(processor, R"(#[TABLE_REMOVE]["label A"]<[ENTRY]["Type"]>)", ""));
    processor.connectTable(ptrTable, "label A", true, true, 1, 1, false);
    { const char* input =
        R"(#[TABLE_BEGIN]["labelxyz", "", "", "csv-ignore-quotes"]FullLinesAnyContent
row1;1,1;1,2;1,3
row2;2,1;2,2;2,3
row3;3,1;3,2;3,3
[TABLE_END]<[ENTRY]["FullLinesAnyContent"]>)";
    BOOST_CHECK(test(processor, input, "#<row1;1,1;1,2;1,3><row2;2,1;2,2;2,3><row3;3,1;3,2;3,3>", true));
    }


    //connect more tables for testing unloading
    SharedTableT ptrAnotherTableA = std::make_shared<TableT>();
    SharedTableT ptrAnotherTableB = std::make_shared<TableT>();
    processor.connectTable(ptrAnotherTableA, "label C", true, true, 0, 0, false);
    processor.connectTable(ptrAnotherTableB, "label D", true, true, 0, 0, false);
    //connect table again
    processor.connectTable( ptrTable, "label B", true, true, 2, 4, false);
    SharedConstTableT ptrDisconnectedTable;
    BOOST_CHECK( processor.disconnectTable( "label A", false, ptrDisconnectedTable));
    BOOST_CHECK( ptrDisconnectedTable == ptrTable);
    //check as expected with changed headers
    BOOST_CHECK( test( processor, "<[ENTRY][\"bool\"]>", "<bool><validtest><falsefalse>"));
    //check return true table can be freed
    ptrDisconnectedTable.reset();
    BOOST_CHECK( processor.disconnectTable( "label B", false, ptrDisconnectedTable));
    BOOST_CHECK( ptrDisconnectedTable == ptrTable);
    //disconnect unloading test tables
    ptrDisconnectedTable.reset();
    BOOST_CHECK( processor.disconnectTable( "label C", false, ptrDisconnectedTable));
    BOOST_CHECK( ptrDisconnectedTable == ptrAnotherTableA);
    ptrDisconnectedTable.reset();
    BOOST_CHECK( processor.disconnectTable( "label D", false, ptrDisconnectedTable));
    BOOST_CHECK( ptrDisconnectedTable == ptrAnotherTableB);

    BOOST_CHECK_NO_THROW( processor.connectTable(ptrTable, "label B", true, true, 7, 5, false));
    BOOST_CHECK_THROW( processor.connectTable(ptrTable, "label C", true, true, 2, 6, false), CMacroProcessorExceptions::ExColumnHeaderIndexOutOfBounds);
    BOOST_CHECK_THROW( processor.connectTable(ptrTable, "label D", true, true, 8, 4, false), CMacroProcessorExceptions::ExRowHeaderIndexOutOfBounds);
}



template <typename StringT>
void testTemplateProcessor()
{
    typedef std::vector<std::vector<StringT> > TableT;
    typedef TStreamHelper<StringT> StreamT;
    typedef TestTemplateLoader<StringT> TemplateLoaderT;
    typedef CTemplateProcessor<TableT, StreamT, TemplateLoaderT> ProcessorT;
    typedef std::shared_ptr<TableT> SharedTableT;
    SharedTableT ptrTable = std::make_shared<TableT>();

    ptrTable->resize( columns);
    for ( unsigned int col = 0; col < columns; ++col)
    {
        (*ptrTable)[col].resize( rows);
        for ( unsigned int row = 0; row < rows; ++row)
        {
            (*ptrTable)[col][row] = boost::lexical_cast<StringT>(cItemTable[row][col]);
        }
    }

    StreamT output;
    StreamT expectedOutput;
    ProcessorT processor;
    TemplateLoaderT loader;

    TStreamConversionWrapper<StringT, ProcessorT> wrappedProcessor( processor);

    processor.connectTemplateLoader( &loader);
    processor.connectTable( ptrTable, boost::lexical_cast<StringT>("label"), true, true, 1, 1, false);
    processor.connectOutputStream( &output);

    processor.open();

    wrappedProcessor
        << "start\n"
        << "  [SET_MARKUP][\"???\",\"!!!\"][TRIM]\t\n" //the markup does not change for a line
        << "???COMMENT!!! This is a comment.\n"
        << "???BEGIN!!!???IF!!!???FIRST_TIME!!!->???OR!!!???END!!!<???ENTRY!!![\"Type\"]>\n"
        << "???BEGIN!!!???IF!!!???FIRST_TIME!!!->???OR!!!???END!!!<???ENTRY.!!![\"Type\"]>\n"
        << "   text???TRIM!!!  \n"
        << "   ???INCLUDE!!![\"templatefile.name \"]  " // include does not add chars
        << "\nend"
        ;
    
    processor.close();

    expectedOutput
        << "start\n"
        << "-><int>\n"
        << "<double>\n"
        << "<bool>\n"
        << "<bool>\n"
        << "-><int>\n"
        << "-><double>\n"
        << "-><bool>\n"
        << "-><bool>\n"
        << "text     \n"
        << "end"
        ;

    BOOST_CHECK( output.result == expectedOutput.result);
    BOOST_CHECK( loader.m_count == 1);
    BOOST_CHECK( loader.m_filename == boost::lexical_cast<StringT>("templatefile.name ") );
}

BOOST_AUTO_TEST_CASE( TTemplateProcessor)
{
    testTemplateProcessor<std::string>();
#ifdef _MSC_VER
    testTemplateProcessor<std::wstring>();
#endif 

    testMacroProcessing<std::string>();
}
