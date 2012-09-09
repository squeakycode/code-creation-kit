//   Copyright (C) 2011 Andreas Gau
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

#ifndef INCLUDED_CTEMPLATEPROCESSOR_H_3494989
#define INCLUDED_CTEMPLATEPROCESSOR_H_3494989

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "ETokens.gen.h"
#include "CToken.h"
#include "CTokenizer.gen.h"
#include "CTemplatePreprocessor.h"
#include "CParser.h"
#include "CMacroProcessor.h"
#include "CLineCollector.h"
#include "CBackEndTokenizer.gen.h"
#include "CProcessingLevelControl.h"
#include "CInlineTemplateParameters.h"

///serves as default template loader
class CNoTemplateLoader
{
public:
    ///dummy with no functionality
    template <typename StringT>
    void loadTemplateFile( const StringT&)
    {
    }
};

///sets up und operates the building blocks needed for processing a template to produce generated output
template <typename TableT, typename OutputStreamT, typename TemplateLoaderT = CNoTemplateLoader>
class CTemplateProcessor
{
    typedef CTemplateProcessor<TableT, OutputStreamT, TemplateLoaderT> ThisT;
    typedef typename TableT::value_type::value_type StringT;

    class BackEndTokenizer;
    class ProcessingLevelControl;
    typedef CLineCollector< StringT, BackEndTokenizer> LineCollectorT;
    typedef CMacroProcessor<TableT, ProcessingLevelControl> ProcessorT;
    typedef CToken<Tokens, StringT> TokenT;
    typedef CParser<ProcessorT, TokenT, StringT> ParserT;
    typedef CProcessingLevelControl<ParserT, ProcessorT, LineCollectorT, BackEndTokenizer, OutputStreamT> ProcessingLevelControlT;
    typedef CTemplatePreprocessor<ProcessingLevelControl, ThisT, TemplateLoaderT, TokenT, StringT> PreprocessorT;
    typedef CTokenizer<TokenT, StringT, PreprocessorT, OutputStreamT> TokenizerT;
    typedef CBackEndTokenizer<TokenT, StringT, PreprocessorT> BackEndTokenizerT;
    class BackEndTokenizer : public BackEndTokenizerT {};
    class ProcessingLevelControl : public ProcessingLevelControlT {};

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
    }

    ///attaches a table with the given properties, does not take ownership of the table
    void connectTable( const TableT* table, StringT label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
    {
        m_processor.connectTable( table, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
    }

    ///detaches a table with the given properties
    bool disconnectTable( const StringT& label, const TableT*& disconnectedTable /*out*/)
    {
        return m_processor.disconnectTable( label, disconnectedTable);
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

    ///resets the processor for next input stream, added for symmetry to close
    void open()
    {
        m_tokenizer.reset();
        m_processingLevelControl.open();
        m_backEndTokenizer.open();
    }

    ///processes a line of the input stream
    CTemplateProcessor< TableT, OutputStreamT, TemplateLoaderT>& operator <<( const StringT& line)
    {
        m_tokenizer << line;
        return *this;
    }

    ///sets new keyword markup, may be called during processing
    void setMarkup( const char* prefix, const char* postfix)
    {
        setMarkup( boost::lexical_cast<StringT>(prefix), boost::lexical_cast<StringT>(postfix));
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
       m_processingLevelControl.setInlineTemplateParameters( p.enabled, p.inlineGeneratedPostfix);
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
    }

    ///resets all buidling blocks
    void reset()
    {
        m_tokenizer.reset();
        m_processingLevelControl.reset();
        m_processor.reset();
        setDefaultMarkup();
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

private:
    ///set default markup
    void setDefaultMarkup()
    {
        setMarkup( boost::lexical_cast<StringT>("["), boost::lexical_cast<StringT>("]"));
    }

    TokenizerT m_tokenizer; ///<splits input lines into tokens
    BackEndTokenizer m_backEndTokenizer; ///<splits lines produced by a macro into tokens
    ProcessingLevelControl m_processingLevelControl; ///<controls the level used for processing in spiral recursion
    ProcessorT m_processor; ///<processes macro expressions
    PreprocessorT m_preprocessor; ///<preprocesses the input
};

#endif /* INCLUDED_CTEMPLATEPROCESSOR_H_3494989 */
