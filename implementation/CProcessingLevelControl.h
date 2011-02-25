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
    { public: ExPossibleInfiniteLoop() : std::runtime_error( "Possible infinite loop detected while expanding macro") {}};
};


template <typename ParserT, typename MacroProcessorT, typename LineCollectorT, typename OutputStreamT, typename FinalOutputStreamT>
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
    typedef CProcessingLevelControl<ParserT, MacroProcessorT, LineCollectorT, OutputStreamT, FinalOutputStreamT> ThisT;
    typedef typename ParserT::ParserTokenT TokenT;
    typedef typename ParserT::ParserStringT StringT;

    CProcessingLevelControl()
        : m_level( m_levelBlocks)
        , m_finalOutputStream(0)
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

    ///receives token input
    ThisT& operator <<( const TokenT& token)
    {
        //check if line without any tokens
        //if no processing in this or in upper levels is in progress
        //the line needs no further processing and can be put into the final output stream
        if ( token == TokenT::eFullLineWithoutTags && !m_level->isProcessing())
        {
            token.toStream( *m_finalOutputStream);
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

        //pass on text fragment
        localLevel->lineCollector << text;

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
            levelBlock.lineCollector.close();
        }
        m_level = m_levelBlocks;
    }

    ///return maximum number of recursion levels
    static int getMaxNumberOfRecursionLevels()
    {
        return m_cMaxNumLevel;
    }

private:
    static const int m_cMaxNumLevel = 32;

    ProcessingLevelBlocks* m_level;
    FinalOutputStreamT* m_finalOutputStream;

    ProcessingLevelBlocks m_levelBlocks[ m_cMaxNumLevel ];
};


#endif /* INCLUDED_CPROCESSINGLEVELCONTROL_H_01302737 */