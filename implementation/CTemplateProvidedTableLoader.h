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

#pragma once

#include "CCsvParser.h"
#include "CVerticalTableBuilder.h"
#include "CNul.h"
#include "StringLiteral.h"
#include "CPositionTracker.h"
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "CSourceFile.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#pragma warning( disable : 4996 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
#endif
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

namespace code_creation_kit
{
    class CsvFileLoadedViaTemplateT;

    ///defines exceptions thrown by CTemplateProvidedTableMap for template argument independent access
    class CTemplateProvidedTableLoaderExceptions
    {
    public:
        class ExUnexpectedTableProperty : public std::runtime_error
        {
        public: ExUnexpectedTableProperty() : std::runtime_error("Unexpected table property found for template-provided table.") {}
        };

        class ExTableLoadFileNameMustNotBeEmpty : public std::runtime_error
        {
        public: ExTableLoadFileNameMustNotBeEmpty() : std::runtime_error("Table filename must not be empty.") {}
        };
    };

    ///stores table data provided by template keywords TABLE_BEGIN and TABLE_END
    template <typename StringT, typename LogOutputStreamT = CNul >
    class CTemplateProvidedTableLoader : public CTemplateProvidedTableLoaderExceptions
    {
    public:
        typedef typename StringT::value_type CharT;
        typedef std::vector<std::vector<StringT> > TableT;
        typedef std::shared_ptr<TableT> SharedTableT;
        typedef std::shared_ptr<const TableT> SharedConstTableT;
        typedef std::basic_istream< CharT, std::char_traits<CharT> > CsvInputStreamT;

        ///the data of a table needed for connecting it to the macro processor
        struct TableData
        {
            TableData()
                : topDown(false)
                , leftRight(false)
                , permanent(false)
                , rowHeaderIndex(1)
                , columnHeaderIndex(1)
            {
            }

            bool empty()
            {
                return ptrTable == nullptr;
            }

            SharedTableT ptrTable;
            bool topDown;
            bool leftRight;
            bool permanent;
            unsigned int rowHeaderIndex;
            unsigned int columnHeaderIndex;
        };
        

        CTemplateProvidedTableLoader()
            : m_pLogOutputStream(nullptr)
            , m_lastRowNumberWithFailure(0)
        {
        }


        ~CTemplateProvidedTableLoader()
        {
        }


        ///connect log output stream
        void connectLogOutputStream(LogOutputStreamT* stream)
        {
            m_pLogOutputStream = stream;
        }


        TableData loadTable(
            const StringT& tableFileName,
            const StringT& label, //The label identifiying the table
            const StringT& csvDelimiter, //The first character specifies the delimiter for the next CSV-files to load. Cannot use 'tab' for tab separated items. Use an empty string for no delimiter.
            const StringT& csvCommentChars, //Specifies a list of characters as string that mark commented lines in CSV-files when found at the beginning of a line.
            const StringT& properties //possible semicolon separated values: top-down;left-to-right;pad-rows;csv-ignore-quotes;permanent
        )
        {
            //check the filename
            if (tableFileName.empty())
            {
                throw ExTableLoadFileNameMustNotBeEmpty();
            }

            //log
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << "Loading table initiated from template:\n";
                *m_pLogOutputStream << "TableFileName=" << tableFileName << "\n";
            }

            //this is used for print correct error information, flags that we are currently loading a table from file and not using a table from the template
            m_tableFileNameLoading = tableFileName;

            //open table file
            CSourceFile<StringT, CsvFileLoadedViaTemplateT> file(tableFileName, false);

            //load the table
            TableData tableData = loadTableImpl(
                file.get(),
                label,
                csvDelimiter,
                csvCommentChars,
                properties
            );

            //clear potential information belonging to an error
            //will not be empty if an exception is raised
            m_tableFileNameLoading.clear();

            return tableData;
        }


        TableData loadTable(
            CsvInputStreamT& inputStream, //the data stream containing the csv table
            const StringT& label, //The label identifiying the table
            const StringT& csvDelimiter, //The first character specifies the delimiter for the next CSV-files to load. Cannot use 'tab' for tab separated items. Use an empty string for no delimiter.
            const StringT& csvCommentChars, //Specifies a list of characters as string that mark commented lines in CSV-files when found at the beginning of a line.
            const StringT& properties //possible semicolon separated values: top-down;left-to-right;pad-rows;csv-ignore-quotes;permanent
        )
        {
            //make sure reading from file is not flagged
            m_tableFileNameLoading.clear();

            //log
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << "Loading table from template provided stream:\n";
            }

            //load the table
            TableData tableData = loadTableImpl(
                inputStream,
                label,
                csvDelimiter,
                csvCommentChars,
                properties
            );

            return tableData;
        }


    protected:
        TableData loadTableImpl(
            CsvInputStreamT& inputStream, //the data stream containing the csv table
            const StringT& label, //The label identifiying the table
            const StringT& csvDelimiter, //The first character specifies the delimiter for the next CSV-files to load. Cannot use 'tab' for tab separated items. Use an empty string for no delimiter.
            const StringT& csvCommentChars, //Specifies a list of characters as string that mark commented lines in CSV-files when found at the beginning of a line.
            const StringT& properties //possible semicolon separated values: top-down;left-to-right;pad-rows;csv-ignore-quotes;permanent
        )
        {
            //log
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << "Label=" << label << "\n";
                *m_pLogOutputStream << "Csv Delimiter (first char used)=" << csvDelimiter << "\n";
                *m_pLogOutputStream << "Csv Comment Chars=" << csvCommentChars << "\n";
                *m_pLogOutputStream << "Properties=" << properties << "\n";
            }

            TableData tableData;
            CharT csvDelimiterChar = csvDelimiter.empty() ? 0 : *csvDelimiter.begin();
            bool csvIgnoreDoubleQuotes = false;
            bool padRows = false;

            std::vector<StringT> propertyVector;
            boost::split(propertyVector, properties, boost::is_any_of(";"));
            for (const StringT& property : propertyVector)
            {
                if (property == STRING_LITERAL("top-down"))
                {
                    tableData.topDown = true;
                }
                else if (property == STRING_LITERAL("left-to-right"))
                {
                    tableData.leftRight = true;
                }
                else if (property == STRING_LITERAL("pad-rows"))
                {
                    padRows = true;
                }
                else if (property == STRING_LITERAL("csv-ignore-quotes"))
                {
                    csvIgnoreDoubleQuotes = true;
                }
                else if (property == STRING_LITERAL("permanent"))
                {
                    tableData.permanent = true;
                }
                else if (!property.empty())
                {
                    throw ExUnexpectedTableProperty();
                }
            }

            //if nothing is specified enable all
            if (!tableData.leftRight && !tableData.topDown)
            {
                tableData.topDown = true;
                tableData.leftRight = true;
            }

            //check input data
            CCsvParser::checkCharsUsedForCommenting(csvCommentChars, csvDelimiterChar, csvIgnoreDoubleQuotes, inputStream);
            CCsvParser::checkDelimiter(csvDelimiterChar, csvIgnoreDoubleQuotes, inputStream);

            try
            {
                m_lastRowNumberWithFailure = 0;

                //create table and table builder
                tableData.ptrTable = std::make_shared<TableT>();
                typedef CVerticalTableBuilder<TableT> TableBuilderT;
                TableBuilderT tableBuidler(*tableData.ptrTable, padRows);

                //parse the table file
                CCsvParser::parse(inputStream, tableBuidler, csvDelimiterChar, csvCommentChars, csvIgnoreDoubleQuotes, m_positionTracker);
            }
            catch (...)
            {
                m_lastRowNumberWithFailure = tableData.ptrTable->size() ? (*tableData.ptrTable)[0].size() : 1;
                throw;
            }

            return tableData;
        }


    public:
        void reset()
        {
            m_lastRowNumberWithFailure = 0;
            m_positionTracker.reset();
            m_tableFileNameLoading.clear();
        }


        ///is empty when currently not loading, this is used to report error information
        StringT getTableLoadFileNameWithFailure()
        {
            return m_tableFileNameLoading;
        }


        size_t getLastCsvRowNumberWithFailure()
        {
            return m_lastRowNumberWithFailure;
        }


        CPositionTracker getCsvPositionWithFailure()
        {
            return m_positionTracker;
        }

    private:
        LogOutputStreamT* m_pLogOutputStream; ///< used for logging purposes; NULL if not logging
        size_t m_lastRowNumberWithFailure;
        CPositionTracker m_positionTracker; ///<used by csv parser
        StringT m_tableFileNameLoading;
    };
}
