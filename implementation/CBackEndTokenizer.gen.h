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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#ifndef INCLUDED_CBACKENDTOKENIZER_TPL_H_6955377
#define INCLUDED_CBACKENDTOKENIZER_TPL_H_6955377

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "KeywordParameterParser.h"
#include "CSpecialRegexCharacterPrefixer.h"

#include <boost/regex.hpp> 
#include <boost/foreach.hpp>

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

///Removes one tick from keywords in input
template <typename TokenT, typename StringT, typename OutputStreamT>
class CBackEndTokenizer
{
public:
    typedef std::vector<StringT> KeywordListT;
    typedef boost::basic_regex<typename StringT::value_type, boost::regex_traits<typename StringT::value_type> > RegexT;
    typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
    typedef typename StringT::value_type CharT;

    CBackEndTokenizer()
        : m_outputStream(0)
        , m_closing(false)
        , m_bypassMode(false)
    {
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
        m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
        m_commentDotKeyword = prefix + STRING_LITERAL("COMMENT.") + postfix;
        m_trimDotKeyword = prefix + STRING_LITERAL("TRIM.") + postfix;

        StringT regexPrefix = prefix;
        StringT regexPostfix = postfix;
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPrefix);
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPostfix);

        StringT expression;
        StringT front = StringT() + STRING_LITERAL("|") + STRING_LITERAL("(") + regexPrefix;
        StringT back = regexPostfix + STRING_LITERAL(")");
        back = StringT() + STRING_LITERAL("(\\.*)") + back;

        expression += STRING_LITERAL("(\n)");
        expression += front + STRING_LITERAL("COMMENT") + back;
        expression += front + STRING_LITERAL("INCLUDE") + back;
        expression += front + STRING_LITERAL("SET_MARKUP") + back;
        expression += front + STRING_LITERAL("TRIM") + back;
        expression += front + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT") + back;
        expression += front + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT_OFF") + back;
        expression += front + STRING_LITERAL("ANY") + back;
        expression += front + STRING_LITERAL("AS_VOLATILE") + back;
        expression += front + STRING_LITERAL("BEGIN") + back;
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
    bool RemoveTick( WhatT& what, int pos)
    {
        if ( (what[ pos ].second - what[ pos ].first) > (m_bypassMode ? 0 : 1) )
        {
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( what[ pos - 1 ].first, what[ pos ].first));
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( what[ pos ].first + 1, what[ pos - 1 ].second));
            return true;
        }
        else if ( m_bypassMode)
        {
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( what[ pos - 1 ].first, what[ pos - 1 ].second));
            return true;
        }

        return false;
    }

    ///tokenize input line
    CBackEndTokenizer<TokenT, StringT, OutputStreamT>& operator <<( const StringT& line)
    {
        bool trimmed = false;
        boost::match_results<typename StringT::const_iterator> what; 
        typename StringT::const_iterator start = line.begin();
        typename StringT::const_iterator fullLineStart = line.begin();
        typename StringT::const_iterator end = line.end(); 

        //check if the line needs to be trimmed or is comment
        if ( !m_bypassMode)
        {
            RangeT range = trimRange( line, boost::is_any_of(" \t\n"));
            if ( !m_commentKeyword.empty() )
            {
                if ( boost::starts_with( range, m_commentKeyword) || boost::starts_with( range, m_commentDotKeyword)) //is comment, drop line
                {
                    return *this;
                }
                if ( boost::ends_with( range, m_trimKeyword)) //trim keyword, trim line
                {
                    start = range.begin();
                    end = range.end() - m_trimKeyword.size();
                    trimmed = true;
                }
                else if ( boost::ends_with( range, m_trimDotKeyword)) //trim keyword, trim line
                {
                    start = range.begin();
                    end = range.end() - m_trimDotKeyword.size();
                    trimmed = true;
                }
            }
        }

        while( regex_search(start, end, what, m_searchExpression)) 
        {
            //if full line without tags, output as special token
            if ( what[ TokenT::eNewLine ].matched && start == fullLineStart )
            {
                *m_outputStream << TokenT( TokenT::eFullLineWithoutTags, StringT( start, what[ TokenT::eNewLine ].second));
                start = what[ 0 ].second;
                continue;
            }

            //forward preceding text as token, if not empty
            if ( start != what[ 0 ].first )
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, StringT( start, what[ 0 ].first));
            }

            //set start position for next loop iteration
            start = what[ 0 ].second;

            //process tokens
            if ( what[ TokenT::eNewLine ].matched )
            {
                *m_outputStream << TokenT( TokenT::eNewLine, StringT( what[ TokenT::eNewLine ].first, what[ TokenT::eNewLine ].second));
            }
            else if ( what[ (TokenT::eComment-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eComment * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eComment);
            }
            else if ( what[ (TokenT::eInclude-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eInclude * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eInclude, list);
            }
            else if ( what[ (TokenT::eMarkup-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eMarkup * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMarkup, list);
            }
            else if ( what[ (TokenT::eTrim-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eTrim * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eTrim);
            }
            else if ( what[ (TokenT::eSetRecursionLevelLimit-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eSetRecursionLevelLimit * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit);
            }
            else if ( what[ (TokenT::eSetRecursionLevelLimitOff-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eSetRecursionLevelLimitOff * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);
            }
            else if ( what[ (TokenT::eAny-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eAny * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eAny);
            }
            else if ( what[ (TokenT::eVolatil-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eVolatil * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eVolatil);
            }
            else if ( what[ (TokenT::eBegin-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eBegin * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eBegin);
            }
            else if ( what[ (TokenT::eContains-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eContains * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eContains, list);
            }
            else if ( what[ (TokenT::eCount-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eCount * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eCount);
            }
            else if ( what[ (TokenT::eEnd-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eEnd * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eEnd);
            }
            else if ( what[ (TokenT::eEndsWith-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eEndsWith * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eEndsWith, list);
            }
            else if ( what[ (TokenT::eEntry-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eEntry * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eEntry, list);
            }
            else if ( what[ (TokenT::eMatches-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eMatches * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMatches, list);
            }
            else if ( what[ (TokenT::eError_-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eError_ * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eError_, list);
            }
            else if ( what[ (TokenT::eFirstTime-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eFirstTime * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eFirstTime);
            }
            else if ( what[ (TokenT::eFlush-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eFlush * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eFlush);
            }
            else if ( what[ (TokenT::eForAll-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eForAll * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eForAll);
            }
            else if ( what[ (TokenT::eIf_-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eIf_ * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eIf_);
            }
            else if ( what[ (TokenT::eIgnoreCase-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eIgnoreCase * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eIgnoreCase);
            }
            else if ( what[ (TokenT::eIndex-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eIndex * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eIndex);
            }
            else if ( what[ (TokenT::eLastTime-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eLastTime * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eLastTime);
            }
            else if ( what[ (TokenT::eMacroBegin-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eMacroBegin * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eMacroBegin);
            }
            else if ( what[ (TokenT::eMacroEnd-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eMacroEnd * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eMacroEnd);
            }
            else if ( what[ (TokenT::eRegexMatches-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eRegexMatches * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eRegexMatches, list);
            }
            else if ( what[ (TokenT::eMerge-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eMerge * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMerge, list);
            }
            else if ( what[ (TokenT::eNot_-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eNot_ * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eNot_);
            }
            else if ( what[ (TokenT::eOr_-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eOr_ * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eOr_);
            }
            else if ( what[ (TokenT::eLeftToRight-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eLeftToRight * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eLeftToRight);
            }
            else if ( what[ (TokenT::eTopDown-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eTopDown * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eTopDown);
            }
            else if ( what[ (TokenT::eRegexReplace-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eRegexReplace * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eRegexReplace, list);
            }
            else if ( what[ (TokenT::eReplace-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eReplace * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eReplace, list);
            }
            else if ( what[ (TokenT::eStartsWith-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eStartsWith * 2) - 1))
                {
                    continue;
                }
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eStartsWith, list);
            }
            else if ( what[ (TokenT::eToCString-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eToCString * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eToCString);
            }
            else if ( what[ (TokenT::eToLower-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eToLower * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eToLower);
            }
            else if ( what[ (TokenT::eToUpper-1)*2 ].matched )
            {
                if ( RemoveTick( what, (TokenT::eToUpper * 2) - 1))
                {
                    continue;
                }
                *m_outputStream << TokenT( TokenT::eToUpper);
            }
            else
            {
                throw std::runtime_error( "Unexpected match found in tokenizer.");
            }
        }

        if ( start != end)
        {
            //if full line without tags, output as special token used for optimizations, otherwise ouput text fragment
            *m_outputStream << TokenT( (start == fullLineStart && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, StringT( start, end));
        }
        if ( trimmed)
        {
            //a trimmed line is treated as line macro
            *m_outputStream << TokenT( TokenT::eNewLine);
        }
        return *this;
    }
private:
    RegexT m_searchExpression; ///<used for finding keywords and new line
    OutputStreamT* m_outputStream; ///<sink for tokens
    bool m_closing;///<output line fragments as full line if closing to force flush
    bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal
    StringT m_trimKeyword; ///keyword for trimming lines
    StringT m_commentKeyword; ///keyword for comment lines
    StringT m_trimDotKeyword; ///keyword for trimming lines
    StringT m_commentDotKeyword; ///keyword for comment lines
};

#endif /* INCLUDED_CBACKENDTOKENIZER_TPL_H_6955377 */
