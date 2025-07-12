// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <ostream>

#include "CombiKeywordParameterParser.gen.h"
#include "KeywordParameterCheckFunctions.h"
#include "cpptokenfinder.hpp"
#include "cppstringx.hpp"
#include "StringLiteral.h"
#include "CNul.h"


namespace code_creation_kit
{

    ///Splits text line input into tokens.
    template <
          typename TokenT
        , typename StringT
        , typename OutputStreamT
        , typename LogOutputStreamT = CNul >
    class CBackEndTokenizer
    {
    public:
        typedef CBackEndTokenizer<TokenT, StringT, OutputStreamT, LogOutputStreamT> ThisT;
        typedef std::vector<StringT> KeywordListT;
        typedef cppstringx::range<typename StringT::const_iterator> RangeT;
        typedef typename StringT::value_type CharT;
    private:
        typedef typename StringT::const_iterator IteratorT;
        typedef typename TokenT::SharedStringListT SharedStringListT;
        typedef typename TokenT::StringListT StringListT;
        typedef void (*ParseParameters)(IteratorT& start, const IteratorT& end, StringListT& parameters);
        typedef void (*CheckParameters)(const StringListT& container);
        class TokenData
        {
        public:
            TokenData(typename TokenT::ETokenT tokenId = TokenT::eInvalid, size_t parameterCount = 0, ParseParameters parseFunction = 0, CheckParameters checkParameters = 0)
                : TokenId(tokenId)
                , ParameterCount(parameterCount)
                , ParseFunction(parseFunction)
                , CheckFunction(checkParameters)
            {
            }
            
            typename TokenT::ETokenT TokenId;
            size_t ParameterCount;
            ParseParameters ParseFunction;
            CheckParameters CheckFunction;
        };
    public:

        CBackEndTokenizer()
            : m_outputStream(0)
            , m_closing(false)
            , m_bypassMode(false)
            , m_logOutputStream(0)
        {
        }


        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_logOutputStream = stream;
        }

        ///open
        void open()
        {
            m_closing = false;
            m_bypassMode = false;
        }

        ///open
        void close()
        {
            m_closing = true;
        }

        ///<used when limiting recursion level, forces text output with tick removal
        void setBypassMode( bool enable)
        {
            m_bypassMode = enable;
        }

        ///sets up regex search expression; 
        void setMarkup( const StringT& prefix, const StringT& postfix)
        {
            m_tokenFinder.clear();
            m_tokenData.clear();
            m_markupPostfix = postfix;
            
            m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
            m_commentDotKeyword = prefix + STRING_LITERAL("COMMENT.") + postfix;
            m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
            m_trimDotKeyword = prefix + STRING_LITERAL("TRIM.") + postfix;
            m_trimLeftKeyword = prefix + STRING_LITERAL("TRIM_LEFT") + postfix;
            m_trimLeftDotKeyword = prefix + STRING_LITERAL("TRIM_LEFT.") + postfix;
            m_trimRightKeyword = prefix + STRING_LITERAL("TRIM_RIGHT") + postfix;
            m_trimRightDotKeyword = prefix + STRING_LITERAL("TRIM_RIGHT.") + postfix;

            m_tokenFinder.add_token(prefix + STRING_LITERAL("COMMENT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eComment));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("INCLUDE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eInclude, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_MARKUP"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMarkup, 2, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrim));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM_LEFT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrimLeft));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM_RIGHT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrimRight));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePart, 0, KeywordParameterParser::getParametersCombi1Plain1CStyleOptional1UIntOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_BEGIN"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartBegin, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_END"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_LAZY"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartLazy, 0, KeywordParameterParser::getParametersCombi1Plain1CStyleOptional1UIntOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_PADDING"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartPadding));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_REMOVE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartRemove, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eSetRecursionLevelLimit));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT_OFF"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eSetRecursionLevelLimitOff));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_BEGIN"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableBegin, 0, KeywordParameterParser::getParametersCombi1Plain2CStyleOptional1PlainOptional1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_END"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_LOAD"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableLoad, 0, KeywordParameterParser::getParametersCombi2Plain2CStyleOptional1PlainOptional1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_REMOVE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableRemove, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ANY"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eAny));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("AS_VOLATILE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eVolatil));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("BEGIN"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eBegin));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("BLOCK_FORMAT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eBlockFormat, 1, KeywordParameterParser::getParameters<CUIntParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("CALC"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eCalc, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("CONTAINS"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eContains, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("COUNT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eCount));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("END"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ENDS_WITH"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEndsWith, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ENTRY"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEntry, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("EQUALS"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMatches, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ERROR"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eError_, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FIRST_TIME"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eFirstTime));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FLUSH"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eFlush));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FOR_ALL"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eForAll));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("HTML_ESCAPE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eHtmlEscape));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("IF"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIf_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("IGNORE_CASE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIgnoreCase));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("INDEX"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIndex));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("LAST_TIME"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eLastTime));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MACRO_BEGIN"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMacroBegin));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MACRO_END"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMacroEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MATCHES_REGEX"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eRegexMatches, 1, KeywordParameterParser::getParameters<CRegexParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MERGE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMerge, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("NOT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eNot_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("OR"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eOr_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PAD_LEFT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePadLeft, 0, KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional<IteratorT, StringListT>, checkPadParameters));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PAD_RIGHT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePadRight, 0, KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional<IteratorT, StringListT>, checkPadParameters));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("READ_LEFT_TO_RIGHT"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eLeftToRight));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("READ_TOP_DOWN"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTopDown));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("REGEX_REPLACE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eRegexReplace, 2, KeywordParameterParser::getParameters<CRegexParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("REPLACE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eReplace, 2, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("STARTS_WITH"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eStartsWith, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_CSTRING"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToCString));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_CSV"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToCsv, 0, KeywordParameterParser::getParametersCombi1CStyle1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_LOWER"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToLower));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_SIZE"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToSize, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_UPPER"), m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToUpper));
            m_tokenFinder.add_token(STRING_LITERAL("\r\n"), m_tokenData.size());
            m_tokenFinder.add_token(STRING_LITERAL("\n"), m_tokenData.size());
            m_tokenData.emplace_back(TokenT::eNewLine);
        }

        ///connect receiver of processed stream
        void connectOutputStream( OutputStreamT* stream)
        {
            m_outputStream = stream;
        }

        ///get the range of characters without surrounding white space
        template <typename PredicateT>
        RangeT trimRange( const StringT& line, PredicateT isSpace)
        {
            IteratorT start = line.begin();
            IteratorT end = line.end();
            while( start != end && isSpace( *start)) { ++start; }
            while( start != end && isSpace( *--end));
            if ( end != line.end()) ++end;
            return RangeT( start, end);
        }

        ///removes a delay mark if needed
        bool removeTick( IteratorT tokenBegin, IteratorT dotsBegin, IteratorT dotsEnd, IteratorT tokenEnd)
        {
            // if there is more than one delay mark, remove delay mark and output as text
            // one delay mark triggers processing
            // remove delay mark and output in bypass mode in any case
            if ( (dotsEnd - dotsBegin) > (m_bypassMode ? 0 : 1) )
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, tokenBegin, dotsBegin);
                *m_outputStream << TokenT( TokenT::eTextFragment, dotsBegin + 1, tokenEnd);
                return true;
            }
            else if ( m_bypassMode) // output in bypass mode
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, tokenBegin, tokenEnd);
                return true;
            }

            return false;
        }

        SharedStringListT getSourceText(
            IteratorT tokenBegin,
            IteratorT dotsBegin,
            IteratorT dotsEnd,
            IteratorT& endPos
            )
        {
            if ( (dotsEnd - dotsBegin) > 0 )
            {
                SharedStringListT list = std::make_shared<typename TokenT::StringListT>(2);
                list->front().assign( tokenBegin, dotsBegin);
                list->back().assign( dotsBegin + 1, endPos);
                return list;
            }
            SharedStringListT list = std::make_shared<typename TokenT::StringListT>(1);
            list->front().assign( tokenBegin, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            IteratorT textBegin = line.begin();
            IteratorT fullLineBegin = line.begin();
            IteratorT textEnd = line.end(); 
            IteratorT trimLeftTokenTrailingTextBegin = textEnd;
            IteratorT trimLeftTokenTrailingTextEnd = textEnd;

            //check if the line needs to be trimmed or is comment
            for (;!m_bypassMode;)
            {
                RangeT range = trimRange( line, cppstringx::utility::is_any_of<const char*>(" \t\n\r"));
                if ( cppstringx::starts_with( range, m_commentKeyword))
                {
                    return *this;
                }
                if ( cppstringx::starts_with( range, m_commentDotKeyword))
                {
                    return *this;
                }
                if ( cppstringx::ends_with( range, m_trimKeyword))
                {
                    size_t keywordSize = m_trimKeyword.size();
                    textBegin = range.begin();
                    textEnd = range.end() - keywordSize;
                    trimmedRight = true;
                    break;
                }
                if ( cppstringx::ends_with( range, m_trimDotKeyword))
                {
                    size_t keywordSize = m_trimDotKeyword.size();
                    textBegin = range.begin();
                    textEnd = range.end() - keywordSize;
                    trimmedRight = true;
                    break;
                }
                if ( cppstringx::ends_with( range, m_trimLeftKeyword))
                {
                    size_t keywordSize = m_trimLeftKeyword.size();
                    textBegin = range.begin();
                    textEnd = range.end() - keywordSize;
                    trimLeftTokenTrailingTextBegin = range.end();
                    break;
                }
                if ( cppstringx::ends_with( range, m_trimLeftDotKeyword))
                {
                    size_t keywordSize = m_trimLeftDotKeyword.size();
                    textBegin = range.begin();
                    textEnd = range.end() - keywordSize;
                    trimLeftTokenTrailingTextBegin = range.end();
                    break;
                }
                if ( cppstringx::ends_with( range, m_trimRightKeyword))
                {
                    size_t keywordSize = m_trimRightKeyword.size();
                    textEnd = range.end() - keywordSize;
                    trimmedRight = true;
                    break;
                }
                if ( cppstringx::ends_with( range, m_trimRightDotKeyword))
                {
                    size_t keywordSize = m_trimRightDotKeyword.size();
                    textEnd = range.end() - keywordSize;
                    trimmedRight = true;
                    break;
                }
                break;
            }

            IteratorT tokenBegin;
            IteratorT tokenEnd;
            size_t index = 0;

            while(m_tokenFinder.find_token(textBegin, textEnd, tokenBegin, tokenEnd, index)) 
            {
                const TokenData& tokenData = m_tokenData[index];
                
                //if full line without tags, output as special token
                if ( TokenT::eNewLine == tokenData.TokenId && textBegin == fullLineBegin )
                {
                    *m_outputStream << TokenT( TokenT::eFullLineWithoutTags, textBegin, tokenEnd);
                    textBegin = tokenEnd;
                    if ( tokenEnd == textEnd)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                // In back end we only found the first part of the tag, now we look for tick mark dots
                IteratorT dotsBegin = tokenEnd;
                IteratorT dotsEnd = tokenEnd;
                if (TokenT::eNewLine != tokenData.TokenId) //but not when new line
                {
                    for (;dotsBegin != textEnd;++dotsEnd)
                    {
                        if (*dotsEnd != STRING_LITERAL('.'))
                        {
                            break;
                        }
                    }
                    
                    // Check if the markup postfix is present
                    if (cppstringx::starts_with(RangeT(dotsEnd, textEnd), m_markupPostfix))
                    {
                        // Advance token end
                        tokenEnd = dotsEnd + m_markupPostfix.size();
                    }
                    else
                    {
                        // Token not complete, continue search with the next character
                        ++textBegin;
                        continue;
                    }
                }
                
                //forward preceding text as token, if not empty
                if ( textBegin != tokenBegin )
                {
                    *m_outputStream << TokenT( TokenT::eTextFragment, textBegin, tokenBegin);
                }

                //set textBegin position for next loop iteration
                textBegin = tokenEnd;

                //process tokens
                if ( TokenT::eNewLine == tokenData.TokenId )
                {
                    *m_outputStream << TokenT( TokenT::eNewLine, tokenBegin, tokenEnd);
                }
                else if ( tokenData.ParseFunction )
                {
                    if ( removeTick( tokenBegin, dotsBegin, dotsEnd, tokenEnd))
                    {
                        continue;
                    }
                    SharedStringListT list = std::make_shared<typename TokenT::StringListT>();
                    try
                    {
                        if (tokenData.ParameterCount)
                        {
                            list->resize(tokenData.ParameterCount);
                        }
                        tokenData.ParseFunction(textBegin, textEnd, *list);
                        if (tokenData.CheckFunction)
                        {
                            tokenData.CheckFunction(*list);
                        }
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineBegin,textBegin) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( tokenData.TokenId, list, getSourceText( tokenBegin, dotsBegin, dotsEnd, textBegin /*has been updated*/));
                    }
                    else
                    {
                        *m_outputStream << TokenT( tokenData.TokenId, list);
                    }
                }
                else
                {
                    if (tokenData.TokenId == TokenT::eSetRecursionLevelLimit)
                    {
                        // Turn limit off to make sure that the new limit gets processed
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);
                    }
                    if (tokenData.TokenId == TokenT::eSetRecursionLevelLimitOff)
                    {
                        // This tag is always processed. Delay dots have no effect.
                    }
                    else if ( removeTick( tokenBegin, dotsBegin, dotsEnd, tokenEnd))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( tokenData.TokenId, SharedStringListT(), getSourceText( tokenBegin, dotsBegin, dotsEnd, tokenEnd));
                    }
                    else
                    {
                        *m_outputStream << TokenT( tokenData.TokenId);
                    }
                }
            }

            if ( textBegin != textEnd)
            {
                //if full line without tags, output as special token used for optimizations, otherwise output text fragment
                *m_outputStream << TokenT( (textBegin == fullLineBegin && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, textBegin, textEnd);
            }
            if ( trimmedRight)
            {
                //a right trimmed line is treated as line macro
                *m_outputStream << TokenT( TokenT::eNewLine);
            }
            else if (trimLeftTokenTrailingTextBegin != trimLeftTokenTrailingTextEnd)
            {
                IteratorT trimLeftTokenTrailingTextNewLine = trimLeftTokenTrailingTextEnd - 1;
                //trailing text ends with new line?
                if (*trimLeftTokenTrailingTextNewLine == STRING_LITERAL('\n'))
                {
                    //only new line?
                    if (trimLeftTokenTrailingTextBegin != trimLeftTokenTrailingTextNewLine)
                    {
                        *m_outputStream << TokenT(TokenT::eTextFragment, trimLeftTokenTrailingTextBegin, trimLeftTokenTrailingTextNewLine);
                    }
                    *m_outputStream << TokenT(TokenT::eNewLine, trimLeftTokenTrailingTextNewLine, trimLeftTokenTrailingTextEnd);
                }
                else
                {
                    *m_outputStream << TokenT(TokenT::eNewLine, trimLeftTokenTrailingTextBegin, trimLeftTokenTrailingTextEnd);
                }
            }
            return *this;
        }
    private:
        bool isLoggingEnabled()
        {
            return m_logOutputStream != NULL;
        }
    private:
        std::vector<TokenData> m_tokenData;
        cpptokenfinder::token_finder<CharT, size_t, size_t, size_t(-1)> m_tokenFinder;
        OutputStreamT* m_outputStream; ///<sink for tokens
        bool m_closing;///<output line fragments as full line if closing to force flush
        bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal
        StringT m_markupPostfix;///<used when processing tick marks in back textEnd
        StringT m_commentKeyword; ///<used for special preprocessing action
        StringT m_commentDotKeyword; ///<used for special preprocessing action
        StringT m_trimKeyword; ///<used for special preprocessing action
        StringT m_trimDotKeyword; ///<used for special preprocessing action
        StringT m_trimLeftKeyword; ///<used for special preprocessing action
        StringT m_trimLeftDotKeyword; ///<used for special preprocessing action
        StringT m_trimRightKeyword; ///<used for special preprocessing action
        StringT m_trimRightDotKeyword; ///<used for special preprocessing action
        LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
