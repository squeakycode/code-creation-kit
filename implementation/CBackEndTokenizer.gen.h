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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <ostream>

#include "CombiKeywordParameterParser.gen.h"
#include "CSpecialRegexCharacterPrefixer.h"
#include "KeywordParameterCheckFunctions.h"

#if defined(CCK_USE_STD_REGEX)
#   include <regex>
    namespace regex_namespace = std;
#else
#   include <boost/regex.hpp>
    namespace regex_namespace = boost;
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#pragma warning( disable : 4996 )
#endif
#include <boost/algorithm/string.hpp>
#include <boost/range.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

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
        typedef regex_namespace::basic_regex<typename StringT::value_type, regex_namespace::regex_traits<typename StringT::value_type> > RegexT;
        typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
        typedef typename StringT::value_type CharT;

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
            m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
            m_commentDotKeyword = prefix + STRING_LITERAL("COMMENT.") + postfix;
            m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
            m_trimDotKeyword = prefix + STRING_LITERAL("TRIM.") + postfix;
            m_trimLeftKeyword = prefix + STRING_LITERAL("TRIM_LEFT") + postfix;
            m_trimLeftDotKeyword = prefix + STRING_LITERAL("TRIM_LEFT.") + postfix;
            m_trimRightKeyword = prefix + STRING_LITERAL("TRIM_RIGHT") + postfix;
            m_trimRightDotKeyword = prefix + STRING_LITERAL("TRIM_RIGHT.") + postfix;

            StringT regexPrefix = prefix;
            StringT regexPostfix = postfix;
            CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPrefix);
            CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPostfix);

            StringT expression;
            StringT front = StringT() + STRING_LITERAL("|") + STRING_LITERAL("(") + regexPrefix;
            StringT back = regexPostfix + STRING_LITERAL(")");
            back = StringT() + STRING_LITERAL("(\\.*)") + back;

            expression += STRING_LITERAL("(\r\n|\n)");
            expression += front + STRING_LITERAL("COMMENT") + back;
            expression += front + STRING_LITERAL("INCLUDE") + back;
            expression += front + STRING_LITERAL("SET_MARKUP") + back;
            expression += front + STRING_LITERAL("TRIM") + back;
            expression += front + STRING_LITERAL("TRIM_LEFT") + back;
            expression += front + STRING_LITERAL("TRIM_RIGHT") + back;
            expression += front + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT") + back;
            expression += front + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT_OFF") + back;
            expression += front + STRING_LITERAL("ANY") + back;
            expression += front + STRING_LITERAL("AS_VOLATILE") + back;
            expression += front + STRING_LITERAL("BEGIN") + back;
            expression += front + STRING_LITERAL("BLOCK_FORMAT") + back;
            expression += front + STRING_LITERAL("CONTAINS") + back;
            expression += front + STRING_LITERAL("COUNT") + back;
            expression += front + STRING_LITERAL("END") + back;
            expression += front + STRING_LITERAL("ENDS_WITH") + back;
            expression += front + STRING_LITERAL("ENTRY") + back;
            expression += front + STRING_LITERAL("EQUALS") + back;
            expression += front + STRING_LITERAL("ERROR") + back;
            expression += front + STRING_LITERAL("FIRST_TIME") + back;
            expression += front + STRING_LITERAL("FLUSH") + back;
            expression += front + STRING_LITERAL("FOR_ALL") + back;
            expression += front + STRING_LITERAL("HTML_ESCAPE") + back;
            expression += front + STRING_LITERAL("IF") + back;
            expression += front + STRING_LITERAL("IGNORE_CASE") + back;
            expression += front + STRING_LITERAL("INDEX") + back;
            expression += front + STRING_LITERAL("LAST_TIME") + back;
            expression += front + STRING_LITERAL("MACRO_BEGIN") + back;
            expression += front + STRING_LITERAL("MACRO_END") + back;
            expression += front + STRING_LITERAL("MATCHES_REGEX") + back;
            expression += front + STRING_LITERAL("MERGE") + back;
            expression += front + STRING_LITERAL("NOT") + back;
            expression += front + STRING_LITERAL("OR") + back;
            expression += front + STRING_LITERAL("PAD_LEFT") + back;
            expression += front + STRING_LITERAL("PAD_RIGHT") + back;
            expression += front + STRING_LITERAL("READ_LEFT_TO_RIGHT") + back;
            expression += front + STRING_LITERAL("READ_TOP_DOWN") + back;
            expression += front + STRING_LITERAL("REGEX_REPLACE") + back;
            expression += front + STRING_LITERAL("REPLACE") + back;
            expression += front + STRING_LITERAL("STARTS_WITH") + back;
            expression += front + STRING_LITERAL("TO_CSTRING") + back;
            expression += front + STRING_LITERAL("TO_LOWER") + back;
            expression += front + STRING_LITERAL("TO_UPPER") + back;

            m_searchExpression = RegexT( expression);
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
            typename StringT::const_iterator start = line.begin();
            typename StringT::const_iterator end = line.end();
            while( start != end && isSpace( *start)) { ++start; }
            while( start != end && isSpace( *--end));
            if ( end != line.end()) ++end;
            return RangeT( start, end);
        }

        ///removes a delay mark if needed
        template <typename WhatT>
        bool removeTick( WhatT& what, int pos)
        {
            // if there is more than one delay mark, remove delay mark and output as text
            // one delay mark triggers processing
            // remove delay mark and output in bypass mode in any case
            if ( (what[ pos ].second - what[ pos ].first) > (m_bypassMode ? 0 : 1) )
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, what[ pos - 1 ].first, what[ pos ].first);
                *m_outputStream << TokenT( TokenT::eTextFragment, what[ pos ].first + 1, what[ pos - 1 ].second);
                return true;
            }
            else if ( m_bypassMode) // remove delay mark and output in bypass mode in any case
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, what[ pos - 1 ].first, what[ pos - 1 ].second);
                return true;
            }

            return false;
        }

        template <typename WhatT>
        typename TokenT::SharedStringListT getSourceText( WhatT& what, int pos, typename StringT::const_iterator& endPos)
        {
            pos = pos * 2 - 1;
            if ( (what[ pos ].second - what[ pos ].first) > 0 )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT(2));
                list->front().assign( what[ pos - 1 ].first, what[ pos ].first);
                list->back().assign( what[ pos ].first + 1, endPos);
                return list;
            }
            --pos;
            typename TokenT::SharedStringListT list( new typename TokenT::StringListT(1));
            list->front().assign( what[ pos ].first, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            regex_namespace::match_results<typename StringT::const_iterator> what;
            typename StringT::const_iterator start = line.begin();
            typename StringT::const_iterator fullLineStart = line.begin();
            typename StringT::const_iterator end = line.end(); 
            typename StringT::const_iterator trimLeftTokenTrailingTextBegin = end;
            typename StringT::const_iterator trimLeftTokenTrailingTextEnd = end;

            //check if the line needs to be trimmed or is comment
            for (;!m_bypassMode;)
            {
                RangeT range = trimRange( line, boost::is_any_of(" \t\n\r"));
                if ( boost::starts_with( range, m_commentKeyword))
                {
                    return *this;
                }
                if ( boost::starts_with( range, m_commentDotKeyword))
                {
                    return *this;
                }
                if ( boost::ends_with( range, m_trimKeyword))
                {
                    size_t keywordSize = m_trimKeyword.size();
                    start = range.begin();
                end = range.end() - keywordSize;
                trimmedRight = true;
                break;
                }
                if ( boost::ends_with( range, m_trimDotKeyword))
                {
                    size_t keywordSize = m_trimDotKeyword.size();
                    start = range.begin();
                end = range.end() - keywordSize;
                trimmedRight = true;
                break;
                }
                if ( boost::ends_with( range, m_trimLeftKeyword))
                {
                    size_t keywordSize = m_trimLeftKeyword.size();
                    start = range.begin();
                end = range.end() - keywordSize;
                trimLeftTokenTrailingTextBegin = range.end();
                break;
                }
                if ( boost::ends_with( range, m_trimLeftDotKeyword))
                {
                    size_t keywordSize = m_trimLeftDotKeyword.size();
                    start = range.begin();
                end = range.end() - keywordSize;
                trimLeftTokenTrailingTextBegin = range.end();
                break;
                }
                if ( boost::ends_with( range, m_trimRightKeyword))
                {
                    size_t keywordSize = m_trimRightKeyword.size();
                    end = range.end() - keywordSize;
                trimmedRight = true;
                break;
                }
                if ( boost::ends_with( range, m_trimRightDotKeyword))
                {
                    size_t keywordSize = m_trimRightDotKeyword.size();
                    end = range.end() - keywordSize;
                trimmedRight = true;
                break;
                }
                break;
            }

            while( regex_search(start, end, what, m_searchExpression)) 
            {
                //if full line without tags, output as special token
                if ( what[ TokenT::eNewLine ].matched && start == fullLineStart )
                {
                    *m_outputStream << TokenT( TokenT::eFullLineWithoutTags, start, what[ TokenT::eNewLine ].second);
                    start = what[ 0 ].second;
                    if ( what[ TokenT::eNewLine ].second == end)
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

                //forward preceding text as token, if not empty
                if ( start != what[ 0 ].first )
                {
                    *m_outputStream << TokenT( TokenT::eTextFragment, start, what[ 0 ].first);
                }

                //set start position for next loop iteration
                start = what[ 0 ].second;

                //process tokens
                if ( what[ TokenT::eNewLine ].matched )
                {
                    *m_outputStream << TokenT( TokenT::eNewLine, what[ TokenT::eNewLine ].first, what[ TokenT::eNewLine ].second);
                }
                else if ( what[ (TokenT::eComment-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eComment * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eComment, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eComment, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eComment);
                    }
                }
                else if ( what[ (TokenT::eInclude-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eInclude * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eInclude, list, getSourceText( what, TokenT::eInclude, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eInclude, list);
                    }
                }
                else if ( what[ (TokenT::eMarkup-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eMarkup * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(2);
                        KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMarkup, list, getSourceText( what, TokenT::eMarkup, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMarkup, list);
                    }
                }
                else if ( what[ (TokenT::eTrim-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eTrim * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrim, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrim, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrim);
                    }
                }
                else if ( what[ (TokenT::eTrimLeft-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eTrimLeft * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrimLeft, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrimLeft, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrimLeft);
                    }
                }
                else if ( what[ (TokenT::eTrimRight-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eTrimRight * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrimRight, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrimRight, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrimRight);
                    }
                }
                else if ( what[ (TokenT::eSetRecursionLevelLimit-1)*2 ].matched )
                {
                    //turn limit off to make sure that the new limit gets processed
                    *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);

                    if ( removeTick( what, (TokenT::eSetRecursionLevelLimit * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eSetRecursionLevelLimit, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit);
                    }
                }
                else if ( what[ (TokenT::eSetRecursionLevelLimitOff-1)*2 ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eSetRecursionLevelLimitOff, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);
                    }
                }
                else if ( what[ (TokenT::eAny-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eAny * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eAny, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eAny, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eAny);
                    }
                }
                else if ( what[ (TokenT::eVolatil-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eVolatil * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eVolatil, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eVolatil, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eVolatil);
                    }
                }
                else if ( what[ (TokenT::eBegin-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eBegin * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eBegin, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eBegin, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eBegin);
                    }
                }
                else if ( what[ (TokenT::eBlockFormat-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eBlockFormat * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CUIntParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eBlockFormat, list, getSourceText( what, TokenT::eBlockFormat, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eBlockFormat, list);
                    }
                }
                else if ( what[ (TokenT::eContains-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eContains * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eContains, list, getSourceText( what, TokenT::eContains, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eContains, list);
                    }
                }
                else if ( what[ (TokenT::eCount-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eCount * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eCount, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eCount, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eCount);
                    }
                }
                else if ( what[ (TokenT::eEnd-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eEnd * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eEnd, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eEnd, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eEnd);
                    }
                }
                else if ( what[ (TokenT::eEndsWith-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eEndsWith * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eEndsWith, list, getSourceText( what, TokenT::eEndsWith, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eEndsWith, list);
                    }
                }
                else if ( what[ (TokenT::eEntry-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eEntry * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eEntry, list, getSourceText( what, TokenT::eEntry, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eEntry, list);
                    }
                }
                else if ( what[ (TokenT::eMatches-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eMatches * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMatches, list, getSourceText( what, TokenT::eMatches, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMatches, list);
                    }
                }
                else if ( what[ (TokenT::eError_-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eError_ * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eError_, list, getSourceText( what, TokenT::eError_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eError_, list);
                    }
                }
                else if ( what[ (TokenT::eFirstTime-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eFirstTime * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eFirstTime, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eFirstTime, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eFirstTime);
                    }
                }
                else if ( what[ (TokenT::eFlush-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eFlush * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eFlush, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eFlush, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eFlush);
                    }
                }
                else if ( what[ (TokenT::eForAll-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eForAll * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eForAll, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eForAll, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eForAll);
                    }
                }
                else if ( what[ (TokenT::eHtmlEscape-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eHtmlEscape * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eHtmlEscape, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eHtmlEscape, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eHtmlEscape);
                    }
                }
                else if ( what[ (TokenT::eIf_-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eIf_ * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIf_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIf_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIf_);
                    }
                }
                else if ( what[ (TokenT::eIgnoreCase-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eIgnoreCase * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIgnoreCase, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIgnoreCase, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIgnoreCase);
                    }
                }
                else if ( what[ (TokenT::eIndex-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eIndex * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIndex, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIndex, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIndex);
                    }
                }
                else if ( what[ (TokenT::eLastTime-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eLastTime * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eLastTime, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eLastTime, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eLastTime);
                    }
                }
                else if ( what[ (TokenT::eMacroBegin-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eMacroBegin * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMacroBegin, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eMacroBegin, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMacroBegin);
                    }
                }
                else if ( what[ (TokenT::eMacroEnd-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eMacroEnd * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMacroEnd, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eMacroEnd, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMacroEnd);
                    }
                }
                else if ( what[ (TokenT::eRegexMatches-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eRegexMatches * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eRegexMatches, list, getSourceText( what, TokenT::eRegexMatches, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eRegexMatches, list);
                    }
                }
                else if ( what[ (TokenT::eMerge-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eMerge * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMerge, list, getSourceText( what, TokenT::eMerge, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMerge, list);
                    }
                }
                else if ( what[ (TokenT::eNot_-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eNot_ * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eNot_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eNot_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eNot_);
                    }
                }
                else if ( what[ (TokenT::eOr_-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eOr_ * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eOr_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eOr_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eOr_);
                    }
                }
                else if ( what[ (TokenT::ePadLeft-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::ePadLeft * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        KeywordParameterParser::getParametersCombiCStyleUIntUIntRepeat(start, end, *list);
                        checkPadParameters(*list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::ePadLeft, list, getSourceText( what, TokenT::ePadLeft, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::ePadLeft, list);
                    }
                }
                else if ( what[ (TokenT::ePadRight-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::ePadRight * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        KeywordParameterParser::getParametersCombiCStyleUIntUIntRepeat(start, end, *list);
                        checkPadParameters(*list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::ePadRight, list, getSourceText( what, TokenT::ePadRight, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::ePadRight, list);
                    }
                }
                else if ( what[ (TokenT::eLeftToRight-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eLeftToRight * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eLeftToRight, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eLeftToRight, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eLeftToRight);
                    }
                }
                else if ( what[ (TokenT::eTopDown-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eTopDown * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTopDown, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTopDown, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTopDown);
                    }
                }
                else if ( what[ (TokenT::eRegexReplace-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eRegexReplace * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(2);
                        KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eRegexReplace, list, getSourceText( what, TokenT::eRegexReplace, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eRegexReplace, list);
                    }
                }
                else if ( what[ (TokenT::eReplace-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eReplace * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(2);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eReplace, list, getSourceText( what, TokenT::eReplace, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eReplace, list);
                    }
                }
                else if ( what[ (TokenT::eStartsWith-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eStartsWith * 2) - 1))
                    {
                        continue;
                    }
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                        list->resize(1);
                        KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                    }
                    catch(...)
                    {
                        //log
                        if ( isLoggingEnabled())
                        {
                            *m_logOutputStream << "Error parsing parameters in line:\n";
                            *m_logOutputStream << line;
                            *m_logOutputStream << StringT(fullLineStart,start) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eStartsWith, list, getSourceText( what, TokenT::eStartsWith, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eStartsWith, list);
                    }
                }
                else if ( what[ (TokenT::eToCString-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eToCString * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eToCString, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eToCString, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eToCString);
                    }
                }
                else if ( what[ (TokenT::eToLower-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eToLower * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eToLower, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eToLower, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eToLower);
                    }
                }
                else if ( what[ (TokenT::eToUpper-1)*2 ].matched )
                {
                    if ( removeTick( what, (TokenT::eToUpper * 2) - 1))
                    {
                        continue;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eToUpper, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eToUpper, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eToUpper);
                    }
                }
                else
                {
                    throw std::runtime_error( "Unexpected match found in tokenizer.");
                }
            }

            if ( start != end)
            {
                //if full line without tags, output as special token used for optimizations, otherwise ouput text fragment
                *m_outputStream << TokenT( (start == fullLineStart && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, start, end);
            }
            if ( trimmedRight)
            {
                //a right trimmed line is treated as line macro
                *m_outputStream << TokenT( TokenT::eNewLine);
            }
            else if (trimLeftTokenTrailingTextBegin != trimLeftTokenTrailingTextEnd)
            {
                typename StringT::const_iterator trimLeftTokenTrailingTextNewLine = trimLeftTokenTrailingTextEnd - 1;
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
        RegexT m_searchExpression; ///<used for finding keywords and new line
        OutputStreamT* m_outputStream; ///<sink for tokens
        bool m_closing;///<output line fragments as full line if closing to force flush
        bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal
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
