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

#ifndef INCLUDED_CGENERATOR_H_3620111
#define INCLUDED_CGENERATOR_H_3620111

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <vector>
#include <list>
#include <fstream>
#include "CTemplateProcessor.h"
#include "CTemplateLoader.h"
#include "CCsvParser.h"
#include "CVerticalTableBuilder.h"
#include "CFilesBinaryEqual.h"
#include "CPositionTracker.h"
#include "CSourceFile.h"
#include "CTargetFile.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <io.h>
#include <stdio.h>
#include "FileSystem.h"

#include "ParameterParser.h"

class CsvFileT;
class GeneratedFileT;
class IntermediateFileT;

///defines exceptions thrown by CGenerator for template argument independent access
class CGeneratorExceptions
{
public:
    class ExFailedToDeleteTempFile : public std::runtime_error 
    { public: ExFailedToDeleteTempFile() : std::runtime_error( "Failed to delete temporary output file.") {}};

    class ExFailedToDeleteOldTargetFile : public std::runtime_error 
    { public: ExFailedToDeleteOldTargetFile() : std::runtime_error( "Failed to delete old version of target file.") {}};

    class ExCannotMoveIntermediateFile : public std::runtime_error 
    { public: ExCannotMoveIntermediateFile() : std::runtime_error( "Failed to move intermediate file.") {}};

    class ExFailedToUnloadTable : public std::runtime_error 
    { public: ExFailedToUnloadTable() : std::runtime_error( "Cannot find table to unload.") {}};

    class ExIntermediateFileRequired : public std::runtime_error  //not in error printer table
    { public: ExIntermediateFileRequired() : std::runtime_error( "Use of an intermediate file is required for in place inline processing.") {}};
};


///sets up and operates all building blocks needed for generating
template <typename StringT, typename LogOutputStreamT = CNul >
class CGenerator : public boost::noncopyable, public CGeneratorExceptions
{
    class TemplateLoader;
    typedef typename StringT::value_type CharT;
    typedef CTargetFile< StringT, GeneratedFileT> TargetFileT;
    typedef CTargetFile< StringT, IntermediateFileT> IntermediateTargetFileT;
    typedef std::vector<std::vector<StringT> > TableT;
    typedef typename TableT::size_type SizeT;
    typedef CTemplateProcessor<TableT, typename TargetFileT::OutputStreamT, TemplateLoader, LogOutputStreamT> TemplateProcessorT;
    typedef CTemplateLoader<TemplateProcessorT, StringT, LogOutputStreamT> TemplateLoaderT;

    typedef typename TargetFileT::OutputStreamT OutputStreamT;
    typedef typename TemplateLoaderT::InputStreamT InputStreamT;

    ///holds the properties and the data of currently loaded tables
    class TableData
    {
    public:
        ///the properties a table can have
        struct TableProperties
        {
            TableProperties()
                : csvDelimiter(0)
                , neverEquals(true)
            {
            }

            TableProperties( const StringT& aFileName, CharT aCsvDelimiter, const StringT& aCsvCommentChars, bool setNeverEquals)
                : csvDelimiter( aCsvDelimiter)    
                , filename( aFileName)
                , csvComment( aCsvCommentChars)
                , neverEquals( setNeverEquals)
            {
            }

            bool operator == ( const TableProperties& rhs) const
            {
                bool equal = csvDelimiter == rhs.csvDelimiter 
                    && filename == rhs.filename
                    && csvComment == rhs.csvComment
                    && !neverEquals
                    && !rhs.neverEquals
                    ;

                return equal;
            }

            CharT csvDelimiter; ///<used when loading
            StringT filename; ///<used when loading
            StringT csvComment; ///<used when loading
            bool neverEquals;  ///<used when loading
        };

        TableData(){}
        TableData( 
            const TableProperties& theProperties, 
            TableT* aTable
            ) 
            : properties(theProperties)
            , table(aTable)
        {
        }

        TableData( boost::shared_ptr<const TableT> aTable)
            : table(aTable)
        {
        }

        ///used for finding the table in a container
        bool operator == ( const TableProperties& rhs) const
        {
            return properties == rhs;
        }

        ///used for finding the table in a container
        bool operator == ( const TableT* rhs) const
        {
            return table.get() == rhs;
        }

        ///access the table data read only
        boost::shared_ptr<const TableT> getTable() const
        {
            return table;
        }

        ///access the properties read only
        const TableProperties& getProperties() const
        {
            return properties;
        }

    private:
        TableProperties properties; ///<properties used when loading
        boost::shared_ptr<const TableT> table; ///<the loaded table data
    };


    typedef std::list<TableData > TableListT;
    class TemplateLoader: public TemplateLoaderT {};

public:
    typedef typename TemplateLoaderT::FileDataListT FileDataListT;

    CGenerator()
        : m_lastRowNumberWithFailure(1)
        , m_indexOfLastProcessedParameter(0)
        , m_csvDelimiter( STRING_LITERAL(';'))
        , m_csvIgnoreDoubleQuotes(false)
        , m_logOutputStream(0)
    {
    }

    ///set delimiter for next csv table to load
    void setCsvDelimiter( CharT delimiter)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Setting CSV Delimiter:\n";
            *m_logOutputStream << "CSV Delimiter=" << delimiter << "\n";
        }

        //get a stream object used to widen the used characters
        CSourceFile<StringT, CsvFileT> file( "", true);
        //check the delimiter
        CCsvParser::checkDelimiter( delimiter, m_csvIgnoreDoubleQuotes, file.get());
        m_csvDelimiter = delimiter;
    }

    ///get delimiter for next csv table to load
    CharT getCsvDelimiter()
    {
        return m_csvDelimiter;
    }

    ///set list of characters as string that mark commented lines for next csv table to load
    void setCsvCommentChars( const StringT& commentChars)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Setting CSV Comment Chars:\n";
            *m_logOutputStream << "CSV Comment Chars=" << commentChars << "\n";
        }

        //get a stream object used to widen the used characters
        CSourceFile<StringT, CsvFileT> file( "", true);
        //check the characters
        CCsvParser::checkCharsUsedForCommenting( commentChars, m_csvDelimiter, m_csvIgnoreDoubleQuotes, file.get());
        //set the characters
        m_csvCommentChars = commentChars;
    }

    ///get list of characters as string that mark commented lines for next csv table to load
    const StringT& getCsvCommentChars() const
    {
        return m_csvCommentChars;
    }

    ///set csv parsing option
    void setCsvIgnoreDoubleQuotes( bool ignoreDoubleQuotes)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Setting CSV Ignore Double Quotes=:\n";
            *m_logOutputStream << "CSV Ignore Double Quotes=" << ignoreDoubleQuotes << "\n";
        }
        m_csvIgnoreDoubleQuotes = ignoreDoubleQuotes;
    }

    ///load another table for generation, see also unloadTable
    void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Loading table:\n";
            *m_logOutputStream << "Table file=" << tableFileName << "\n";
            *m_logOutputStream << "Label=" << label << "\n";
            *m_logOutputStream << "Read top down=" << topDown << "\n";
            *m_logOutputStream << "Read left to right=" << leftRight << "\n";
            *m_logOutputStream << "Row header index=" << rowHeaderIndex << "\n";
            *m_logOutputStream << "Column header index=" << columnHeaderIndex << "\n";
            *m_logOutputStream << "Pad rows=" << padRows << "\n";
        }

        bool useCinAsInput = tableFileName == STRING_LITERAL("-");
        //assemble the properties of the table
        typename TableData::TableProperties properties( tableFileName, getCsvDelimiter(), getCsvCommentChars(), useCinAsInput);

        //try to find the table among the already loaded tables
        typename TableListT::iterator pos = std::find( m_tableList.begin(), m_tableList.end(), properties);
        if ( pos != m_tableList.end())
        {
            //use the already loaded table
            m_templateProcessor.connectTable( pos->getTable().get(), label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
        }
        else //table not found, need to load the table
        {
            m_lastRowNumberWithFailure = 0;

            //create table and table builder
            typedef CVerticalTableBuilder<TableT> TableBuilderT;
            TableT* tableToLoad = new TableT;
            TableBuilderT tableBuidler( *tableToLoad, padRows);
            typename TableListT::value_type tableData( properties, tableToLoad);

            try
            {
                //open table file
                CSourceFile<StringT, CsvFileT> file( tableFileName, tableFileName == STRING_LITERAL("-"));
                //parse the table file
                CCsvParser::parse( file.get(), tableBuidler, m_csvDelimiter, getCsvCommentChars(), m_csvIgnoreDoubleQuotes, m_positionTracker);
                //connect table to processor and keep reference in list
                m_templateProcessor.connectTable( tableData.getTable().get(), label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
                m_tableList.push_back( tableData);
            }
            catch(...)
            {
                m_lastRowNumberWithFailure = tableData.getTable()->size() ? (*tableData.getTable())[0].size() : 1;
                throw;
            }
        }
    }

    ///load another table for generation, see also unloadTable
    void loadTable( InputStreamT& inputStream, const StringT& label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Loading table from stream:\n";
            *m_logOutputStream << "Label=" << label << "\n";
            *m_logOutputStream << "Read top down=" << topDown << "\n";
            *m_logOutputStream << "Read left to right=" << leftRight << "\n";
            *m_logOutputStream << "Row header index=" << rowHeaderIndex << "\n";
            *m_logOutputStream << "Column header index=" << columnHeaderIndex << "\n";
            *m_logOutputStream << "Pad rows=" << padRows << "\n";
        }

        m_lastRowNumberWithFailure = 0;

        //create table and table builder
        typedef CVerticalTableBuilder<TableT> TableBuilderT;
        TableT* tableToLoad = new TableT;
        TableBuilderT tableBuidler( *tableToLoad, padRows);
        boost::shared_ptr<const TableT> psTableToLoad( tableToLoad);
        typename TableListT::value_type tableData( psTableToLoad);

        try
        {
            //parse the table file
            CCsvParser::parse( inputStream, tableBuidler, m_csvDelimiter, getCsvCommentChars(), m_csvIgnoreDoubleQuotes, m_positionTracker);
            //connect table to processor and keep reference in list
            m_templateProcessor.connectTable( tableData.getTable().get(), label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
            m_tableList.push_back( tableData);
        }
        catch(...)
        {
            m_lastRowNumberWithFailure = tableData.getTable()->size() ? (*tableData.getTable())[0].size() : 1;
            throw;
        }
    }

    ///load another table for generation, see also unloadTable
    void loadTable( boost::shared_ptr<const TableT> table, const StringT& label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Loading provided table:\n";
            *m_logOutputStream << "Label=" << label << "\n";
            *m_logOutputStream << "Read top down=" << topDown << "\n";
            *m_logOutputStream << "Read left to right=" << leftRight << "\n";
            *m_logOutputStream << "Row header index=" << rowHeaderIndex << "\n";
            *m_logOutputStream << "Column header index=" << columnHeaderIndex << "\n";
        }

        typename TableListT::value_type tableData( table);
        m_templateProcessor.connectTable( tableData.getTable().get(), label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
        m_tableList.push_back( tableData);
    }
        
    ///generates output by processing a template file, no parameters, no intermediate file
    void generate( const StringT& templateFileName, const StringT& targetFileName, bool append = false)
    {
        generate( templateFileName, targetFileName, append, std::vector<StringT>());
    }

    ///generates output by processing a template file, no intermediate file
    template <typename ParameterListT>
    void generate( const StringT& templateFileName, const StringT& targetFileName, bool append, const ParameterListT& parameters)
    {
        generate( templateFileName, targetFileName, false, false, StringT(), append, parameters);
    }

    ///generates output by processing a template file
    template <typename ParameterListT>
    void generate( 
        const StringT& templateFileName, 
        const StringT& targetFileName, 
        bool useIntermediateFile, 
        bool recycle,
        const StringT& intermediateFileName, 
        bool append, 
        const ParameterListT& parameters,
        const CInlineTemplateParameters<StringT>& inlineTemplateParameters = CInlineTemplateParameters<StringT>()
        )
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Processing file:\n";
            *m_logOutputStream << "Template file=" << templateFileName << "\n";
            *m_logOutputStream << "Target file=" << targetFileName << "\n";
            *m_logOutputStream << "Use intermediate file=" << useIntermediateFile << "\n";
            *m_logOutputStream << "Recycle=" << recycle << "\n";
            *m_logOutputStream << "Intermediate file=" << intermediateFileName << "\n";
            *m_logOutputStream << "Append=" << append << "\n";

            if ( inlineTemplateParameters.enabled)
            {
                *m_logOutputStream << "Inline prefix=" << inlineTemplateParameters.inlinePrefix << "\n";
                *m_logOutputStream << "Inline postfix=" << inlineTemplateParameters.inlinePostfix << "\n";
                *m_logOutputStream << "Inline generated postfix=" << inlineTemplateParameters.inlineGeneratedPostfix << "\n";
                *m_logOutputStream << "Inline pad=" << inlineTemplateParameters.inlinePad << "\n";
            }
        }

        // An intermediate file is automatically used when processing files with inline templates
        // if no output file is provided.
        if (   templateFileName == targetFileName
            && targetFileName != STRING_LITERAL("-") //std streams not used
            && inlineTemplateParameters.enabled 
            && !useIntermediateFile
            )
        {
            throw ExIntermediateFileRequired();
        }

        //create parameter table
        TableT parameterTable;
        ParameterParser::parse( parameters, parameterTable, m_indexOfLastProcessedParameter, STRING_LITERAL('='));
        //connect parameter table if not empty
        if ( !parameterTable.empty())
        {
            m_templateProcessor.connectTable( &parameterTable, STRING_LITERAL("Internal Parameter Table"), true, false, 1, 1);
        }

        //create output file
        bool useCoutInstead = targetFileName == STRING_LITERAL("-");
        useIntermediateFile = useCoutInstead ? false : useIntermediateFile;

        TargetFileT generatedFile;
        IntermediateTargetFileT intermediateFile;

        //open requested target file
        if ( useIntermediateFile )
        {
            intermediateFile.open( intermediateFileName);
        }
        else
        {
            generatedFile.open( targetFileName, useCoutInstead, append);
        }

        //reset the loader
        m_templateLoader.resetInclusionHierarchy();
        //open the template processor
        m_templateProcessor.open();
        //connect the objects
        m_templateLoader.connectOutputStream( &m_templateProcessor);
        m_templateProcessor.connectOutputStream( useIntermediateFile ? &intermediateFile.get() : &generatedFile.get());
        m_templateProcessor.connectTemplateLoader( &m_templateLoader);
        m_templateProcessor.setInlineTemplateParameters( inlineTemplateParameters);
        //start processing the template file
        m_templateLoader.loadTemplateFile( templateFileName, templateFileName == STRING_LITERAL("-"));
        //close everything
        m_templateProcessor.close();
        if ( useIntermediateFile )
        {
            intermediateFile.checkGood();
            intermediateFile.close();
        }
        else
        {
            generatedFile.checkGood();
            generatedFile.close();
        }
        //clean up
        m_templateProcessor.connectOutputStream(0);

        //disconnect parameter table if not empty
        if ( !parameterTable.empty())
        {
            const TableT* disconnectedTable = 0;
            if ( !m_templateProcessor.disconnectTable( STRING_LITERAL("Internal Parameter Table"), disconnectedTable))
            {
                throw std::runtime_error("Cannot disconnect temporary parameter table");
            }
            assert( disconnectedTable == &parameterTable);
        }

        if ( useIntermediateFile)
        {
            //if temporary output is the same as the target file, clean up
            if ( FilesBinaryEqual( intermediateFileName, targetFileName))
            {
                if ( !removeFile( intermediateFileName))
                {
                    throw ExFailedToDeleteTempFile();
                }
            }
            else //replace target file
            {
                //recycle target file if possible
                if ( recycle && FileSystem::isRegularFile( targetFileName) && FileSystem::recycleFile( targetFileName))
                {
                    // moved file to system recycle bin
                }
                else if ( !removeFile( targetFileName))
                {
                    throw ExFailedToDeleteOldTargetFile();
                }
                if ( !moveFile( intermediateFileName, targetFileName))
                {
                    throw ExCannotMoveIntermediateFile();
                }
            }
        }
    }

    void generate( InputStreamT& inputStream, OutputStreamT& outputStream)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Processing stream.\n";
        }
        
        //reset the loader
        m_templateLoader.resetInclusionHierarchy();
        //open the template processor
        m_templateProcessor.open();
        //connect the objects
        m_templateLoader.connectOutputStream( &m_templateProcessor);
        m_templateProcessor.connectOutputStream( &outputStream);
        m_templateProcessor.connectTemplateLoader( &m_templateLoader);
        //start processing the template file
        m_templateLoader.loadTemplateStream( inputStream);
        //close everything
        m_templateProcessor.close();
        //clean up
        m_templateProcessor.connectOutputStream(0);
    }

    //resets the generator building blocks
    void reset()
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Resetting:\n";
        }
        m_templateProcessor.reset();

        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Clearing table list.\n";
        }
        m_tableList.clear();

        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Clearing set include paths.\n";
        }
        m_templateLoader.reset();
        m_lastRowNumberWithFailure = 1;
        m_positionTracker.reset();
        m_indexOfLastProcessedParameter = 0;
        setCsvDelimiter( STRING_LITERAL(';'));
        setCsvCommentChars( STRING_LITERAL(""));
        setCsvIgnoreDoubleQuotes( false);
    }

    ///unloads a table, see also loadTable
    void unloadTable( const StringT& label)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Unloading table:\n";
            *m_logOutputStream << "Label=" << label << "\n";
        }

        const TableT* disconnectedTable = 0;
        if ( m_templateProcessor.disconnectTable( label, disconnectedTable) )
        {
            typename TableListT::iterator pos = std::find( m_tableList.begin(), m_tableList.end(), disconnectedTable);
            if ( pos != m_tableList.end())
            {
                //if fully disconnected discard the table
                m_tableList.erase( pos);
            }
            else
            {
                throw ExFailedToUnloadTable();
            }
        }
    }

    ///sets new tag markup
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Setting markup:\n";
            *m_logOutputStream << "Prefix=" << prefix << "\n";
            *m_logOutputStream << "Postfix=" << postfix << "\n";
        }

        m_templateProcessor.setMarkup( prefix, postfix);
    }

    ///adds an include directory to the list
    void addIncludeDirectory( const StringT& directory)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Adding include directory:\n";
            *m_logOutputStream << "Include directory=" << directory << "\n";
        }

        m_templateLoader.addIncludeDirectory( directory);
    }

    ///last row of table with failure for error output
    SizeT getLastRowNumberWithFailure() { return m_lastRowNumberWithFailure; }

    ///index of last processed parameter for error output
    SizeT getIndexOfLastProcessedParameter() { return m_indexOfLastProcessedParameter; }

    ///last column of table or template file with failure for error output
    unsigned int getLastColumnWithFailure() { return m_positionTracker.getColumn();}

    ///last line of table or template file with failure for error output
    unsigned int getLastLineWithFailure() { return m_positionTracker.getLine();}

    ///returns stack of files currently opened
    const FileDataListT& getInclusionHierarchy()
    {
        return m_templateLoader.getInclusionHierarchy();
    }

    ///return maximum number of recursion levels
    static int getMaxNumberOfRecursionLevels()
    {
        return TemplateProcessorT::getMaxNumberOfRecursionLevels();
    }

    ///return maximum text size of macro
    static size_t getMaxMacroTextSizeBytes()
    {
        return TemplateProcessorT::getMaxMacroTextSizeBytes();
    }

    ///connect log output stream
    void connectLogOutputStream( LogOutputStreamT* stream)
    {
        //log
        if ( m_logOutputStream && !stream)
        {
            *m_logOutputStream << "Disabling log.\n";
        }
        if ( !m_logOutputStream && stream)
        {
            *stream << "Enabling log.\n";
        }
        if ( m_logOutputStream && stream && m_logOutputStream != stream)
        {
            *m_logOutputStream << "Switching log stream.\n";
            *stream  << "Switching log stream.\n";
        }

        m_logOutputStream = stream;
        m_templateProcessor.connectLogOutputStream( stream);
        m_templateLoader.connectLogOutputStream( stream);
    }

private:

    ///deletes file specified by name and returns true if the file has been deleted
    bool removeFile( const StringT& name)
    {
        if ( FileSystem::isRegularFile( name) ) //if exists
        {
            return FileSystem::removeFile( name);
        }
        return true;
    }

    ///moves a file
    bool moveFile( const StringT& from, const StringT& to)
    {
        try
        {
            FileSystem::moveFile( from, to);
            return true;
        }
        catch( std::exception& e)
        {
            (void) e;
        }
        return false;
    }

private:
    TableListT m_tableList; ///<table data held in smart pointer, table meta data
    TemplateProcessorT m_templateProcessor; ///<does the work
    TemplateLoader m_templateLoader; ///<the loader
    CPositionTracker m_positionTracker; ///<used by csv parser
    SizeT m_lastRowNumberWithFailure; ///<for error output
    SizeT m_indexOfLastProcessedParameter; ///<for error output
    CharT m_csvDelimiter; ///<delimiter used by csv files to load
    StringT m_csvCommentChars; ///<list of characters as string that mark commented lines in CSV-files
    bool m_csvIgnoreDoubleQuotes; ///< Option for csv parser, double quotes are treated as normal character
    LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
};

#endif /* INCLUDED_CGENERATOR_H_3620111 */
