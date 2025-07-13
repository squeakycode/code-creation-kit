// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <set>
#include "CSimpleFormat.h"
#include "FileSystem.h"
#include "StringLiteral.h"

#if defined CODE_CREATION_KIT_JUST_TESTING
//prevent test output to be listed as error
#   define CODE_CREATION_KIT_ERROR_TAG1 "just_testing"
#   define CODE_CREATION_KIT_ERROR_TAG2 "Just_Testing"
#else
#   define CODE_CREATION_KIT_ERROR_TAG1 "error"
#   define CODE_CREATION_KIT_ERROR_TAG2 "Error"
#endif
[PART_BEGIN]["print error message"][TRIM]
FormatT formatter(STRING_LITERAL("[BEGIN][ENTRY]["Prefix"] : " CODE_CREATION_KIT_ERROR_TAG1 " [OR] " CODE_CREATION_KIT_ERROR_TAG2 " [END]TC[ENTRY]["Error Number"]: [ENTRY]["Description"]\n"));
formatter % [ENTRY]["Source"][MERGE][" % "];
toErrorStream( formatter.str());[PART_END][TRIM]
[PART_BEGIN]["error handler catch"][TRIM]
catch( [ENTRY]["Scope"]::[ENTRY]["Exception Name"]& e)
{
    (void) e; //unused
[BEGIN][IF][ENTRY]["Condition"][TRIM]
    [MACRO_BEGIN.][IF.][ENTRY.]["Scope"][EQUALS.]["[ENTRY]["Scope"]"][IF.][ENTRY.]["Exception Name"][EQUALS.]["[ENTRY]["Exception Name"]"][IF.][ENTRY.]["Condition Group"][EQUALS.]["[ENTRY]["Condition Group"]"][TRIM.]
    [BEGIN.]if ([ENTRY.]["Condition"])[OR.]else[END.]
    {
        [PART.]["print error message", "[PART_PADDING]        "]
    }
    [MACRO_END.][TRIM.]
[OR][TRIM]
    [PART]["print error message", " ", 4]
[END][TRIM]
    throw CErrorPrinted();
}[PART_END]
namespace code_creation_kit
{
    class CErrorPrinted{};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

    template <typename StringT, typename GeneratorT>
    class CErrorPrinter
    {
    public:
        typedef CSimpleFormat<StringT> FormatT;
        typedef typename StringT::value_type CharT; 

        CErrorPrinter( GeneratorT& generator) : m_generator( generator) {}

        template <typename ParameterListT>
        void generate(
            const StringT& templateFileName,
            const StringT& targetFileName,
            bool useIntermediateFile,
            bool recycle,
            const StringT& intermediateFileName,
            bool append,
            const ParameterListT& parameters,
            bool canChangeTableList = false,
            const CInlineTemplateParameters<StringT>& inlineTemplateParameters = CInlineTemplateParameters<StringT>()
        )
        {
            try
            {
                m_generator.generate( 
                    templateFileName,
                    targetFileName,
                    useIntermediateFile,
                    recycle,
                    intermediateFileName,
                    append,
                    parameters,
                    canChangeTableList,
                    inlineTemplateParameters);
            }
            [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["generate"][TRIM]
            [PART]["error handler catch"," ",12]
            [MACRO_END][TRIM]
        }

        void reset()
        {
            m_generator.reset();
        }

        ///set delimiter for next csv table to load
        void setCsvDelimiterChars(const StringT& csvDelimiterChars)
        {
            try
            {
                m_generator.setCsvDelimiterChars(csvDelimiterChars);
            }
            [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["setCsvDelimiter"][TRIM]
            [PART]["error handler catch"," ",12]
            [MACRO_END][TRIM]
        }

        ///set list of characters as string that mark commented lines for next csv table to load
        void setCsvCommentChars( const StringT& csvCommentChars)
        {
            try
            {
                m_generator.setCsvCommentChars( csvCommentChars);
            }
            [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["setCsvCommentChars"][TRIM]
            [PART]["error handler catch"," ",12]
            [MACRO_END][TRIM]
        }

        void setCsvQuoteChars(const StringT& csvQuoteChars)
        {
            try
            {
                m_generator.setCsvQuoteChars(csvQuoteChars);
            }
            [MACRO_BEGIN] [IF][ENTRY]["Operation"][EQUALS]["setCsvQuoteChars"][TRIM]
                [PART]["error handler catch", " ", 12]
                [MACRO_END][TRIM]
        }

        void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
        {
            try
            {
                m_generator.loadTable( tableFileName, label, topDown, leftToRight, rowHeaderIndex, columnHeaderIndex, padRows);
            }
            [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["loadTable"][TRIM]
            [PART]["error handler catch"," ",12]
            [MACRO_END][TRIM]
        }

        void unloadTable( const StringT& label)
        {
            try
            {
                m_generator.unloadTable( label);
            }
            [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["unloadTable"][TRIM]
            [PART]["error handler catch"," ",12]
            [MACRO_END][TRIM]
        }

        ///sets new tag markup
        void setMarkup( const StringT& prefix, const StringT& postfix)
        {
            m_generator.setMarkup( prefix, postfix);
        }

        ///adds an include directory to the list
        void addIncludeDirectory( const StringT& directory)
        {
            m_generator.addIncludeDirectory( directory);
        }

        typedef std::set<StringT> FileSetT;

        ///get list of loaded tables, statistic only
        const FileSetT& getTableFiles() const
        {
            return m_generator.getTableFiles();
        }

        ///get list of generated files, statistic only
        const FileSetT& getGeneratedFiles() const
        {
            return m_generator.getGeneratedFiles();
        }

        ///get list of template files, statistic only
        const FileSetT& getTemplateFiles() const
        {
            return m_generator.getTemplateFiles();
        }

        ///connect log output stream
        template <typename LogOutputStreamT>
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_generator.connectLogOutputStream( stream);
        }
    private:
        void toErrorStream( std::string text)
        {
            std::cerr << text;
        }

        void toErrorStream( std::wstring text)
        {
            std::wcerr << text;
        }

        unsigned int getCurrentLineNumber()
        {
            const typename GeneratorT::FileDataT& fileData = m_generator.getLastTemplateFileProcessed();
            if (fileData.line == 0) //uninitialized?
            {
                return 1;
            }
            return fileData.line;
        }

        StringT getCurrentFileName()
        {
            const typename GeneratorT::FileDataT& fileData = m_generator.getLastTemplateFileProcessed();
            if ( fileData.name.empty()) //uninitialized?
            {
                return STRING_LITERAL("???");
            }

            return fileData.name;
        }

        StringT getTableLoadFileNameWithFailure()
        {
            return m_generator.getTableLoadFileNameWithFailure();
        }

        StringT addPath( const StringT& location)
        {
            return FileSystem::determineDependentLocation( location);
        }

        GeneratorT& m_generator;
    };

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#undef CODE_CREATION_KIT_ERROR_TAG1
#undef CODE_CREATION_KIT_ERROR_TAG2
}
