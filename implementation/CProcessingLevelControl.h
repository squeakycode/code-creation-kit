//  Copyright (c) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_CPROCESSINGLEVELCONTROL_H_01302737
#define INCLUDED_CPROCESSINGLEVELCONTROL_H_01302737

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/foreach.hpp>
#include <stdexcept>

///defines exceptions thrown by CProcessingLevelControlExceptions for template argument independent access
class CProcessingLevelControlExceptions
{
public:
    class ExPossibleInfiniteLoop : public std::runtime_error
    { public: ExPossibleInfiniteLoop() : std::runtime_error( "Possible infinite loop detected while expanding macro.") {}};

    class ExCannotSetRecursionLevelLimit : public std::runtime_error
    { public: ExCannotSetRecursionLevelLimit() : std::runtime_error( "The set recursion level limit directive can only be used at the beginning of a line and outside of a macro.") {}};
};


template <typename ParserT, typename MacroProcessorT, typename LineCollectorT, typename OutputStreamT, typename FinalOutputStreamT, typename LogOutputStreamT = CNul >
class CProcessingLevelControl : public CProcessingLevelControlExceptions
{
    ///represents a processing stage in the processing spiral
    ///holds the processing blocks having a state
    struct ProcessingLevelBlocks
    {
        ProcessingLevelBlocks()
            : nextLevelIsProcessing(false)
        {
        }

        bool isProcessing()
        {
            return
                    parser.processingInProgress()
                ||  lineCollector.processingInProgress()
                ||  nextLevelIsProcessing;
        }

        bool nextLevelIsProcessing; ///<upper level have processing in progress
        ParserT parser; ///<parses tokens
        LineCollectorT lineCollector; ///<produces a stream of lines from macro processor output
    };

public:
    typedef CProcessingLevelControl<ParserT, MacroProcessorT, LineCollectorT, OutputStreamT, FinalOutputStreamT, LogOutputStreamT> ThisT;
    typedef typename ParserT::ParserTokenT TokenT;
    typedef typename ParserT::ParserStringT StringT;
    typedef typename StringT::value_type CharT;

    CProcessingLevelControl()
        : m_level( m_levelBlocks)
        , m_levelLimit( m_levelBlocks + m_cMaxNumLevel)
        , m_finalOutputStream(0)
        , m_outputStream(0)
        , m_inlineTemplateMode(false)
        , m_numInlinePad(0)
    {

    }

    ///attaches stream for the final output
    void connectFinalOutputStream( FinalOutputStreamT* stream)
    {
        m_finalOutputStream = stream;
    }

    ///attaches output stream
    void connectOutputStream( OutputStreamT* stream)
    {
        m_outputStream = stream;
        BOOST_FOREACH( ProcessingLevelBlocks& levelBlock, m_levelBlocks)
        {
            levelBlock.lineCollector.connectOutputStream( stream);
        }
    }

    ///attaches the macro processor for the next level
    void connectMacroProcessor( MacroProcessorT* processor)
    {
        BOOST_FOREACH( ProcessingLevelBlocks& levelBlock, m_levelBlocks)
        {
            levelBlock.parser.connectOutputStream( processor);
        }
    }

    ///connect log output stream
    void connectLogOutputStream( LogOutputStreamT* stream)
    {
        BOOST_FOREACH( ProcessingLevelBlocks& levelBlock, m_levelBlocks)
        {
            levelBlock.parser.connectLogOutputStream( stream, &levelBlock - m_levelBlocks);
        }
    }

    ///receives token input
    ThisT& operator <<( const TokenT& token)
    {
        //check for recursion level limitation directive
        if ( token == TokenT::eSetRecursionLevelLimit || token == TokenT::eSetRecursionLevelLimitOff)
        {
            //can set limit now?
            if ( m_level->isProcessing())
            {
                throw ExCannotSetRecursionLevelLimit();
            }
            
            if ( token == TokenT::eSetRecursionLevelLimit)
            {
                m_levelLimit = m_level;
            }
            else
            {
                m_levelLimit = m_levelBlocks + m_cMaxNumLevel;
                //process tokens normally
                m_outputStream->setBypassMode( false);
            }
        }
        else if ( 
            //check if line without any tokens
            //if no processing in this or in upper levels is in progress
            //the line needs no further processing and can be put into the final output stream
            ( token == TokenT::eFullLineWithoutTags && !m_level->isProcessing() )
            //check whether recursion level limit has been reached
            //if so output the text tokens
            || ( m_level > m_levelLimit)
            )
        {
            if ( !m_inlineTemplateMode)
            {
                token.toStream( *m_finalOutputStream);
            }
            else
            {
                //insert inline generated postfix
                typename TokenT::ConstSharedStringListT stringList = token.getStringList();
                if ( stringList)
                {
                    const typename TokenT::StringListT& strings = *stringList;
                    BOOST_FOREACH( const StringT& text, strings)
                    {
                        if ( !text.empty())
                        {
                            typename StringT::const_iterator last = --text.end();
                            if ( *last == STRING_LITERAL('\n'))
                            {
                                bool withCarriageReturn = false;
                                if ( last != text.begin() && *(last - 1) == STRING_LITERAL('\r'))
                                {
                                    --last;
                                    withCarriageReturn = true;
                                }

                                StringT temp;
                                temp.assign(text.begin(), last);
                                if ( temp.size() < m_numInlinePad)
                                {
                                    temp.resize( m_numInlinePad, STRING_LITERAL(' '));
                                }
                                *m_finalOutputStream << temp;
                                *m_finalOutputStream 
                                    << (withCarriageReturn 
                                        ? m_inlineGeneratedPostfixAndCarriageReturnNewLine
                                        : m_inlineGeneratedPostfixAndNewLine);
                            }
                            else
                            {
                                *m_finalOutputStream << text;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            m_level->parser << token;
        }

        return *this;
    }

    ///receives the output of the macro processor and places it into the correct line collector
    ThisT& operator <<( const StringT& text)
    {
        //increase to next level
        ProcessingLevelBlocks* localLevel = m_level;
        ++m_level;
        if ( m_level >= &m_levelBlocks[ m_cMaxNumLevel ])
        {
            throw ExPossibleInfiniteLoop();
        }

        if ( m_level > m_levelLimit)
        {
            //limit the processing by treating tokens as text
            //output stream is the macro processor
            m_outputStream->setBypassMode( true);

            //pass on text fragment
            localLevel->lineCollector << text;

            //process tokens normally
            m_outputStream->setBypassMode( false);
        }
        else
        {
            //pass on text fragment
            localLevel->lineCollector << text;
        }

        //store upper stages are processing
        localLevel->nextLevelIsProcessing = m_level->isProcessing();

        //lower the processing level again
        --m_level;

        return *this;
    }

    ///reset everything
    void reset()
    {
        m_level = m_levelBlocks;
        m_levelLimit = m_levelBlocks + m_cMaxNumLevel;

        BOOST_FOREACH( ProcessingLevelBlocks& levelBlock, m_levelBlocks)
        {
            levelBlock.lineCollector.reset();
            levelBlock.parser.reset();
            levelBlock.nextLevelIsProcessing = false;
        }
    }

    ///reset everything
    void open()
    {
        reset();
    }

    ///close all blocks flushing the content
    void close()
    {
        BOOST_FOREACH( ProcessingLevelBlocks& levelBlock, m_levelBlocks)
        {
            if ( &levelBlock >= &m_levelBlocks[ m_cMaxNumLevel ])
            {
                //still processing in last level?
                if ( levelBlock.isProcessing())
                {
                    //cannot process any further, exit with error
                    throw ExPossibleInfiniteLoop();
                }
                else
                {
                    //done
                    break;
                }
            }

            //close the level
            m_level = &levelBlock;
            levelBlock.parser.close();
            ++m_level;

            //recursion level limit reached?
            if ( m_level > m_levelLimit)
            {
                //limit the processing by treating tokens as text
                //output stream is the macro processor
                m_outputStream->setBypassMode( true);
            }

            //close
            levelBlock.lineCollector.close();
        }
        m_outputStream->setBypassMode( false);
        m_level = m_levelBlocks;
        m_levelLimit = m_levelBlocks + m_cMaxNumLevel;
    }

    ///return maximum number of recursion levels
    static int getMaxNumberOfRecursionLevels()
    {
        return m_cMaxNumLevel;
    }

    ///set inline template processing parameters
    void setInlineTemplateParameters( bool enabled, const StringT& inlineGeneratedPostfix, size_t numInlinePad)
    {
        m_inlineTemplateMode = enabled;
        m_inlineGeneratedPostfixAndNewLine = inlineGeneratedPostfix + STRING_LITERAL('\n');
        m_inlineGeneratedPostfixAndCarriageReturnNewLine = inlineGeneratedPostfix + STRING_LITERAL("\r\n");
        m_numInlinePad = numInlinePad > 64*1024 ? 64*1024 : numInlinePad; //clip value
    }

private:
    static const unsigned int m_cMaxNumLevel = 32;

    ProcessingLevelBlocks* m_level;
    ProcessingLevelBlocks* m_levelLimit;
    FinalOutputStreamT* m_finalOutputStream;
    OutputStreamT* m_outputStream;
    bool m_inlineTemplateMode; ///<toggles inline template processing
    StringT m_inlineGeneratedPostfixAndNewLine; ///< marks a generated line
    StringT m_inlineGeneratedPostfixAndCarriageReturnNewLine; ///< marks a generated line
    size_t m_numInlinePad; ///< if a line has less chars than this value then pad with spaces

    ProcessingLevelBlocks m_levelBlocks[ m_cMaxNumLevel ];
};


#endif /* INCLUDED_CPROCESSINGLEVELCONTROL_H_01302737 */