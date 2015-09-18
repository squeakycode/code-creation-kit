//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_CPARSER_H_1584436
#define INCLUDED_CPARSER_H_1584436

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "CMacro.h"
#include <vector>
#include <boost/foreach.hpp>

///defines exceptions thrown by CParser for template argument independent access
class CParserExceptions
{
public:
    class ExMissingBlockBegin : public std::runtime_error 
    { public: ExMissingBlockBegin() : std::runtime_error( "Missing begin block marker.") {}};

    class ExMissingMacroBegin : public std::runtime_error 
    { public: ExMissingMacroBegin() : std::runtime_error( "Missing begin macro marker.") {}};

    class ExMissingBlockEnd : public std::runtime_error 
    { public: ExMissingBlockEnd() : std::runtime_error( "Missing block end marker.") {}};

    class ExMissingMacroEnd : public std::runtime_error 
    { public: ExMissingMacroEnd() : std::runtime_error( "Missing macro end marker.") {}};

    class ExUnexpectedKeyword : public std::runtime_error 
    { public: ExUnexpectedKeyword() : std::runtime_error( "Syntax error. Unexpected keyword found.") {}};

    class ExUnexpectedEndOfMacro : public std::runtime_error 
    { public: ExUnexpectedEndOfMacro() : std::runtime_error( "Unexpected end of macro.") {}};

    class ExDirectiveAlreadyApplied : public std::runtime_error 
    { public: ExDirectiveAlreadyApplied() : std::runtime_error( "Directive has been applied already.") {}};

    class ExConstraintExpectedAfterNot : public std::runtime_error 
    { public: ExConstraintExpectedAfterNot() : std::runtime_error( "Constraint following not expected.") {}};

    class ExSubstitutionExpectedAfterIf : public std::runtime_error 
    { public: ExSubstitutionExpectedAfterIf() : std::runtime_error( "Substitution following if expected.") {}};

    class ExCannotApplyDirectiveToSubstitution : public std::runtime_error 
    { public: ExCannotApplyDirectiveToSubstitution() : std::runtime_error( "Directive cannot be applied to this substitution.") {}};

    class ExCannotApplyDirectiveToConstraint : public std::runtime_error 
    { public: ExCannotApplyDirectiveToConstraint() : std::runtime_error( "Directive cannot be applied to this constraint.") {}};

    class ExCannotApplyDirectiveToConversion : public std::runtime_error 
    { public: ExCannotApplyDirectiveToConversion() : std::runtime_error( "Directive cannot be applied to this conversion.") {}};

    class ExCannotApplyConstraintToSubstitution : public std::runtime_error 
    { public: ExCannotApplyConstraintToSubstitution() : std::runtime_error( "Constraint cannot be applied to this substitution.") {}};

    class ExCannotApplyConversionToSubstitution : public std::runtime_error 
    { public: ExCannotApplyConversionToSubstitution() : std::runtime_error( "Conversion cannot be applied to this substitution.") {}};

    class ExSubstitutionParsingBadOrder : public std::runtime_error 
    { public: ExSubstitutionParsingBadOrder() : std::runtime_error( "Extensions fo a substitution are expected in the order directives, constraints, conversions.") {}};

    class ExSubstitutionRequiresIf : public std::runtime_error 
    { public: ExSubstitutionRequiresIf() : std::runtime_error( "Substitution requires if.") {}};

    class ExMacroTooLarge : public std::runtime_error 
    { public: ExMacroTooLarge() : std::runtime_error( "The macro exceeds the maximum allowed size.") {}};
};

#include "ParserExtensions.gen.h"

template <typename OutputStreamT, typename TokenT, typename StringT, typename LogOutputStreamT = CNul >
class CParser : public CParserExceptions
{
public:
    typedef CParser<OutputStreamT, TokenT, StringT, LogOutputStreamT> ThisT;
    typedef CMacro<StringT> MacroT;
    typedef std::vector<TokenT> StackT;
    typedef typename StackT::const_iterator PosT;
    typedef typename MacroT::MacroExpressionT MacroExpressionT;
    typedef typename MacroT::SubstitutionT SubstitutionT;
    typedef typename MacroT::IndexT IndexT;
    typedef TokenT ParserTokenT;
    typedef StringT ParserStringT;


    CParser()
        : m_outputStream( 0)
        , m_currentMacroTextSize( 0)
        , m_level(0)
        , m_logOutputStream(0)
    {
    }

    ///attaches output stream as sink for macros
    void connectOutputStream( OutputStreamT* stream)
    {
        m_outputStream = stream;
    }

    ///connect log output stream
    void connectLogOutputStream( LogOutputStreamT* stream, size_t level)
    {
        m_level = level;
        m_logOutputStream = stream;
    }

    ///release items on the stack, returns true if something has beed flushed
    bool close()
    {
        bool flushed = !m_stack.empty();
        parseStack();
        return flushed;
    }

    ///return true if currently processing a macro
    bool processingInProgress()
    {
        return !m_stack.empty();
    }

    ///clear items on the stack
    void open()
    {
        reset();
    }

    ///clear items on the stack
    void reset()
    {
        m_currentMacroTextSize = 0;
        m_stack.clear();
    }

    ///process tokens, pass on unprocessed text, pass on macros
    ThisT& operator <<( const TokenT& token)
    {
        //monitor macro size
        m_currentMacroTextSize += token.getTextSize();
        if ( m_currentMacroTextSize > cMaxAllowedMacroSize)
        {
            throw ExMacroTooLarge();
        }

        if ( token == TokenT::eMacroBegin)
        {
            parseStack();
            m_stack.push_back( token);
        }
        else if ( token == TokenT::eMacroEnd)
        {
            m_stack.push_back( token);
            parseStack();
        }
        else if ( token == TokenT::eNewLine || token == TokenT::eFullLineWithoutTags)
        {
            if (!m_stack.empty() && m_stack.front() == TokenT::eMacroBegin) //if inside macro block
            {
                if (token == TokenT::eNewLine && !token.getStringList())
                {
                    //trimmed line, ignore new line
                }
                else
                {
                    m_stack.push_back(token);
                }
            }
            else
            {
                m_stack.push_back(token);
                parseStack();
            }
        }
        else
        {
            m_stack.push_back( token);
        }

        return *this;
    }

    ///return maximum text size of macro
    static size_t getMaxMacroTextSizeBytes()
    {
        return cMaxAllowedMacroSize;
    }

private:
    ///parses the tokens on the stack
    void parseStack()
    {
        if ( !m_stack.empty())
        {
            if ( parseTextOnly())
            {
                outputText( m_stack, m_outputStream);
                m_stack.clear();
                m_currentMacroTextSize = 0;
            }
            else
            {
                //parse the macro and pass it to the macro processor
                MacroT macro;
                parseMacro( macro);
                m_stack.clear();
                m_currentMacroTextSize = 0;
                *m_outputStream << macro;
            }
        }
    }

    ///check is text only
    bool parseTextOnly()
    {
        BOOST_FOREACH( const TokenT& token, m_stack)
        {
            if (   token != TokenT::eFullLineWithoutTags
                && token != TokenT::eTextFragment
                && token != TokenT::eNewLine 
                )
            {
                return false;
            }
        }
        return true;
    }

    ///output text only fragments on the stack
    template<typename TextOutputT>
    void outputText(const StackT& stack, TextOutputT* output)
    {
        //output the text fragments
        BOOST_FOREACH( const TokenT& token, stack)
        {
            token.toStream( *output);
        }
    }

    ///start parsing a macro
    void parseMacro( MacroT& macro)
    {
        //log
        if ( m_logOutputStream)
        {
            *m_logOutputStream << "Found macro (level " << m_level << "):\n";
            BOOST_FOREACH( const TokenT& token, m_stack)
            {
                token.sourceTextToStream( *m_logOutputStream);
            }
            *m_logOutputStream << "\n";
        }

        MacroExpressionT expression;

        PosT pos = m_stack.begin();

        try
        {
            if (  pos != m_stack.end())
            {
                if ( *pos == TokenT::eMacroBegin)
                {
                    ++pos;
                    MacroExpressionT block;
                    parseBlock<ExMissingMacroEnd>( pos, block, TokenT::eMacroEnd, macro);
                    expression.attach( block);
                }
                else
                {
                    MacroExpressionT blockContent;
                    parseBlockContent( pos, blockContent, macro);
                    expression.attach( blockContent);
                }
            }
            else
            {
                throw ExUnexpectedEndOfMacro();
            }

            if ( pos != m_stack.end())
            {
                if ( *pos == TokenT::eEnd)
                {
                    throw ExMissingBlockBegin();
                }
                else if ( *pos == TokenT::eMacroEnd)
                {
                    throw ExMissingMacroBegin();
                }
                else
                {
                    throw ExUnexpectedKeyword();
                }
            }

            macro.attach( expression);
        }
        catch(...)
        {
            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Successfully processed part of macro:\n";
                for ( PosT it = m_stack.begin(); it != pos; ++it)
                {
                    it->sourceTextToStream( *m_logOutputStream);
                }
                *m_logOutputStream << "\n";
            }
            throw;
        }
    }

    ///parse a block
    template <typename MissingEndExceptionT>
    void parseBlock( PosT& pos, MacroExpressionT& expression, typename TokenT::ETokenT expectedEndMarker, MacroT& macro)
    {
        if ( pos != m_stack.end() && *pos == expectedEndMarker)
        {
            ++pos;
            return;
        }

        MacroExpressionT blockContent;
        parseBlockContent( pos, blockContent, macro);
        expression.attach( blockContent);

        if ( pos != m_stack.end() && *pos == expectedEndMarker)
        {
            ++pos;
            return;
        }

        if ( pos != m_stack.end() && *pos == TokenT::eEnd)
        {
            throw ExMissingBlockBegin();
        }

        throw MissingEndExceptionT();
    }

    ///parse block content
    void parseBlockContent( PosT& pos, MacroExpressionT& expression, MacroT& macro)
    {
        while ( pos != m_stack.end()) 
        {
            if ( *pos == TokenT::eNewLine || *pos == TokenT::eFullLineWithoutTags || *pos == TokenT::eTextFragment)
            {
                addText( pos, expression);
                ++pos;
            }
            else if ( *pos == TokenT::eOr_)
            {
                expression.ored(); //flag list of expressions as alternatives
                ++pos;
            }
            else if ( *pos == TokenT::eBegin)
            {
                ++pos;
                MacroExpressionT block;
                parseBlock<ExMissingBlockEnd>( pos, block, TokenT::eEnd, macro);
                expression.attach( block);
            }
            else if ( isSubstitution( pos) || *pos == TokenT::eIf_ )
            {
                SubstitutionT substitution;
                PosT end = m_stack.end();
                parseSubstitution( pos, end, substitution);

                if ( (
                    substitution == SubstitutionT::eFirstTime
                    || substitution == SubstitutionT::eLastTime)
                    && !substitution.if_()
                    )
                {
                    throw ExSubstitutionRequiresIf();
                }

                IndexT index = macro.addSubstitution( substitution);
                expression.add( index);
            }
            else if (  isDirectiveForSubstitution( pos)
                || isDirectiveForConstraint( pos)
                || isDirectiveForConversion( pos)
                || isConstraint( pos)
                || isConversion( pos)
                )
            {
                throw ExSubstitutionParsingBadOrder();
            }
            else
            {
                break;
            }
        }    
    }

    ///attaches text on current position
    void addText( PosT& pos, MacroExpressionT& expression)
    {
        if ( pos->getStringList())
        {
            const typename TokenT::StringListT& textList = *(pos->getStringList());
            BOOST_FOREACH( const StringT& text, textList)
            {
                expression.add( text.begin(), text.end());
            }
        }
    }

private:

    OutputStreamT* m_outputStream; ///<sink for macros, also excepts text around macros
    StackT m_stack;
    size_t m_currentMacroTextSize;
    size_t m_level; ///<used for logging purposes
    LogOutputStreamT* m_logOutputStream; ///<used for logging purposes; NULL if not logging
    static const size_t cMaxAllowedMacroSize = 2 * 1024 * 1024; ///<randomly chosen value for catching error conditions
};

#endif /* INCLUDED_CPARSER_H_1584436 */
