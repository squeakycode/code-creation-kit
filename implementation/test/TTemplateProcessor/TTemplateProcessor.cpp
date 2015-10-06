//   Copyright (C) 2011-2015 Andreas Gau
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
#include "CTemplateProcessor.h"
#include <sstream>
#include <iostream>

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

const unsigned int rows = 5;
const unsigned int columns = 7;

const char* itemTable[rows][columns] =
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
    TestTemplateLoader(): m_count(0) {}

    ///dummy with no functionality
    void loadTemplateFile( const StringT& filename)
    {
        m_count++;
        m_filename = filename;
    }
    int m_count;
    StringT m_filename;
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

    TableT table;

    table.resize( columns);
    for ( unsigned int col = 0; col < columns; ++col)
    {
        table[col].resize( rows);
        for ( unsigned int row = 0; row < rows; ++row)
        {
            table[col][row] = boost::lexical_cast<StringT>(itemTable[row][col]);
        }
    }

    ProcessorT processor;

    //check too large macro check is working
    if ( processor.getMaxMacroTextSizeBytes() < 20 * 1024 * 1024)
    {
        StringT largeString( processor.getMaxMacroTextSizeBytes() + 1, 'a');
        BOOST_CHECK_THROW( test( processor, largeString, ""), CParserExceptions::ExMacroTooLarge);
    }

    processor.connectTable( &table, "label A", true, true, 1, 1);

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

    //something still in parser
    BOOST_CHECK_THROW( test( processor, "a[SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);
    //something still in line collector
    BOOST_CHECK_THROW( test( processor, "[MACRO_BEGIN][ENTRY][\"Type\"][MACRO_END][SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);
    //something still in next level
    BOOST_CHECK_THROW( test( processor, "[MACRO_BEGIN.][ENTRY.][\"Type\"]\n[SET_RECURSION_LEVEL_LIMIT]b", ""), CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit);


    class ExCannotApplyConversionToSubstitution : public std::runtime_error 
    { public: ExCannotApplyConversionToSubstitution() : std::runtime_error( "Conversion cannot be applied to this substitution.") {}};

    processor.reset();
    processor.connectTable( &table, "label A", true, true, 1, 1);

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
    BOOST_CHECK( test( processor, "§[BEGIN]<+[IF][FIRST_TIME]+[ENTRY][\"a\"]>[OR]<[ENTRY][\"a\"][BEGIN][IF][ENTRY][\"a\"]$[IF][ENTRY][\"a\"][OR][END]>[OR][[ENTRY][\"b\"]][END]§[BEGIN][END]", "§<++int>§§<valueCount$>§§[30]§§<0$>§§[description]§§[&more]§"));
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

    //connect more tables for testing unloading
    TableT anotherTableA;
    TableT anotherTableB;
    processor.connectTable( &anotherTableA, "label C", true, true, 0, 0);
    processor.connectTable( &anotherTableB, "label C", true, true, 0, 0);
    //connect table again
    processor.connectTable( &table, "label B", true, true, 2, 4);
    //check return false table still in use, with above connection
    const TableT* disconnectedTable = 0;
    BOOST_CHECK( !processor.disconnectTable( "label A", disconnectedTable));
    BOOST_CHECK( disconnectedTable == &table);
    //check as expected with changed headers
    BOOST_CHECK( test( processor, "<[ENTRY][\"bool\"]>", "<bool><validtest><falsefalse>"));
    //check return true table can be freed
    disconnectedTable = 0;
    BOOST_CHECK( processor.disconnectTable( "label B", disconnectedTable));
    BOOST_CHECK( disconnectedTable == &table);
    //disconnect unloading test tables
    disconnectedTable = 0;
    BOOST_CHECK( processor.disconnectTable( "label C", disconnectedTable));
    BOOST_CHECK( disconnectedTable == &anotherTableB);
    disconnectedTable = 0;
    BOOST_CHECK( processor.disconnectTable( "label C", disconnectedTable));
    BOOST_CHECK( disconnectedTable == &anotherTableA);

    BOOST_CHECK_NO_THROW( processor.connectTable( &table, "label B", true, true, 7, 5));
    BOOST_CHECK_THROW( processor.connectTable( &table, "label B", true, true, 2, 6), CMacroProcessorExceptions::ExColumnHeaderIndexOutOfBounds);
    BOOST_CHECK_THROW( processor.connectTable( &table, "label B", true, true, 8, 4), CMacroProcessorExceptions::ExRowHeaderIndexOutOfBounds);
}



template <typename StringT>
void testTemplateProcessor()
{
    typedef std::vector<std::vector<StringT> > TableT;
    typedef TStreamHelper<StringT> StreamT;
    typedef TestTemplateLoader<StringT> TemplateLoaderT;
    typedef CTemplateProcessor<TableT, StreamT, TemplateLoaderT> ProcessorT;

    TableT table;

    table.resize( columns);
    for ( unsigned int col = 0; col < columns; ++col)
    {
        table[col].resize( rows);
        for ( unsigned int row = 0; row < rows; ++row)
        {
            table[col][row] = boost::lexical_cast<StringT>(itemTable[row][col]);
        }
    }

    StreamT output;
    StreamT expectedOutput;
    ProcessorT processor;
    TemplateLoaderT loader;

    TStreamConversionWrapper<StringT, ProcessorT> wrappedProcessor( processor);

    processor.connectTemplateLoader( &loader);
    processor.connectTable( &table, boost::lexical_cast<StringT>( "label"), true, true, 1, 1);
    processor.connectOutputStream( &output);

    processor.open();

    wrappedProcessor << "text";

    //wrappedProcessor
    //    << "start\n"
    //    << "  [MARKUP_PREFIX][\"???\"]\t\n"
    //    << "???MARKUP_POSTFIX][\"!!!\"]\n"
    //    << "???COMMENT!!! This is a comment.\n"
    //    << "???BEGIN!!!???FIRST_TIME!!!->???OR!!!???END!!!<???ENTRY!!![\"Type\"]>\n"
    //    << "???BEGIN!!!???FIRST_TIME!!!->???OR!!!???END!!!<???ENTRY.!!![\"Type\"]>\n"
    //    << "   text???TRIM!!!  \n"
    //    << "   ???INCLUDE!!![\"templatefile.name \"]  "
    //    << "\nend"
    //    ;
    //
    //processor.close();

    //expectedOutput
    //    << "start\n"
    //    << "-><int>\n"
    //    << "<double>\n"
    //    << "<bool>\n"
    //    << "<bool>\n"
    //    << "-><int>\n"
    //    << "-><double>\n"
    //    << "-><bool>\n"
    //    << "-><bool>\n"
    //    << "text\n"
    //    << "end"
    //    ;

    //BOOST_CHECK( output.result == expectedOutput.result);
    //BOOST_CHECK( loader.m_count == 1);
    //BOOST_CHECK( loader.m_filename == boost::lexical_cast<StringT>("templatefile.name ") );
}

BOOST_AUTO_TEST_CASE( TTemplateProcessor)
{
    testTemplateProcessor<std::string>();
    testTemplateProcessor<std::wstring>();

    testMacroProcessing<std::string>();
}
