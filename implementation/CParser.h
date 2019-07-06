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

#include "CMacro.h"
#include <vector>
#include <map>
#include <cassert>

namespace code_creation_kit
{
    ///defines exceptions thrown by CParser for template argument independent access
    class CParserExceptions
    {
    public:
        class ExMissingBlockBegin : public std::runtime_error
        {
        public: ExMissingBlockBegin() : std::runtime_error("Missing begin block marker.") {}
        };

        class ExMissingMacroBegin : public std::runtime_error
        {
        public: ExMissingMacroBegin() : std::runtime_error("Missing begin macro marker.") {}
        };

        class ExMissingBlockEnd : public std::runtime_error
        {
        public: ExMissingBlockEnd() : std::runtime_error("Missing block end marker.") {}
        };

        class ExMissingMacroEnd : public std::runtime_error
        {
        public: ExMissingMacroEnd() : std::runtime_error("Missing macro end marker.") {}
        };

        class ExUnexpectedKeyword : public std::runtime_error
        {
        public: ExUnexpectedKeyword() : std::runtime_error("Syntax error. Unexpected keyword found.") {}
        };

        class ExUnexpectedEndOfMacro : public std::runtime_error
        {
        public: ExUnexpectedEndOfMacro() : std::runtime_error("Unexpected end of macro.") {}
        };

        class ExDirectiveAlreadyApplied : public std::runtime_error
        {
        public: ExDirectiveAlreadyApplied() : std::runtime_error("Directive has been applied already.") {}
        };

        class ExConstraintExpectedAfterNot : public std::runtime_error
        {
        public: ExConstraintExpectedAfterNot() : std::runtime_error("Constraint following not expected.") {}
        };

        class ExSubstitutionExpectedAfterIf : public std::runtime_error
        {
        public: ExSubstitutionExpectedAfterIf() : std::runtime_error("Substitution following if expected.") {}
        };

        class ExCannotApplyDirectiveToSubstitution : public std::runtime_error
        {
        public: ExCannotApplyDirectiveToSubstitution() : std::runtime_error("Directive cannot be applied to this substitution.") {}
        };

        class ExCannotApplyDirectiveToConstraint : public std::runtime_error
        {
        public: ExCannotApplyDirectiveToConstraint() : std::runtime_error("Directive cannot be applied to this constraint.") {}
        };

        class ExCannotApplyDirectiveToConversion : public std::runtime_error
        {
        public: ExCannotApplyDirectiveToConversion() : std::runtime_error("Directive cannot be applied to this conversion.") {}
        };

        class ExCannotApplyConstraintToSubstitution : public std::runtime_error
        {
        public: ExCannotApplyConstraintToSubstitution() : std::runtime_error("Constraint cannot be applied to this substitution.") {}
        };

        class ExCannotApplyConversionToSubstitution : public std::runtime_error
        {
        public: ExCannotApplyConversionToSubstitution() : std::runtime_error("Conversion cannot be applied to this substitution.") {}
        };

        class ExSubstitutionParsingBadOrder : public std::runtime_error
        {
        public: ExSubstitutionParsingBadOrder() : std::runtime_error("Extensions fo a substitution are expected in the order directives, constraints, conversions.") {}
        };

        class ExSubstitutionRequiresIf : public std::runtime_error
        {
        public: ExSubstitutionRequiresIf() : std::runtime_error("Substitution requires if.") {}
        };

        class ExMacroTooLarge : public std::runtime_error
        {
        public: ExMacroTooLarge() : std::runtime_error("The macro exceeds the maximum allowed size.") {}
        };

        class ExPartBlocksCannotBeNested : public std::runtime_error
        {
        public: ExPartBlocksCannotBeNested() : std::runtime_error("Part blocks cannot be nested.") {}
        };

        class ExMissingPartBegin : public std::runtime_error
        {
        public: ExMissingPartBegin() : std::runtime_error("Missing begin part marker.") {}
        };

        class ExMissingPartEnd : public std::runtime_error
        {
        public: ExMissingPartEnd() : std::runtime_error("Missing part end marker.") {}
        };

        class ExPartAlreadyDefined : public std::runtime_error
        {
        public: ExPartAlreadyDefined() : std::runtime_error("Part already defined. Cannot add a second part with the same label.") {}
        };

        class ExPartNotDefined : public std::runtime_error
        {
        public: ExPartNotDefined() : std::runtime_error("Part not defined. Cannot expand part.") {}
        };

        class ExPossibleInfiniteLoop : public std::runtime_error
        {
        public: ExPossibleInfiniteLoop() : std::runtime_error("Possible infinite loop detected while expanding part.") {}
        };
    };
}

#include "ParserExtensions.gen.h"

namespace code_creation_kit
{
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
        typedef std::shared_ptr<StackT> SharedStackT;
        typedef std::map<StringT, SharedStackT> PartMapT;


        CParser()
            : m_pOutputStream(nullptr)
            , m_currentMacroTextSize(0)
            , m_level(0)
            , m_partExpansionRecursionLevel(0)
            , m_pLogOutputStream(nullptr)
            , m_pPartMap(nullptr)
        {
        }

        ///attaches the map for part blocks defined by PART_BEGIN and PART_END
        ///one part map is used by all parsers from different processing levels
        void connectPartMap(PartMapT* pPartMap)
        {
            m_pPartMap = pPartMap;
        }

        ///attaches output stream as sink for macros
        void connectOutputStream( OutputStreamT* stream)
        {
            m_pOutputStream = stream;
        }

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream, size_t level)
        {
            m_level = level;
            m_pLogOutputStream = stream;
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
            m_partExpansionRecursionLevel = 0;
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

            if (!m_stack.empty() && m_stack.front() == TokenT::ePartBegin) //if inside part block
            {
                if (token == TokenT::ePartBegin)
                {
                    //we transfer the processing to parseStack()->parsePartBlock()
                    //because parseStack will also be called if the processing finishes calling close()
                    m_stack.push_back(token);
                    parseStack(); //will throw here
                }
                else if (token == TokenT::ePartEnd)
                {
                    //done, we can add the part to the part map
                    m_stack.push_back(token);
                    parseStack();
                }
                else //collect any other token
                {
                    m_stack.push_back(token);
                }
            }
            else if (token == TokenT::ePart)
            {
                //expand a previously defined part here
                processPartToken(token);
            }
            else if (token == TokenT::ePartRemove)
            {
                //remove a previously defined part from the part map
                processPartRemoveToken(token);
            }
            else if (token == TokenT::ePartEnd)
            {
                throw ExMissingPartBegin();
            }
            else if (token == TokenT::ePartBegin)
            {
                parseStack();
                m_stack.push_back(token);
            }
            else if ( token == TokenT::eMacroBegin)
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
        ///processes a part token
        void processPartToken(const TokenT& token)
        {
            //parts may not be available in different configuration of the code
            if (!m_pPartMap)
            {
                throw std::runtime_error("Internal error. Part map is not available.");
            }

            //get the part label
            const StringT& partLabel = token.getStringList()->at(0);

            try
            {
                ++m_partExpansionRecursionLevel;
                if (m_partExpansionRecursionLevel > m_partExpansionRecursionLevelLimit)
                {
                    throw ExPossibleInfiniteLoop();
                }

                //log that we are about to expand the part with the given label
                if (m_pLogOutputStream)
                {
                    *m_pLogOutputStream << "Expanding part with label: " << partLabel << "\n";
                }

                //try to remove the part from the part map
                auto pos = m_pPartMap->find(partLabel);
                if (pos != m_pPartMap->end())
                {
                    assert(pos->second->size() >= 2);
                    if (pos->second->size() > 2)
                    {
                        //keep a hold on the stack, someone else may remove it from the part map while feeding the tokens below
                        SharedStackT localStack = pos->second;

                        auto itBegin = localStack->cbegin() + 1; //remove begin marker
                        auto itEnd = localStack->cend() - 1; //remove end marker
                        for (auto it = itBegin; it != itEnd; ++it)
                        {
                            (*this) << *it; //feed back the tokens of the part for parsing
                        }
                    }
                }
                else
                {
                    throw ExPartNotDefined();
                }
            }
            catch (...)
            {
                assert(m_partExpansionRecursionLevel);
                if (m_partExpansionRecursionLevel > 0)
                {
                    --m_partExpansionRecursionLevel;
                }
                throw;
            }
            assert(m_partExpansionRecursionLevel);
            if (m_partExpansionRecursionLevel > 0)
            {
                --m_partExpansionRecursionLevel;
            }
        }


        ///processes a part remove token
        void processPartRemoveToken(const TokenT& token)
        {
            if (m_pPartMap)
            {
                //get the part label
                const StringT& partLabel = token.getStringList()->at(0);

                //try to remove the part from the part map
                auto pos = m_pPartMap->find(partLabel);
                if (pos != m_pPartMap->end())
                {
                    //log that we are about to remove the part with the given label
                    if (m_pLogOutputStream)
                    {
                        *m_pLogOutputStream << "Removing part with label: " << partLabel << "\n";
                    }

                    m_pPartMap->erase(pos);
                }
                else
                {
                    //if the part is not found consider it removed. this is not an error
                    //log that we are about to remove the part with the given label
                    if (m_pLogOutputStream)
                    {
                        *m_pLogOutputStream << "Part to remove not found with label: " << partLabel << "\n";
                    }
                }
            }
        }


        ///processes a part end token
        void parsePartBlock()
        {
            if (!m_stack.empty())
            {
                //part not closed correctly, or nested part block
                if (m_stack.size() >= 2 && m_stack.front() == TokenT::ePartBegin && m_stack.back() == TokenT::ePartBegin)
                {
                    //log
                    logStackSourceText("Unexpected part begin marker in started part block");
                    //we are already in a part block
                    throw ExPartBlocksCannotBeNested();
                }
                //correct part block
                else if (m_stack.size() >= 2 && m_stack.front() == TokenT::ePartBegin && m_stack.back() == TokenT::ePartEnd)
                {
                    //log
                    logStackSourceText("Found part");

                    //parts may not be available in different configuration of the code
                    if (!m_pPartMap)
                    {
                        throw std::runtime_error("Internal error. Part map is not available.");
                    }

                    //get the part label
                    const StringT& partLabel = m_stack.front().getStringList()->at(0);

                    //log that we are about to add the part with the given label
                    if (m_pLogOutputStream)
                    {
                        *m_pLogOutputStream << "Adding part with label: " << partLabel << "\n";
                    }

                    //try to add the part to the part map
                    auto pos = m_pPartMap->find(partLabel);
                    if (pos != m_pPartMap->end())
                    {
                        //part label already in use
                        throw ExPartAlreadyDefined();
                    }
                    else
                    {
                        //add part to partmap
                        (*m_pPartMap)[partLabel] = std::make_shared<StackT>(m_stack);
                    }
                }
                else //if we are here a part block has been started but it never got closed
                {
                    throw ExMissingPartEnd();
                }
            }
        }


        ///parses the tokens on the stack
        void parseStack()
        {
            if ( !m_stack.empty())
            {
                if (m_stack.front() == TokenT::ePartBegin)
                {
                    parsePartBlock();
                    m_stack.clear();
                }
                else if ( parseTextOnly())
                {
                    outputText( m_stack, m_pOutputStream);
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
                    *m_pOutputStream << macro;
                }
            }
        }

        ///check is text only
        bool parseTextOnly()
        {
            for (const TokenT& token : m_stack)
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
            for (const TokenT& token : stack)
            {
                token.toStream( *output);
            }
        }

        //log the source text currently on the stack
        void logStackSourceText(const char* message)
        {
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << message << " (level " << m_level << "):\n";
                for (const TokenT& token : m_stack)
                {
                    token.sourceTextToStream(*m_pLogOutputStream);
                }
                *m_pLogOutputStream << "\n";
            }
        }

        ///start parsing a macro
        void parseMacro( MacroT& macro)
        {
            //log
            logStackSourceText("Found macro");

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
                if ( m_pLogOutputStream)
                {
                    *m_pLogOutputStream << "Successfully processed part of macro:\n";
                    for ( PosT it = m_stack.begin(); it != pos; ++it)
                    {
                        it->sourceTextToStream( *m_pLogOutputStream);
                    }
                    *m_pLogOutputStream << "\n";
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
                for (const StringT& text : textList)
                {
                    expression.add( text.begin(), text.end());
                }
            }
        }

    private:

        OutputStreamT* m_pOutputStream; ///<sink for macros, also excepts text around macros
        StackT m_stack;
        size_t m_currentMacroTextSize;
        size_t m_level; ///<used for logging purposes
        size_t m_partExpansionRecursionLevel; ///<user can use parts in a way that caused endless recursion
        static const size_t m_partExpansionRecursionLevelLimit = 32; ///<user can use parts in a way that caused endless recursion
        LogOutputStreamT* m_pLogOutputStream; ///<used for logging purposes; NULL if not logging
        PartMapT* m_pPartMap;
        static const size_t cMaxAllowedMacroSize = 2 * 1024 * 1024; ///<randomly chosen value for catching error conditions
    };
}
