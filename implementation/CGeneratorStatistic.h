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

#include <set>
#include "CTemplateLoader.h"
#include "ETokens.gen.h"
#include "CToken.h"
#include "CTokenizer.gen.h"
#include "CTemplatePreprocessor.h"
#include "StringLiteral.h"
#include "CInlineTemplateParameters.h"

namespace code_creation_kit
{
    ///defines exceptions thrown by CGenerator for template argument independent access
    class CGeneratorStatisticExceptions
    {
    public:
    };

    ///serves generator stub and creating a statistic of the used files
    template <typename StringT>
    class CGeneratorStatistic : public CGeneratorStatisticExceptions
    {
        typedef CGeneratorStatistic<StringT> ThisT;
        typedef CToken<Tokens, StringT> TokenT;
        typedef CTemplatePreprocessor<CNul, ThisT, ThisT, TokenT, StringT> PreprocessorT;
        typedef CTokenizer<TokenT, StringT, PreprocessorT, CNul> TokenizerT;
        typedef CTemplateLoader<TokenizerT, StringT> TemplateLoaderT;

    public:
        typedef typename StringT::value_type CharT;
        typedef typename TemplateLoaderT::FileDataListT FileDataListT;
        typedef std::set<StringT> FileSetT;

        CGeneratorStatistic()
            : m_csvDelimiter( STRING_LITERAL(';'))
        {
            //set default markup
            setDefaultMarkup();

            //setup tokenizer
            m_tokenizer.connectOutputStream( &m_preprocessor);

            //setup prepocessor
            m_preprocessor.connectPreprocessedStream( &m_nul);
            m_preprocessor.connectMarkupObserver( this);
            m_preprocessor.connectTemplateLoader( this);
            m_templateLoader.connectOutputStream( &m_tokenizer);
        }

        //noncopyable
        CGeneratorStatistic(const CGeneratorStatistic&) = delete;
        CGeneratorStatistic& operator=(const CGeneratorStatistic&) = delete;

        ///set delimiter for next csv table to load
        void setCsvDelimiter( CharT delimiter)
        {
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
            m_csvCommentChars = commentChars;
        }

        ///get list of characters as string that mark commented lines for next csv table to load
        const StringT& getCsvCommentChars() const
        {
            return m_csvCommentChars;
        }

        void setCsvIgnoreDoubleQuotes( bool /*ignoreDoubleQuotes*/)
        {
        }

        ///load another table for generation, see also unloadTable
        void loadTable( const StringT& tableFileName, const StringT&, bool, bool, unsigned int, unsigned int, bool)
        {
            m_tables.insert( tableFileName);
        }

        ///load template file and keep stats
        void loadTemplateFile( const StringT& filename, bool useCinInstead = false)
        {
            if ( !useCinInstead )
            {
                m_templateFiles.insert( m_templateLoader.resolveFileName( filename));
            }
            m_templateLoader.loadTemplateFile( filename, useCinInstead);
        }

        ///generates output by processing a template file
        template <typename ParameterListT>
        void generate( 
            const StringT& templateFileName, 
            const StringT& targetFileName, 
            bool , 
            bool , 
            const StringT& , 
            bool , 
            const ParameterListT& ,
            const CInlineTemplateParameters<StringT>& p
            )
        {
            //setup tokenizer
            m_tokenizer.reset();
            m_tokenizer.setInlineTemplateMode( p.enabled);
            if ( p.enabled)
            {
                m_tokenizer.setInlineTemplateMarkup(
                    p.inlinePrefix,
                    p.inlinePostfix,
                    p.inlineGeneratedPostfix);
            }

            //reset
            m_templateLoader.resetInclusionHierarchy();

            //add to statistic
            if ( targetFileName != STRING_LITERAL("-")) //if not use cout
            {
                m_generatedFiles.insert( targetFileName);
            }

            //start processing the template file
            loadTemplateFile( templateFileName, templateFileName == STRING_LITERAL("-"));
        }

        //resets the generator building blocks
        void reset()
        {
            m_templateLoader.reset();
            m_tokenizer.reset();
            m_tables.clear();
            m_generatedFiles.clear();
            m_templateFiles.clear();
            m_csvDelimiter = STRING_LITERAL(';');
            setDefaultMarkup();
        }


        ///sets new tag markup
        void setMarkup( const StringT& prefix, const StringT& postfix)
        {
            m_preprocessor.setPrefix( prefix);
            m_preprocessor.setPostfix( postfix);
            m_tokenizer.setMarkup( prefix, postfix);
        }

        ///adds an include directory to the list
        void addIncludeDirectory( const StringT& directory)
        {
            m_templateLoader.addIncludeDirectory( directory);
        }

        ///returns stack of files currently opened
        const FileDataListT& getInclusionHierarchy()
        {
            return m_templateLoader.getInclusionHierarchy();
        }

        ///get list of loaded tables
        const FileSetT& getTableFiles() const
        {
            return m_tables;
        }

        ///get list of generated files
        const FileSetT& getGeneratedFiles() const
        {
            return m_generatedFiles;
        }

        ///get list of template files
        const FileSetT& getTemplateFiles() const
        {
            return m_templateFiles;
        }

        ///dummy only:
        void unloadTable( const StringT&){}
        unsigned int getLastRowNumberWithFailure() { return 1; }
        unsigned int getIndexOfLastProcessedParameter() { return 0; }
        unsigned int getLastColumnWithFailure() { return 1;}
        unsigned int getLastLineWithFailure() { return 1;}
        static int getMaxNumberOfRecursionLevels() { return 1; }
        static int getMaxMacroTextSizeBytes() { return 1; }
        template <typename LogOutputStreamT>
        void connectLogOutputStream( LogOutputStreamT*){}

    private:
        ///set default markup
        void setDefaultMarkup()
        {
            setMarkup( STRING_LITERAL("["), STRING_LITERAL("]"));
        }

        TokenizerT m_tokenizer; ///<splits input lines into tokens
        PreprocessorT m_preprocessor; ///<does the preprocessing
        TemplateLoaderT m_templateLoader; ///<the loader
        CNul m_nul; ///<dumps the data stream as no output is produced
        CharT m_csvDelimiter; ///<delimiter used by csv files to load
        FileSetT m_tables; ///<list of tables loaded
        FileSetT m_generatedFiles; ///<list of files generated
        FileSetT m_templateFiles; ///<list of template files loaded
        StringT m_csvCommentChars; ///<list of characters as string that mark commented lines in CSV-files
    };
}
