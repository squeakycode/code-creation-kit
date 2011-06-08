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

#ifndef INCLUDED_[ENTRY]["Tokenizer"][TO_UPPER]_TPL_H_6955377
#define INCLUDED_[ENTRY]["Tokenizer"][TO_UPPER]_TPL_H_6955377

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
class [ENTRY]["Tokenizer"]
{
public:
    typedef std::vector<StringT> KeywordListT;
    typedef boost::basic_regex<typename StringT::value_type, boost::regex_traits<typename StringT::value_type> > RegexT;
    typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
    typedef typename StringT::value_type CharT;

    [ENTRY]["Tokenizer"]()
        : m_outputStream(0)
        , m_closing(false)[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
        , m_bypassMode(false)[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
    {
    }

    [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][TRIM]
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

    [MACRO_END][TRIM]
    ///sets up regex search expression; 
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        [MACRO_BEGIN][IF][ENTRY]["Tokenizer Preprocessor Action"][TRIM]
        m_[ENTRY]["Tag Name Small"]Keyword = prefix + STRING_LITERAL("[ENTRY]["Tag Name"]") + postfix;
        m_[ENTRY]["Tag Name Small"]DotKeyword = prefix + STRING_LITERAL("[ENTRY]["Tag Name"].") + postfix;[IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]
        [MACRO_END][TRIM]

        StringT regexPrefix = prefix;
        StringT regexPostfix = postfix;
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPrefix);
        CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPostfix);

        StringT expression;
        StringT front = StringT() + STRING_LITERAL("|") + STRING_LITERAL("(") + regexPrefix;
        StringT back = regexPostfix + STRING_LITERAL(")");
        back = StringT() + STRING_LITERAL("(\\.*)") + back;[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]

        expression += STRING_LITERAL("(\n)");
        expression += front + STRING_LITERAL("[ENTRY]["Tag Name"][READ_TOP_DOWN]") + back;

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

    [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][TRIM]
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

    [MACRO_END][TRIM]
    ///tokenize input line
    [ENTRY]["Tokenizer"]<TokenT, StringT, OutputStreamT>& operator <<( const StringT& line)
    {
        bool trimmed = false;
        boost::match_results<typename StringT::const_iterator> what; 
        typename StringT::const_iterator start = line.begin();
        typename StringT::const_iterator fullLineStart = line.begin();
        typename StringT::const_iterator end = line.end(); 

        //check if the line needs to be trimmed or is comment
        for (;[MACRO_BEGIN]!m_bypassMode[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][MACRO_END];)
        {
            RangeT range = trimRange( line, boost::is_any_of(" \t\n"));
            [MACRO_BEGIN][TRIM]
            if ( boost::[ENTRY]["Tokenizer Preprocessor Check"]( range, m_[ENTRY]["Tag Name Small"]Keyword))
            {
                [BEGIN][TRIM]
                size_t keywordSize = m_[ENTRY]["Tag Name Small"][STARTS_WITH]["trim"]Keyword.size();
                [OR][END][TRIM]
                [ENTRY]["Tokenizer Preprocessor Action"][REPLACE]["\n","\n                "]
            }
            [MACRO_BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"][TRIM.]
            if ( boost::[ENTRY]["Tokenizer Preprocessor Check"]( range, m_[ENTRY]["Tag Name Small"]DotKeyword))
            {
                [BEGIN][TRIM]
                size_t keywordSize = m_[ENTRY]["Tag Name Small"][STARTS_WITH]["trim"]DotKeyword.size();
                [OR][END][TRIM]
                [ENTRY]["Tokenizer Preprocessor Action"][REPLACE]["\n","\n                "]
            }
            [MACRO_END.][TRIM.]
            [MACRO_END][TRIM]
            break;
        }

        while( regex_search(start, end, what, m_searchExpression)) 
        {
            [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][TRIM]
            //if full line without tags, output as special token
            if ( what[ TokenT::eNewLine ].matched && start == fullLineStart )
            {
                *m_outputStream << TokenT( TokenT::eFullLineWithoutTags, StringT( start, what[ TokenT::eNewLine ].second));
                start = what[ 0 ].second;
                continue;
            }
            [MACRO_END][TRIM]

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
            [MACRO_BEGIN][TRIM]
            else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
            {
                [MACRO_BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"][TRIM]
                if ( RemoveTick( what, (TokenT::e[ENTRY]["Tag Name Capital"] * 2) - 1))
                {
                    continue;
                }
                [MACRO_END.][TRIM]
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( [ENTRY]["Parameter Count"]);
                KeywordParameterParser::getParameters<C[ENTRY]["Parameter Format"]ParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"], list);
            }
            [OR][IF][ENTRY]["Tag Name Capital"][EQUALS]["SetRecursionLevelLimitOff"][TRIM]
            else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
            {
                *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"]);
            }
            [OR][TRIM]
            else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
            {
                [MACRO_BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"][TRIM]
                [BEGIN][IF][ENTRY]["Tag Name Capital"][EQUALS]["SetRecursionLevelLimit"][TRIM]
                //turn limit off to make sure that the new limit gets processed
                *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);

                [OR][END][TRIM]
                if ( RemoveTick( what, (TokenT::e[ENTRY]["Tag Name Capital"] * 2) - 1))
                {
                    continue;
                }
                [MACRO_END.][TRIM]
                *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"]);
            }
            [MACRO_END][TRIM]
            else
            {
                throw std::runtime_error( "Unexpected match found in tokenizer.");
            }
        }

        if ( start != end)
        {
            [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][TRIM]
            //if full line without tags, output as special token used for optimizations, otherwise ouput text fragment
            *m_outputStream << TokenT( (start == fullLineStart && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, StringT( start, end));
            [OR][TRIM]
            *m_outputStream << TokenT( TokenT::eTextFragment, StringT( start, end));
            [MACRO_END][TRIM]
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
    bool m_closing;///<output line fragments as full line if closing to force flush[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
    bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
    [MACRO_BEGIN][IF][ENTRY]["Tokenizer Preprocessor Action"][TRIM]
    StringT m_[ENTRY]["Tag Name Small"]Keyword; ///<used for special preprocessing action
    StringT m_[ENTRY]["Tag Name Small"]DotKeyword; ///<used for special preprocessing action[IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]
    [MACRO_END][TRIM]
};

#endif /* INCLUDED_[ENTRY]["Tokenizer"][TO_UPPER]_TPL_H_6955377 */
