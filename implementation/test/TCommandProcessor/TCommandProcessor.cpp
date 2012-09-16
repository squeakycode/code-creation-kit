//   Copyright (C) 2011-2012 Andreas Gau
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
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <list>
#include "CommandProcessor.h"
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 4702 )
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#endif
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
#include <boost/foreach.hpp>

///externally provided exception class showing that an error has been printed
class CErrorPrinted{};

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
        , m_setCsvDelimiter(false)
        , m_setCsvCommentChars(false)
        , m_delimiter(0)
        , m_rowHeaderIndex(0)
        , m_columnHeaderIndex(0)
        , m_padRows(false)
        , m_append(false)
    {
        setMarkupPrefix( "[");
        setMarkupPostfix( "]");
    }

    void generate( 
        const StringT& templateFile,
        const StringT& targetFile,
        bool useIntermediateFile,
        const StringT& intermediateFile,
        bool append, const ParameterListT& parameters,
        const CInlineTemplateParameters<StringT>& inlineTemplateParameters
        )
    {
        BOOST_CHECK( m_generate);
        BOOST_CHECK( m_templateFile == templateFile);
        BOOST_CHECK( m_targetFile == targetFile);
        BOOST_CHECK( m_useIntermediateFile == useIntermediateFile);
        BOOST_CHECK( m_intermediateFileName == intermediateFile);
        BOOST_CHECK( m_parameters == parameters);
        BOOST_CHECK( m_append == append);
        BOOST_CHECK( m_inlineTemplateParameters == inlineTemplateParameters);
    }

    void reset()
    {
        BOOST_CHECK( m_reset);
    }

    void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
    {
        BOOST_CHECK( m_loadTable);
        BOOST_CHECK( m_tableFileName == tableFileName);
        BOOST_CHECK( m_label == label);
        BOOST_CHECK( m_topDown == topDown);
        BOOST_CHECK( m_leftToRight == leftToRight);
        BOOST_CHECK( m_rowHeaderIndex == rowHeaderIndex);
        BOOST_CHECK( m_columnHeaderIndex == columnHeaderIndex);
        BOOST_CHECK( m_padRows == padRows);
    }

    void unloadTable( const StringT& label)
    {
        BOOST_CHECK( m_unloadTable);
        BOOST_CHECK( m_label == label);
    }

    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        BOOST_CHECK( m_generate);
        BOOST_CHECK( m_markupPrefix == prefix);
        BOOST_CHECK( m_markupPostfix == postfix);
    }

    void addIncludeDirectory( const StringT& directory)
    {
        BOOST_CHECK( m_addIncludeDirectory);
        BOOST_CHECK( m_includeDirectory == directory);
    }

    void setCsvDelimiter( CharT delimiter)
    {
        BOOST_CHECK( m_setCsvDelimiter);
        BOOST_CHECK( m_delimiter == delimiter);
    }

    void setCsvCommentChars( const StringT& commentChars)
    {
        BOOST_CHECK( m_setCsvCommentChars);
        BOOST_CHECK( m_csvCommentChars == commentChars);
    }

    bool m_reset;
    bool m_loadTable;
    bool m_unloadTable;
    bool m_generate;

    bool m_topDown;
    bool m_leftToRight;
    bool m_useIntermediateFile;
    bool m_addIncludeDirectory;
    bool m_setCsvDelimiter;
    bool m_setCsvCommentChars;

    unsigned int m_rowHeaderIndex;
    unsigned int m_columnHeaderIndex;
    bool m_padRows;

    CInlineTemplateParameters<StringT> m_inlineTemplateParameters;

    bool m_append;

    void setTemplateFile( const char* text)
    {
        m_templateFile = boost::lexical_cast<StringT>(text);
        m_templateFile = FileSystem::determineDependentLocation( m_templateFile);
    }

    void setTargetFile( const char* text)
    {
        m_targetFile = boost::lexical_cast<StringT>(text);
        m_targetFile = FileSystem::determineDependentLocation( m_targetFile);
    }

    void setTableFileName( const char* text)
    {
        m_tableFileName = boost::lexical_cast<StringT>(text);
        m_tableFileName = FileSystem::determineDependentLocation( m_tableFileName);
    }

    void setIntermediateFileName( const char* text)
    {
        m_intermediateFileName = boost::lexical_cast<StringT>(text);
        m_intermediateFileName = FileSystem::determineDependentLocation( m_intermediateFileName);
    }

    void setParameter( const char* text)
    {
        m_parameters.push_back( boost::lexical_cast<StringT>(text));
    }

    void setMarkupPrefix( const char* text)
    {
        m_markupPrefix = boost::lexical_cast<StringT>(text);
    }

    void setMarkupPostfix( const char* text)
    {
        m_markupPostfix = boost::lexical_cast<StringT>(text);
    }

    void setCsvCommentCharsExpected( const char* text)
    {
        m_csvCommentChars = boost::lexical_cast<StringT>(text);
    }

    void setLabel( const char* text)
    {
        m_label = boost::lexical_cast<StringT>(text);
    }

    void setIncludeDirectory( const char* text)
    {
        m_includeDirectory = boost::lexical_cast<StringT>(text);
        m_includeDirectory = FileSystem::determineDependentLocation( m_includeDirectory);
    }

    void setDelimiter( const char delimiter)
    {
        m_delimiter = boost::lexical_cast<CharT>(delimiter);
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
    CharT m_delimiter;
    StringT m_templateFile;
    StringT m_targetFile;
    StringT m_tableFileName;
    StringT m_intermediateFileName;
    StringT m_markupPrefix;
    StringT m_markupPostfix;
    StringT m_includeDirectory;
    StringT m_csvCommentChars;
    StringT m_label;
    ParameterListT m_parameters;
    FileSetT m_dummy;
};

///converts a list of string to command line arg format
template <typename StringT, typename ContainerT, typename GeneratorT> 
void process( ContainerT& container, GeneratorT& generator)
{
    std::list<StringT> argsString;
    std::vector<typename StringT::value_type*> args;

    argsString.push_back( boost::lexical_cast<StringT>( "exename"));
    args.push_back( const_cast<typename StringT::value_type*> (argsString.back().c_str()));

    BOOST_FOREACH( const typename ContainerT::value_type& arg, container)
    {
        argsString.push_back( boost::lexical_cast<StringT>( arg.c_str()));
        args.push_back( const_cast<typename StringT::value_type*> (argsString.back().c_str()));
    }

    CommandProcessor::processCommandLine( args.size(), &args[0], generator, generator);
}

///runs the test for given string type
template <typename StringT>
void runTest()
{
    using namespace boost::assign;
    typedef TTestGenerator<StringT> GeneratorT;
    typedef typename StringT::value_type CharT;

    {
        //test help
        GeneratorT generator;
        std::vector<std::string> args;
        args += "-h";
        process<StringT>( args, generator);
    }

    {
        //test reset
        GeneratorT generator;
        generator.m_reset = true;
        std::vector<std::string> args;
        args += "-c", "-r";
        process<StringT>( args, generator);
    }

    {
        //test empty line
        GeneratorT generator;
        generator.m_reset = true;
        std::vector<std::string> args;
        args += "-c", "";
        process<StringT>( args, generator);
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
        std::vector<std::string> args;
        args += "-c", "table.csv";
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
        std::vector<std::string> args;
        args += "-c", "-n 6 -t \"tab le.csv\" --pad-rows";
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
        std::vector<std::string> args;
        args += "-c", "-w 0 -l -a LabelA table.csv";
        process<StringT>( args, generator);
    }

    {
        //test set delimiter
        GeneratorT generator;
        generator.m_setCsvDelimiter = true;
        generator.setDelimiter( '\t');
        std::vector<std::string> args;
        args += "-c", "--csv-delimiter tab";
        process<StringT>( args, generator);
    }

    {
        //test set comment chars
        GeneratorT generator;
        generator.m_setCsvCommentChars = true;
        generator.setCsvCommentCharsExpected( "#'");
        std::vector<std::string> args;
        args += "-c", "--csv-comment-chars #'";
        process<StringT>( args, generator);
    }

    {
        //test unload table
        GeneratorT generator;
        generator.setLabel( "LabelA");
        generator.m_unloadTable = true;
        std::vector<std::string> args;
        args += "-c", "-x LabelA LabelA";
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
        std::vector<std::string> args;
        args += "-c", "-s a.txt -o b.txt";
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
        std::vector<std::string> args;
        args += "-c", "-s a.txt -o b.txt --append-to-file";
        process<StringT>( args, generator);
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
        generator.m_inlineTemplateParameters = CInlineTemplateParameters<StringT>( true, STRING_LITERAL("'''"), STRING_LITERAL(">>>"), STRING_LITERAL("<<<"), 56);
        std::vector<std::string> args;
        args += "-c", "-s a.txt --inlined -b ''' -c >>> -d <<< --inline-pad 56";
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
        std::vector<std::string> args;
        args += "-c", "-s a.txt -o b.txt -p a=b name=d";
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
        std::vector<std::string> args;
        args += "-c", "-s a.txt -o b.txt --markup-prefix \"<<\" --markup-postfix \">>\"";
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
        std::vector<std::string> args;
        args += "-c", "-s a.txt -o b.txt -m $";
        process<StringT>( args, generator);
    }

    {
        //test add include directory
        GeneratorT generator;
        generator.setIncludeDirectory( "my/templates");
        std::vector<std::string> args;
        generator.m_addIncludeDirectory = true;
        args += "-c", "-i my/templates";
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
        std::vector<std::string> args;
        args += "-f", "CommandFile.tccmd";
        process<StringT>( args, generator);
    }

    {
        //test file not found handling
        GeneratorT generator;
        std::vector<std::string> args;
        generator.m_reset = true;
        args += "NotThere.tccmd";
        BOOST_CHECK_THROW( process<StringT>( args, generator), CErrorPrinted);
    }
}

BOOST_AUTO_TEST_CASE( TCommandProcessor)
{
    runTest<std::string>();
    runTest<std::wstring>();
}