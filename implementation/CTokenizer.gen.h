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

#ifndef INCLUDED_CTOKENIZER_TPL_H_6955377
#define INCLUDED_CTOKENIZER_TPL_H_6955377

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
class CTokenizer
{
public:
    typedef std::vector<StringT> KeywordListT;
    typedef boost::basic_regex<typename StringT::value_type, boost::regex_traits<typename StringT::value_type> > RegexT;
    typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
    typedef typename StringT::value_type CharT;

    CTokenizer()
        : m_outputStream(0)
    {
    }

    ///sets up regex search expression; 
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
        m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;

        StringT regexPrefix = prefix;
        StringT regexPostfix = postfix;
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPrefix);
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPostfix);

        StringT expression;
        StringT front = StringT() + STRING_LITERAL("|") + STRING_LITERAL("(") + regexPrefix;
        StringT back = regexPostfix + STRING_LITERAL(")");

        expression += STRING_LITERAL("(\n)");
        expression += front + STRING_LITERAL("COMMENT") + back;
        expression += front + STRING_LITERAL("INCLUDE") + back;
        expression += front + STRING_LITERAL("SET_MARKUP") + back;
        expression += front + STRING_LITERAL("TRIM") + back;
        expression += front + STRING_LITERAL("ANY") + back;
        expression += front + STRING_LITERAL("AS_VOLATILE") + back;
        expression += front + STRING_LITERAL("BEGIN") + back;
        expression += front + STRING_LITERAL("CONTAINS") + back;
        expression += front + STRING_LITERAL("COUNT") + back;
        expression += front + STRING_LITERAL("END") + back;
        expression += front + STRING_LITERAL("ENDS_WITH") + back;
        expression += front + STRING_LITERAL("ENTRY") + back;
        expression += front + STRING_LITERAL("EQUALS") + back;
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
        if ( (what[ pos ].second - what[ pos ].first) > 1 )
        {
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( what[ pos - 1 ].first, what[ pos ].first));
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( what[ pos ].first + 1, what[ pos - 1 ].second));
            return true;                        
        }
        return false;
    }

    ///tokenize input line
    CTokenizer<TokenT, StringT, OutputStreamT>& operator <<( const StringT& line)
    {
        boost::match_results<typename StringT::const_iterator> what; 
        typename StringT::const_iterator start = line.begin();
        typename StringT::const_iterator fullLineStart = line.begin();
        typename StringT::const_iterator end = line.end(); 

        //check if the line needs to be trimmed or is comment
        {
            RangeT range = trimRange( line, boost::is_any_of(" \t\n"));
            if ( !m_commentKeyword.empty() &&  boost::starts_with( range, m_commentKeyword)) //is comment, drop line
            {
                return *this;
            }
            if ( !m_trimKeyword.empty() && boost::ends_with( range, m_trimKeyword)) //trim keyword, trim line
            {
                start = range.begin();
                end = range.end() - m_trimKeyword.size();
            }
        }

        while( regex_search(start, end, what, m_searchExpression)) 
        {

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
            else if ( what[ (TokenT::eComment) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eComment);
            }
            else if ( what[ (TokenT::eInclude) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eInclude, list);
            }
            else if ( what[ (TokenT::eMarkup) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CPlainParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMarkup, list);
            }
            else if ( what[ (TokenT::eTrim) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eTrim);
            }
            else if ( what[ (TokenT::eAny) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eAny);
            }
            else if ( what[ (TokenT::eVolatil) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eVolatil);
            }
            else if ( what[ (TokenT::eBegin) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eBegin);
            }
            else if ( what[ (TokenT::eContains) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eContains, list);
            }
            else if ( what[ (TokenT::eCount) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eCount);
            }
            else if ( what[ (TokenT::eEnd) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eEnd);
            }
            else if ( what[ (TokenT::eEndsWith) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eEndsWith, list);
            }
            else if ( what[ (TokenT::eEntry) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eEntry, list);
            }
            else if ( what[ (TokenT::eMatches) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMatches, list);
            }
            else if ( what[ (TokenT::eFirstTime) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eFirstTime);
            }
            else if ( what[ (TokenT::eFlush) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eFlush);
            }
            else if ( what[ (TokenT::eForAll) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eForAll);
            }
            else if ( what[ (TokenT::eIf_) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eIf_);
            }
            else if ( what[ (TokenT::eIgnoreCase) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eIgnoreCase);
            }
            else if ( what[ (TokenT::eIndex) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eIndex);
            }
            else if ( what[ (TokenT::eLastTime) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eLastTime);
            }
            else if ( what[ (TokenT::eMacroBegin) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eMacroBegin);
            }
            else if ( what[ (TokenT::eMacroEnd) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eMacroEnd);
            }
            else if ( what[ (TokenT::eRegexMatches) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eRegexMatches, list);
            }
            else if ( what[ (TokenT::eMerge) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eMerge, list);
            }
            else if ( what[ (TokenT::eNot_) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eNot_);
            }
            else if ( what[ (TokenT::eOr_) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eOr_);
            }
            else if ( what[ (TokenT::eLeftToRight) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eLeftToRight);
            }
            else if ( what[ (TokenT::eTopDown) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eTopDown);
            }
            else if ( what[ (TokenT::eRegexReplace) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CRegexParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eRegexReplace, list);
            }
            else if ( what[ (TokenT::eReplace) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 2);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eReplace, list);
            }
            else if ( what[ (TokenT::eStartsWith) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eStartsWith, list);
            }
            else if ( what[ (TokenT::eToCString) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eToCString);
            }
            else if ( what[ (TokenT::eToLower) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eToLower);
            }
            else if ( what[ (TokenT::eToUpper) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eToUpper);
            }
            else
            {
                throw std::runtime_error( "Unexpected match found in tokenizer.");
            }
        }

        if ( start != end)
        {
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( start, end));
        }
        return *this;
    }
private:
    RegexT m_searchExpression; ///<used for finding keywords and new line
    OutputStreamT* m_outputStream; ///<sink for tokens
    StringT m_trimKeyword; ///keyword for trimming lines
    StringT m_commentKeyword; ///keyword for comment lines    
};

#endif /* INCLUDED_CTOKENIZER_TPL_H_6955377 */
