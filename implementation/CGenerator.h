// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

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
#include <memory>
#include "FileSystem.h"

#include "ParameterParser.h"

namespace code_creation_kit
{
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

        class ExIntermediateFileRequired : public std::runtime_error  //not in error printer table
        { public: ExIntermediateFileRequired() : std::runtime_error( "Use of an intermediate file is required for in place inline processing.") {}};
    };


    ///sets up and operates all building blocks needed for generating
    template <typename StringT, typename LogOutputStreamT = CNul >
    class CGenerator : public CGeneratorExceptions
    {
        class TemplateLoader;
        typedef typename StringT::value_type CharT;
        typedef CTargetFile< StringT, GeneratedFileT> TargetFileT;
        typedef CTargetFile< StringT, IntermediateFileT> IntermediateTargetFileT;
        typedef std::vector<std::vector<StringT> > TableT;
        typedef std::shared_ptr<TableT> SharedTableT;
        typedef std::shared_ptr<const TableT> SharedConstTableT;
        typedef typename TableT::size_type SizeT;
        typedef CTemplateProcessor<TableT, typename TargetFileT::OutputStreamT, TemplateLoader, LogOutputStreamT> TemplateProcessorT;
        typedef CTemplateLoader<TemplateProcessorT, StringT, LogOutputStreamT> TemplateLoaderT;

        typedef typename TargetFileT::OutputStreamT OutputStreamT;
        typedef typename TemplateLoaderT::InputStreamT InputStreamT;

        class TemplateLoader: public TemplateLoaderT {};

    public:
        typedef typename TemplateLoaderT::FileDataListT FileDataListT;
        typedef typename TemplateLoaderT::FileData FileDataT;

        CGenerator()
            : m_lastRowNumberWithFailure(1)
            , m_indexOfLastProcessedParameter(0)
            , m_csvDelimiterChars(STRING_LITERAL(";"))
            , m_csvCommentChars(STRING_LITERAL(""))
            , m_csvQuoteChars(STRING_LITERAL("\""))
            , m_logOutputStream(0)
        {
        }

        //noncopyable
        CGenerator(const CGenerator&) = delete;
        CGenerator& operator=(const CGenerator&) = delete;

        ///set delimiter for next csv table to load
        void setCsvDelimiterChars(const StringT& csvDelimiterChars)
        {
            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Setting CSV Delimiter:\n";
                *m_logOutputStream << "CSV Delimiter=" << csvDelimiterChars << "\n";
            }

            //check the delimiter
            CCsvParser::checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Delimiter, csvDelimiterChars, m_csvQuoteChars, m_csvCommentChars);
            m_csvDelimiterChars = csvDelimiterChars;
        }

        ///get delimiter for next csv table to load
        StringT getCsvDelimiterChars() const
        {
            return m_csvDelimiterChars;
        }

        ///set list of characters as string used for quoting text item for next csv table to load
        void setCsvQuoteChars(const StringT& csvQuoteChars)
        {
            //log
            if (m_logOutputStream)
            {
                *m_logOutputStream << "Setting CSV Quote Chars:\n";
                *m_logOutputStream << "CSV Quote Chars=" << csvQuoteChars << "\n";
            }

            //check the characters
            CCsvParser::checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Quote, m_csvDelimiterChars, csvQuoteChars, m_csvQuoteChars);
            //set the characters
            m_csvQuoteChars = csvQuoteChars;
        }

        ///get list of characters as string used for quoting text item for next csv table to load
        const StringT& getCsvQuoteChars() const
        {
            return m_csvQuoteChars;
        }

        ///set list of characters as string that mark commented lines for next csv table to load
        void setCsvCommentChars( const StringT& csvCommentChars)
        {
            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Setting CSV Comment Chars:\n";
                *m_logOutputStream << "CSV Comment Chars=" << csvCommentChars << "\n";
            }

            //check the characters
            CCsvParser::checkCharsUsedForCsvParsing<std::string>(CCsvParser::UsedCsvCharsCheck_Commenting, m_csvDelimiterChars, m_csvQuoteChars, csvCommentChars);
            //set the characters
            m_csvCommentChars = csvCommentChars;
        }

        ///get list of characters as string that mark commented lines for next csv table to load
        const StringT& getCsvCommentChars() const
        {
            return m_csvCommentChars;
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

            //load the table
            m_lastRowNumberWithFailure = 0;

            //create table and table builder
            typedef CVerticalTableBuilder<TableT> TableBuilderT;
            SharedTableT ptrTableToLoad = std::make_shared<TableT>();
            TableBuilderT tableBuidler( *ptrTableToLoad, padRows);
            try
            {
                //open table file
                CSourceFile<StringT, CsvFileT> file( tableFileName, tableFileName == STRING_LITERAL("-"));
                //parse the table file
                CCsvParser::parse( file.get(), tableBuidler, m_csvDelimiterChars, m_csvQuoteChars, m_csvCommentChars, m_positionTracker);
                //connect table to processor and keep reference in list
                m_templateProcessor.connectTable(ptrTableToLoad, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex, false);
            }
            catch(...)
            {
                m_lastRowNumberWithFailure = ptrTableToLoad->size() ? (*ptrTableToLoad)[0].size() : 1;
                throw;
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
            SharedTableT ptrTableToLoad = std::make_shared<TableT>();
            TableBuilderT tableBuidler( *ptrTableToLoad, padRows);

            try
            {
                //parse the table file
                CCsvParser::parse( inputStream, tableBuidler, m_csvDelimiterChars, m_csvQuoteChars, m_csvCommentChars, m_positionTracker);
                //connect table to processor and keep reference in list
                m_templateProcessor.connectTable(ptrTableToLoad, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex, false);
            }
            catch(...)
            {
                m_lastRowNumberWithFailure = ptrTableToLoad->size() ? (*ptrTableToLoad)[0].size() : 1;
                throw;
            }
        }

        ///load another table for generation, see also unloadTable
        void loadTable( std::shared_ptr<const TableT> table, const StringT& label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
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

            m_templateProcessor.connectTable( table, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex, false);
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
            generate( templateFileName, targetFileName, false, false, StringT(), append, parameters, false);
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
            bool canChangeTableList,
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
            SharedTableT ptrParameterTable = std::make_shared<TableT>();
            ParameterParser::parse( parameters, *ptrParameterTable, m_indexOfLastProcessedParameter, STRING_LITERAL('='));
            //connect parameter table if not empty
            if ( !ptrParameterTable->empty())
            {
                m_templateProcessor.connectTable(ptrParameterTable, STRING_LITERAL("__UserProvidedParameterTable"), true, false, 1, 1, true /*temporary*/);
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

            try
            {
                //control whether the templates can add tables during processing or remove tables loaded with loadTable()
                m_templateProcessor.setCanChangeNonTemporaryTableList(canChangeTableList);
                //reset the loader
                m_templateLoader.resetInclusionHierarchy();
                //open the template processor
                m_templateProcessor.open();
                //connect the objects
                m_templateLoader.connectOutputStream(&m_templateProcessor);
                m_templateProcessor.connectOutputStream(useIntermediateFile ? &intermediateFile.get() : &generatedFile.get());
                m_templateProcessor.connectTemplateLoader(&m_templateLoader);
                m_templateProcessor.setInlineTemplateParameters(inlineTemplateParameters);
                //start processing the template file
                m_templateLoader.loadTemplateFile(templateFileName, templateFileName == STRING_LITERAL("-"));
                //close everything
                m_templateProcessor.close();
                if (useIntermediateFile)
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
                m_templateProcessor.setCanChangeNonTemporaryTableList(true);
            }
            catch (...)
            {
                m_templateProcessor.setCanChangeNonTemporaryTableList(true);

                //update possible position data in case csv table parsing caused the error
                m_lastRowNumberWithFailure = m_templateProcessor.getLastCsvRowNumberWithFailure();
                m_positionTracker = m_templateProcessor.getCsvPositionWithFailure();
                throw;
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
            
            try
            {
                //control whether the templates can add tables during processing or remove tables loaded with loadTable()
                m_templateProcessor.setCanChangeNonTemporaryTableList(false);
                //reset the loader
                m_templateLoader.resetInclusionHierarchy();
                //open the template processor
                m_templateProcessor.open();
                //connect the objects
                m_templateLoader.connectOutputStream(&m_templateProcessor);
                m_templateProcessor.connectOutputStream(&outputStream);
                m_templateProcessor.connectTemplateLoader(&m_templateLoader);
                //start processing the template file
                m_templateLoader.loadTemplateStream(inputStream);
                //close everything
                m_templateProcessor.close();
                //clean up
                m_templateProcessor.connectOutputStream(0);
                m_templateProcessor.setCanChangeNonTemporaryTableList(true);
            }
            catch (...)
            {
                m_templateProcessor.setCanChangeNonTemporaryTableList(true);

                //update possible position data in case csv table parsing caused the error
                m_lastRowNumberWithFailure = m_templateProcessor.getLastCsvRowNumberWithFailure();
                m_positionTracker = m_templateProcessor.getCsvPositionWithFailure();
                throw;
            }
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
                *m_logOutputStream << "Clearing set include paths.\n";
            }
            m_templateLoader.reset();
            m_lastRowNumberWithFailure = 1;
            m_positionTracker.reset();
            m_indexOfLastProcessedParameter = 0;
            m_csvDelimiterChars = STRING_LITERAL(";");
            m_csvCommentChars = STRING_LITERAL("");
            m_csvQuoteChars = STRING_LITERAL("\"");
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

            SharedConstTableT disconnectedTable;
            m_templateProcessor.disconnectTable( label, true, disconnectedTable);
            assert(disconnectedTable);
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

        ///is empty when currently not loading, this is used to report error information
        StringT getTableLoadFileNameWithFailure()
        {
            return m_templateProcessor.getTableLoadFileNameWithFailure();
        }

        ///last column of table or template file with failure for error output
        unsigned int getLastColumnWithFailure() { return m_positionTracker.getColumn();}

        ///last line of table or template file with failure for error output
        unsigned int getLastLineWithFailure() { return m_positionTracker.getLine();}

        ///returns stack of files currently opened
        const FileDataListT& getInclusionHierarchy()
        {
            return m_templateLoader.getInclusionHierarchy();
        }

        ///can be used for error reporting
        const FileDataT& getLastTemplateFileProcessed() const
        {
            return m_templateLoader.getLastFileProcessed();
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
        TemplateProcessorT m_templateProcessor; ///<does the work
        TemplateLoader m_templateLoader; ///<the loader
        CPositionTracker m_positionTracker; ///<used by csv parser
        SizeT m_lastRowNumberWithFailure; ///<for error output
        SizeT m_indexOfLastProcessedParameter; ///<for error output
        StringT m_csvDelimiterChars; ///<delimiter used by csv files to load
        StringT m_csvCommentChars; ///<list of characters as string that mark commented lines in CSV-files
        StringT m_csvQuoteChars; ///< Specifies a list of characters as string that are used for quoting text items in CSV-files. The default is the double quote character.
        LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
