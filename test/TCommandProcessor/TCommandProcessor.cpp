// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <vector>
#include <list>

#include "TCommandProcessorTestFiles.h"

///externally provided exception class showing that an error has been printed
class CErrorPrinted{};

#include "CommandProcessor.h"
#include "TargetFile.h"
#include "StringConvert.h"
using namespace code_creation_kit;

class LogFileT;

///stub for the generator
template <typename StringT>
class TTestGenerator
{
public:
    typedef typename StringT::value_type CharT;
    typedef std::vector< StringT> ParameterListT;
    typedef std::set<StringT> FileSetT;

    TTestGenerator() 
        : m_reset(false)
        , m_loadTable(false)
        , m_unloadTable(false)
        , m_generate(false)

        , m_topDown(false)
        , m_leftToRight(false)
        , m_useIntermediateFile(false)
        , m_addIncludeDirectory(false)
        , m_setCsvDelimiterChars(false)
        , m_setCsvCommentChars(false)
        , m_setCsvQuoteChars(false)
        , m_setLogStream(false)


        , m_rowHeaderIndex(0)
        , m_columnHeaderIndex(0)
        , m_padRows(false)

        , m_append(false)
        , m_logStream(false)
        , m_recycle(false)
        , m_canChangeTableList(false)
    {
        setMarkupPrefix( "[");
        setMarkupPostfix( "]");
    }

    void generate( 
        const StringT& templateFile,
        const StringT& targetFile,
        bool useIntermediateFile,
        bool recycle,
        const StringT& intermediateFile,
        bool append,
        const ParameterListT& parameters,
        bool canChangeTableList,
        const InlineTemplateParameters<StringT>& inlineTemplateParameters
        )
    {
        CHECK( m_generate);
        CHECK( m_templateFile == templateFile);
        CHECK( m_targetFile == targetFile);
        CHECK( m_useIntermediateFile == useIntermediateFile);
        CHECK( m_recycle == recycle);
        CHECK( m_intermediateFileName == intermediateFile);
        CHECK( m_parameters == parameters);
        CHECK( m_append == append);
        CHECK( (m_inlineTemplateParameters == inlineTemplateParameters));
        CHECK(m_canChangeTableList == canChangeTableList);
    }

    void reset()
    {
        CHECK( m_reset);
    }

    void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
    {
        CHECK( m_loadTable);
        CHECK( m_tableFileName == tableFileName);
        CHECK( m_label == label);
        CHECK( m_topDown == topDown);
        CHECK( m_leftToRight == leftToRight);
        CHECK( m_rowHeaderIndex == rowHeaderIndex);
        CHECK( m_columnHeaderIndex == columnHeaderIndex);
        CHECK( m_padRows == padRows);
    }

    void unloadTable( const StringT& label)
    {
        CHECK( m_unloadTable);
        CHECK( m_label == label);
    }

    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        CHECK( m_generate);
        CHECK( m_markupPrefix == prefix);
        CHECK( m_markupPostfix == postfix);
    }

    void addIncludeDirectory( const StringT& directory)
    {
        CHECK( m_addIncludeDirectory);
        CHECK( m_includeDirectory == directory);
    }

    void setCsvDelimiterChars( StringT delimiter)
    {
        CHECK( m_setCsvDelimiterChars);
        CHECK( m_csvDelimiterChars == delimiter);
    }

    void setCsvCommentChars( const StringT& commentChars)
    {
        CHECK( m_setCsvCommentChars);
        CHECK( m_csvCommentChars == commentChars);
    }

    void setCsvQuoteChars( StringT quoteChars) 
    {
        CHECK( m_setCsvQuoteChars);
        CHECK( m_csvQuoteChars == quoteChars);
    }

    void connectLogOutputStream( const void* stream)
    {
        CHECK( m_setLogStream);
        m_logStream = (stream != NULL);
    }

    bool m_reset;
    bool m_loadTable;
    bool m_unloadTable;
    bool m_generate;

    bool m_topDown;
    bool m_leftToRight;
    bool m_useIntermediateFile;
    bool m_addIncludeDirectory;
    bool m_setCsvDelimiterChars;
    bool m_setCsvCommentChars;
    bool m_setCsvQuoteChars;
    bool m_setLogStream;

    unsigned int m_rowHeaderIndex;
    unsigned int m_columnHeaderIndex;
    bool m_padRows;

    InlineTemplateParameters<StringT> m_inlineTemplateParameters;

    bool m_append;
    bool m_logStream;
    bool m_recycle;
    bool m_canChangeTableList;

    void setTemplateFile( const char* text)
    {
        m_templateFile = StringConvert<StringT>(text);
        m_templateFile = FileSystem::determineDependentLocation( m_templateFile);
    }

    void setTargetFile( const char* text)
    {
        m_targetFile = StringConvert<StringT>(text);
        m_targetFile = FileSystem::determineDependentLocation( m_targetFile);
    }

    void setTableFileName( const char* text)
    {
        m_tableFileName = StringConvert<StringT>(text);
        m_tableFileName = FileSystem::determineDependentLocation( m_tableFileName);
    }

    void setIntermediateFileName( const char* text)
    {
        m_intermediateFileName = StringConvert<StringT>(text);
        m_intermediateFileName = FileSystem::determineDependentLocation( m_intermediateFileName);
    }

    void setParameter( const char* text)
    {
        m_parameters.push_back( StringConvert<StringT>(text));
    }

    void setMarkupPrefix( const char* text)
    {
        m_markupPrefix = StringConvert<StringT>(text);
    }

    void setMarkupPostfix( const char* text)
    {
        m_markupPostfix = StringConvert<StringT>(text);
    }

    void setCsvCommentCharsExpected( const char* text)
    {
        m_csvCommentChars = StringConvert<StringT>(text);
    }

    void setLabel( const char* text)
    {
        m_label = StringConvert<StringT>(text);
    }

    void setIncludeDirectory( const char* text)
    {
        m_includeDirectory = StringConvert<StringT>(text);
        m_includeDirectory = FileSystem::determineDependentLocation( m_includeDirectory);
    }

    void setSetCsvDelimiterCharsExpected(const char* text)
    {
        m_csvDelimiterChars = StringConvert<StringT>(text);
    }

    void setCsvQuoteCharsExpected(const char* text)
    {
        m_csvQuoteChars = StringConvert<StringT>(text);
    }

    const FileSetT& getTableFiles() const
    {
        return m_dummy;
    }

    const FileSetT& getGeneratedFiles() const
    {
        return m_dummy;
    }

    const FileSetT& getTemplateFiles() const
    {
        return m_dummy;
    }

private:
    StringT m_csvDelimiterChars;
    StringT m_templateFile;
    StringT m_targetFile;
    StringT m_tableFileName;
    StringT m_intermediateFileName;
    StringT m_markupPrefix;
    StringT m_markupPostfix;
    StringT m_includeDirectory;
    StringT m_csvCommentChars;
    StringT m_csvQuoteChars;
    StringT m_label;
    ParameterListT m_parameters;
    FileSetT m_dummy;
};

///converts a list of string to command line arg format
template <typename StringT, typename ContainerT, typename GeneratorT> 
void process( ContainerT& container, GeneratorT& generator)
{
    TargetFile<StringT, LogFileT> logFile;

    std::list<StringT> argsString;
    std::vector<typename StringT::value_type*> args;

    argsString.push_back( StringConvert<StringT>( "exename"));
    args.push_back( const_cast<typename StringT::value_type*> (argsString.back().c_str()));

    for( const typename ContainerT::value_type& arg : container)
    {
        argsString.push_back( StringConvert<StringT>( arg.c_str()));
        args.push_back( const_cast<typename StringT::value_type*> (argsString.back().c_str()));
    }

    CommandProcessor::processCommandLine( static_cast<int>(args.size()), &args[0], generator, generator, logFile);
}

///runs the test for given string type
template <typename StringT>
void runTest()
{
    typedef TTestGenerator<StringT> GeneratorT;
    typedef typename StringT::value_type CharT;

    {
        //test help
        GeneratorT generator;
        std::vector<std::string> args = { "-h" };
        process<StringT>( args, generator);
    }

    {
        //test reset
        GeneratorT generator;
        generator.m_reset = true;
        std::vector<std::string> args = { "-c", "-r" };
        process<StringT>( args, generator);
    }

    {
        //test empty line
        GeneratorT generator;
        generator.m_reset = true;
        std::vector<std::string> args = { "-c", "" };
        process<StringT>( args, generator);
    }

    {
        //test logging off
        GeneratorT generator;
        generator.m_setLogStream = true;
        generator.m_logStream = true;
        std::vector<std::string> args = { "-c", "--log-file none" };
        process<StringT>( args, generator);
        CHECK( !generator.m_logStream);
    }

    {
        //test logging on; stdout
        GeneratorT generator;
        generator.m_setLogStream = true;
        generator.m_logStream = false;
        std::vector<std::string> args = { "-c", "--log-file -" };
        process<StringT>( args, generator);
        CHECK( generator.m_logStream);
    }

    {
        //test logging on; file
        GeneratorT generator;
        generator.m_setLogStream = true;
        generator.m_logStream = false;
        std::vector<std::string> args = { "-c", "--log-file testlogfile.log" };
        process<StringT>( args, generator);
        CHECK( generator.m_logStream);
    }

    {
        //test load table
        GeneratorT generator;
        generator.setTableFileName( "table.csv");
        generator.setLabel( "table.csv");
        generator.m_topDown = true;
        generator.m_leftToRight = true;
        generator.m_columnHeaderIndex = 1;
        generator.m_rowHeaderIndex = 1;
        generator.m_loadTable = true;
        std::vector<std::string> args = { "-c", "table.csv" };
        process<StringT>( args, generator);
    }

    {
        //test load table space in name, top down, column, pad rows
        GeneratorT generator;
        generator.setTableFileName( "tab le.csv");
        generator.setLabel( "tab le.csv");
        generator.m_topDown = true;
        generator.m_loadTable = true;
        generator.m_padRows = true;
        generator.m_columnHeaderIndex = 6;
        generator.m_rowHeaderIndex = 1;
        std::vector<std::string> args = { "-c", "-n 6 -t \"tab le.csv\" --pad-rows" };
        process<StringT>( args, generator);
    }
    {
        //test load table change direction, row, left to right
        GeneratorT generator;
        generator.setTableFileName( "table.csv");
        generator.setLabel( "LabelA");
        generator.m_leftToRight = true;
        generator.m_loadTable = true;
        generator.m_columnHeaderIndex = 1;
        generator.m_rowHeaderIndex = 0;
        std::vector<std::string> args = { "-c", "-w 0 -l -a LabelA table.csv" };
        process<StringT>( args, generator);
    }

    {
        //test set delimiter
        GeneratorT generator;
        generator.m_setCsvDelimiterChars = true;
        generator.setSetCsvDelimiterCharsExpected( "\t");
        std::vector<std::string> args = 
//#ifdef _MSC_VER
        { "-c", "--csv-delimiter-chars \\t" };
//#else
//        { "-c", "--csv-delimiter-chars \"\t\"" };
//#endif
        process<StringT>( args, generator);
    }

    {
        //test set comment chars
        GeneratorT generator;
        generator.m_setCsvCommentChars = true;
        generator.setCsvCommentCharsExpected( "#+");
        std::vector<std::string> args = { "-c", "--csv-comment-chars #+" };
        process<StringT>( args, generator);
    }

    {
        //test set quote chars
        GeneratorT generator;
        generator.m_setCsvQuoteChars = true;
        generator.setCsvQuoteCharsExpected( "#-");
        std::vector<std::string> args = { "-c", "--csv-quote-chars #-" };
        process<StringT>( args, generator);
    }

    {
        //test unload table
        GeneratorT generator;
        generator.setLabel( "LabelA");
        generator.m_unloadTable = true;
        std::vector<std::string> args = { "-c", "-x LabelA LabelA" };
        process<StringT>( args, generator);
    }

    {
        //test generate
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.m_useIntermediateFile = false;
        generator.setIntermediateFileName( "b.txt.intermediate");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt" };
        process<StringT>( args, generator);
    }

    {
        //test generate with append
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.m_useIntermediateFile = false;
        generator.m_append = true;
        generator.setIntermediateFileName( "b.txt.intermediate");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt --append-to-file" };
        process<StringT>( args, generator);
    }

    {
        //test generate with can-change-table-list
        GeneratorT generator;
        generator.setTemplateFile("a.txt");
        generator.setTargetFile("b.txt");
        generator.m_useIntermediateFile = false;
        generator.m_canChangeTableList = true;
        generator.setIntermediateFileName("b.txt.intermediate");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt --can-change-table-list" };
        process<StringT>(args, generator);
    }
    
    {
        //test generate inline
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "a.txt");
        generator.m_useIntermediateFile = true;
        generator.m_append = false;
        generator.setIntermediateFileName( "a.txt.intermediate");
        generator.m_generate = true;
        generator.m_recycle = true;
        generator.m_inlineTemplateParameters = InlineTemplateParameters<StringT>( true, STRING_LITERAL("+++"), STRING_LITERAL(">>>"), STRING_LITERAL("<<<"), 56);
        std::vector<std::string> args = { "-c", "-s a.txt --inlined -b +++ -c >>> -d <<< --inline-pad 56 -y" };
        process<StringT>( args, generator);
    }

    {
        //test generate with parameters
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.m_useIntermediateFile = false;
        generator.setIntermediateFileName( "b.txt.intermediate");
        generator.setParameter( "a=b");
        generator.setParameter( "name=d");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt -p a=b name=d" };
        process<StringT>( args, generator);
    }

    {
        //test generate with markup single
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.setMarkupPrefix( "<<");
        generator.setMarkupPostfix( ">>");
        generator.m_useIntermediateFile = false;
        generator.setIntermediateFileName( "b.txt.intermediate");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt --markup-prefix \"<<\" --markup-postfix \">>\"" };
        process<StringT>( args, generator);
    }

    {
        //test generate with markup both
        GeneratorT generator;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.setMarkupPrefix( "$");
        generator.setMarkupPostfix( "$");
        generator.m_useIntermediateFile = false;
        generator.setIntermediateFileName( "b.txt.intermediate");
        generator.m_generate = true;
        std::vector<std::string> args = { "-c", "-s a.txt -o b.txt -m $" };
        process<StringT>( args, generator);
    }

    {
        //test add include directory
        GeneratorT generator;
        generator.setIncludeDirectory( "my/templates");
        std::vector<std::string> args = { "-c", "-i my/templates" };
        generator.m_addIncludeDirectory = true;
        process<StringT>( args, generator);
    }

    {
        //test command file
        GeneratorT generator;
        generator.m_reset = true;
        generator.setTemplateFile( "a.txt");
        generator.setTargetFile( "b.txt");
        generator.m_useIntermediateFile = true;
        generator.setIntermediateFileName( "b.txt.temp");
        generator.m_generate = true;
        generator.setTableFileName( "table.csv");
        generator.setLabel( "table.csv");
        generator.m_columnHeaderIndex = 1;
        generator.m_rowHeaderIndex = 1;
        generator.m_leftToRight = true;
        generator.m_loadTable = true;
        generator.setParameter( "a=\"b\"");
        std::vector<std::string> args = { "-f", CCK_TEST_INPUT_FILE_PREFIX "CommandFile.tccmd" };
        process<StringT>( args, generator);
    }

    {
        //test instant template file
        GeneratorT generator;
        generator.m_reset = true;
        generator.setTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "myfile.h.itpl");
        generator.setTargetFile(CCK_TEST_INPUT_FILE_PREFIX "myfile.h");
        generator.setIntermediateFileName(CCK_TEST_INPUT_FILE_PREFIX "myfile.h.intermediate");
        generator.m_useIntermediateFile = false;
        generator.m_generate = true;
        std::vector<std::string> args = { "-t", CCK_TEST_INPUT_FILE_PREFIX "myfile.h.itpl" };
        process<StringT>(args, generator);
    }
    
    {
        //test instant template file no extension
        GeneratorT generator;
        generator.m_reset = true;
        generator.setTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "myfile");
        generator.setTargetFile(CCK_TEST_INPUT_FILE_PREFIX "myfile.gen");
        generator.setIntermediateFileName(CCK_TEST_INPUT_FILE_PREFIX "myfile.gen.intermediate");
        generator.m_useIntermediateFile = false;
        generator.m_generate = true;
        std::vector<std::string> args = { "-t", CCK_TEST_INPUT_FILE_PREFIX "myfile" };
        process<StringT>(args, generator);
    }

    {
        //test file not found handling
        GeneratorT generator;
        std::vector<std::string> args = { "NotThere.h.itpl" };
        generator.m_reset = true;
        CHECK_THROWS_AS(process<StringT>(args, generator), CErrorPrinted);
    }
    
    {
        //test file not found handling
        GeneratorT generator;
        std::vector<std::string> args = { "NotThere.tccmd" };
        generator.m_reset = true;
        CHECK_THROWS_AS( process<StringT>( args, generator), CErrorPrinted);
    }
}

TEST_CASE( "TCommandProcessor", "[TCommandProcessor]")
{
    CHECK_NOTHROW(CreateTCommandProcessorFiles());

    runTest<std::string>();
#ifdef _MSC_VER
    runTest<std::wstring>();
#endif
}

template <typename StringT>
std::vector<StringT> splitCommandLine(const StringT& input)
{
    typedef typename StringT::value_type CharT;
    std::vector<StringT> arguments;
    StringT currentArgument;
    bool inQuotes = false;
    bool seenQuotes = false;

    for (auto it = input.cbegin() ; it != input.cend(); ++it)
    {
        const CharT c = *it;

        if (c == '\\')
        {
            // escaping is only relevant inside of quotes
            if (inQuotes && (it + 1) != input.cend())
            {
                const CharT nextC = *(it + 1);
                if (nextC == '"' || nextC == '\\')
                {
                    currentArgument += nextC;
                    ++it;
                }
                else
                {
                    currentArgument += c;
                }
            }
            else
            {
                currentArgument += c;
            }
        }
        else if (c == '"')
        {
            inQuotes = !inQuotes;
            seenQuotes = true;
        }
        else if ((c == ' ' || c == '\t') && !inQuotes)
        {
            if (!currentArgument.empty() || seenQuotes)
            {
                arguments.push_back(currentArgument);
                currentArgument.clear();
                seenQuotes = false;
            }
        }
        else
        {
            currentArgument += c;
        }
    }
    if (!currentArgument.empty())
    {
        arguments.push_back(currentArgument);
    }
    return arguments;
}

TEST_CASE( "TSplitCommandLine", "[TCommandProcessor]")
{
    auto result = splitCommandLine<std::string>(R"(  arg1\path\path\path " arg2 " "" "\\\"" \\      last   )");
    REQUIRE(result.size() == 6);
    CHECK(result[0] == "arg1\\path\\path\\path");
    CHECK(result[1] == " arg2 ");
    CHECK(result[2] == "");
    CHECK(result[3] == "\\\"");
    CHECK(result[4] == "\\\\");
    CHECK(result[5] == "last");
    auto result2 = splitCommandLine<std::wstring>(L"a\tb");
    REQUIRE(result2.size() == 2);
    CHECK(result2[0] == L"a");
    CHECK(result2[1] == L"b");
}