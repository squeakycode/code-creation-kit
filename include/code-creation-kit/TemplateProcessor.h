// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "ETokens.gen.h"
#include "Token.h"
#include "Tokenizer.gen.h"
#include "TemplatePreprocessor.h"
#include "Parser.h"
#include "MacroProcessor.h"
#include "LineCollector.h"
#include "BackEndTokenizer.gen.h"
#include "ProcessingLevelControl.h"
#include "InlineTemplateParameters.h"
#include "TemplateProvidedTableLoader.h"
#include "StringLiteral.h"

namespace code_creation_kit
{
    ///serves as default template loader
    class CNoTemplateLoader
    {
    public:
        ///dummy with no functionality
        template <typename StringT>
        void loadTemplateFile( const StringT&)
        {
        }

        template <typename StringT>
        StringT resolveFileNameForTableToLoad(const StringT& filename)
        {
            return filename;
        }
    };

    ///sets up und operates the building blocks needed for processing a template to produce generated output
    template <typename TableT, typename OutputStreamT, typename TemplateLoaderT = CNoTemplateLoader, typename LogOutputStreamT = CNul >
    class CTemplateProcessor
    {
        typedef CTemplateProcessor<TableT, OutputStreamT, TemplateLoaderT, LogOutputStreamT> ThisT;
        typedef typename TableT::value_type::value_type StringT;

        class BackEndTokenizer;
        class ProcessingLevelControl;
        typedef CLineCollector< StringT, BackEndTokenizer> LineCollectorT;
        typedef CMacroProcessor<TableT, ProcessingLevelControl, LogOutputStreamT> ProcessorT;
        typedef CToken<Tokens, StringT> TokenT;
        typedef CTemplateProvidedTableLoader<StringT, LogOutputStreamT> TemplateProvidedTableLoaderT;
        typedef CParser<ProcessorT, TemplateProvidedTableLoaderT, TokenT, StringT, LogOutputStreamT> ParserT;
        typedef CProcessingLevelControl<ParserT, TemplateProvidedTableLoaderT, ProcessorT, LineCollectorT, BackEndTokenizer, OutputStreamT, LogOutputStreamT> ProcessingLevelControlT;
        typedef CTemplatePreprocessor<ProcessingLevelControl, ThisT, TemplateLoaderT, TokenT, StringT> PreprocessorT;
        typedef CTokenizer<TokenT, StringT, PreprocessorT, OutputStreamT, LogOutputStreamT> TokenizerT;
        typedef CBackEndTokenizer<TokenT, StringT, PreprocessorT, LogOutputStreamT> BackEndTokenizerT;
        class BackEndTokenizer : public BackEndTokenizerT {};
        class ProcessingLevelControl : public ProcessingLevelControlT {};
        typedef std::shared_ptr<const TableT> SharedConstTableT;

    public:

        CTemplateProcessor()
        {
            //set default markup
            setDefaultMarkup();

            //setup tokenizer
            m_tokenizer.connectOutputStream( &m_preprocessor);

            //setup prepocessor
            m_preprocessor.connectPreprocessedStream( &m_processingLevelControl);
            m_preprocessor.connectMarkupObserver( this);

            //setup parser
            m_processingLevelControl.connectMacroProcessor( &m_processor);

            //setup processor
            m_processor.connectOutputStream( &m_processingLevelControl);

            //setup line collectors
            m_processingLevelControl.connectOutputStream( &m_backEndTokenizer);

            //setup tick remover
            m_backEndTokenizer.connectOutputStream( &m_preprocessor);

            //setup table loading
            m_processingLevelControl.connectTemplateProvidedTableLoader(&m_templateProvidedTableLoader);
        }

        ///attaches a table with the given properties, does not take ownership of the table
        void connectTable( const SharedConstTableT ptrTable, StringT label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool isTemporary)
        {
            m_processor.connectTable(ptrTable, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex, isTemporary);
        }

        ///detaches a table with the given properties
        bool disconnectTable( const StringT& label, bool throwIfNotFound, SharedConstTableT& ptrDisconnectedTable /*out*/)
        {
            return m_processor.disconnectTable( label, throwIfNotFound, ptrDisconnectedTable);
        }

        ///attaches output stream as sink for generated output
        void connectOutputStream( OutputStreamT* stream)
        {
            m_processingLevelControl.connectFinalOutputStream( stream);
            m_tokenizer.connectFinalOutputStream( stream);
        }

        ///attaches template loader used for include files
        void connectTemplateLoader( TemplateLoaderT* loader)
        {
            m_preprocessor.connectTemplateLoader( loader);
        }

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_tokenizer.connectLogOutputStream( stream);
            m_backEndTokenizer.connectLogOutputStream( stream);
            m_processingLevelControl.connectLogOutputStream( stream);
            m_processor.connectLogOutputStream( stream);
            m_templateProvidedTableLoader.connectLogOutputStream( stream);
        }

        ///resets the processor for next input stream, added for symmetry to close
        void open()
        {
            m_tokenizer.reset();
            m_processingLevelControl.open();
            m_backEndTokenizer.open();
        }

        ///processes a line of the input stream
        ThisT& operator <<( const StringT& line)
        {
            m_tokenizer << line;
            return *this;
        }

        ///sets new keyword markup, may be called during processing
        void setMarkup( const StringT& prefix, const StringT& postfix)
        {
            m_preprocessor.setPrefix( prefix);
            m_preprocessor.setPostfix( postfix);
            m_tokenizer.setMarkup( prefix, postfix);
            m_backEndTokenizer.setMarkup( prefix, postfix);
        }

        ///sets inline template processing parameters
        void setInlineTemplateParameters( const CInlineTemplateParameters<StringT>& p)
        {
            m_processingLevelControl.setInlineTemplateParameters( p.enabled, p.inlineGeneratedPostfix, p.inlinePad);
            m_tokenizer.setInlineTemplateMode( p.enabled);

            if ( p.enabled)
            {
                m_tokenizer.setInlineTemplateMarkup(
                    p.inlinePrefix,
                    p.inlinePostfix,
                    p.inlineGeneratedPostfix);
            }
        }

        ///closes all processing blocks performing consistency checks
        void close()
        {
            m_backEndTokenizer.close();
            m_processingLevelControl.close();
            m_processor.removeTemporaryTables();
        }

        ///resets all buidling blocks
        void reset()
        {
            m_tokenizer.reset();
            m_processingLevelControl.reset();
            m_processor.reset();
            m_templateProvidedTableLoader.reset();
            setDefaultMarkup();
        }

        ///this allows controlling the changes made by a template
        void setCanChangeNonTemporaryTableList(bool canChangeNonTemporaryTableList)
        {
            m_processor.setCanChangeNonTemporaryTableList(canChangeNonTemporaryTableList);
        }

        ///return maximum number of recursion levels
        static int getMaxNumberOfRecursionLevels()
        {
            return ProcessingLevelControl::getMaxNumberOfRecursionLevels();
        }

        ///return maximum text size of macro
        static size_t getMaxMacroTextSizeBytes()
        {
            return ParserT::getMaxMacroTextSizeBytes();
        }


        ///is empty when currently not loading, this is used to report error information
        StringT getTableLoadFileNameWithFailure()
        {
            return m_templateProvidedTableLoader.getTableLoadFileNameWithFailure();
        }


        size_t getLastCsvRowNumberWithFailure()
        {
            return m_templateProvidedTableLoader.getLastCsvRowNumberWithFailure();
        }


        CPositionTracker getCsvPositionWithFailure()
        {
            return m_templateProvidedTableLoader.getCsvPositionWithFailure();
        }

    private:
        ///set default markup
        void setDefaultMarkup()
        {
            typedef typename StringT::value_type CharT;
            setMarkup( STRING_LITERAL("["), STRING_LITERAL("]"));
        }

        TokenizerT m_tokenizer; ///<splits input lines into tokens
        BackEndTokenizer m_backEndTokenizer; ///<splits lines produced by a macro into tokens
        ProcessingLevelControl m_processingLevelControl; ///<controls the level used for processing in spiral recursion
        ProcessorT m_processor; ///<processes macro expressions
        PreprocessorT m_preprocessor; ///<preprocesses the input
        TemplateProvidedTableLoaderT m_templateProvidedTableLoader; ///<loader for tables provided by template keywords TABLE_BEGIN and TABLE_END
    };
}
