//   Copyright (C) 2011-2012 Andreas Gau
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

///defines exceptions thrown by CTokenizer for template argument independent access
class CTokenizerExceptions
{
public:
    class ExInlinePrefixEmpty : public std::runtime_error //not in error printer table
    { public: ExInlinePrefixEmpty() : std::runtime_error( "An empty string is not allowed for inline template prefix.") {}};

    class ExInlineGeneratedPostfixEmpty : public std::runtime_error //not in error printer table
    { public: ExInlineGeneratedPostfixEmpty() : std::runtime_error( "An empty string is not allowed for inline template generated postfix.") {}};

    class ExInlineMarkupWhiteSpace : public std::runtime_error  //collides with TRIM  //not in error printer table
    { public: ExInlineMarkupWhiteSpace() : std::runtime_error( "Trailing or leading white space for inline template markup is not allowed.") {}};

    class ExBadInlineGeneratedPostfix : public std::runtime_error  //not in error printer table
    { public: ExBadInlineGeneratedPostfix() : std::runtime_error( "No combination of inline prefix and inline postfix must end with the inline generated postfix.") {}};
};

///Removes one tick from keywords in input
template <typename TokenT, typename StringT, typename OutputStreamT, typename FinalOutputStreamT>
class CTokenizer
{
public:
    typedef std::vector<StringT> KeywordListT;
    typedef boost::basic_regex<typename StringT::value_type, boost::regex_traits<typename StringT::value_type> > RegexT;
    typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
    typedef typename StringT::value_type CharT;

    CTokenizer()
        : m_outputStream(0)
        , m_finalOutputStream(0)
        , m_inlineTemplateMode(false)
    {
    }


    class CAutoLineClear
    {
    public:
        CAutoLineClear()
            : m_pLine(0)
        {
        }

        ~CAutoLineClear()
        {
            if ( m_pLine)
            {
                m_pLine->clear();
            }
        }

        void set( StringT* p)
        {
            m_pLine = p;
        }
    private:
        StringT* m_pLine;
    };

    ///reset state information
    void reset()
    {
        m_temporaryInlineTemplateLine.clear();
    }

    ///set inline markup
    void setInlineTemplateMarkup( const StringT& prefix, const StringT& postfix,  const StringT& generatedPostfix)
    {
        if (   prefix != boost::trim_copy( prefix)
            || postfix != boost::trim_copy( postfix)
            || generatedPostfix != boost::trim_copy( generatedPostfix)
        )
        {
            throw CTokenizerExceptions::ExInlineMarkupWhiteSpace();
        }
        if ( prefix.empty())
        {
            throw CTokenizerExceptions::ExInlinePrefixEmpty();
        }
        if ( generatedPostfix.empty())
        {
            throw CTokenizerExceptions::ExInlineGeneratedPostfixEmpty();
        }
        if ( boost::ends_with( prefix+postfix, generatedPostfix))
        {
            throw CTokenizerExceptions::ExBadInlineGeneratedPostfix();
        }

        m_inlinePrefix = prefix;
        m_inlinePostfix = postfix;
        m_inlineGeneratedPostfix = generatedPostfix;
    }

    ///connect output file stream
    void connectFinalOutputStream( FinalOutputStreamT* stream)
    {
        m_finalOutputStream = stream;
    }

    ///switches inline template mode
    void setInlineTemplateMode( bool enable)
    {
        m_inlineTemplateMode = enable;
    }

    ///sets up regex search expression; 
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
        m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
        m_trimLeftKeyword = prefix + STRING_LITERAL("TRIM_LEFT") + postfix;

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
        expression += front + STRING_LITERAL("TRIM_LEFT") + back;
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

    ///tokenize input line
    CTokenizer<TokenT, StringT, OutputStreamT, FinalOutputStreamT>& operator <<( const StringT& line)
    {
        bool trimmed = false;
        CAutoLineClear autoClear;
        boost::match_results<typename StringT::const_iterator> what; 
        typename StringT::const_iterator start = line.begin();
        typename StringT::const_iterator fullLineStart = line.begin();
        typename StringT::const_iterator end = line.end(); 

        //check if the line needs to be trimmed or is comment
        for (;;)
        {
            RangeT range = trimRange( line, boost::is_any_of(" \t\n"));
            //if in inline processing Mode
            if ( m_inlineTemplateMode && m_temporaryInlineTemplateLine.empty())
            {
                //check whether the line contains generated content
                if ( boost::ends_with( range, m_inlineGeneratedPostfix))
                {
                    //generated content is ignored/removed
                    return *this;
                }

                //write the line to the output file
                *m_finalOutputStream << line;

                //check whether the line contains template content
                if (    boost::starts_with( range, m_inlinePrefix) // must start with prefix
                    &&  (m_inlinePostfix.empty() // either no postfix
                        || 
                        (boost::ends_with( range, m_inlinePostfix) // or ends with postfix
                        &&
                        static_cast<size_t>(range.size()) >= ( m_inlinePostfix.size() + m_inlinePrefix.size())) //and no overlap
                        )
                )
                {
                    //remove markup and create a new line used for processing
                    m_temporaryInlineTemplateLine.clear();
                    m_temporaryInlineTemplateLine.append( start, range.begin());
                    m_temporaryInlineTemplateLine.append( range.begin() + m_inlinePrefix.size(), range.end() - m_inlinePostfix.size());
                    m_temporaryInlineTemplateLine.append( range.end(), end);
                    
                    //switch to processing of this line
                    range = trimRange( m_temporaryInlineTemplateLine, boost::is_any_of(" \t\n"));
                    start = m_temporaryInlineTemplateLine.begin();
                    fullLineStart = m_temporaryInlineTemplateLine.begin();
                    end = m_temporaryInlineTemplateLine.end(); 

                    autoClear.set( &m_temporaryInlineTemplateLine);
                }
                else
                {
                    //not template content, we are done 
                    return *this;
                }
            }

            if ( boost::starts_with( range, m_commentKeyword))
            {
                return *this;
            }
            if ( boost::ends_with( range, m_trimKeyword))
            {
                size_t keywordSize = m_trimKeyword.size();
                start = range.begin();
                end = range.end() - keywordSize;
                trimmed = true;
                break;
            }
            if ( boost::ends_with( range, m_trimLeftKeyword))
            {
                size_t keywordSize = m_trimLeftKeyword.size();
                end = range.end() - keywordSize;
                trimmed = true;
                break;
            }
            break;
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
            else if ( what[ (TokenT::eTrimLeft) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eTrimLeft);
            }
            else if ( what[ (TokenT::eSetRecursionLevelLimit) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit);
            }
            else if ( what[ (TokenT::eSetRecursionLevelLimitOff) ].matched )
            {
                *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);
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
            else if ( what[ (TokenT::eError_) ].matched )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                list->resize( 1);
                KeywordParameterParser::getParameters<CCStyleParameterPolicy>( start, end, *list);
                *m_outputStream << TokenT( TokenT::eError_, list);
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
    StringT m_commentKeyword; ///<used for special preprocessing action
    StringT m_trimKeyword; ///<used for special preprocessing action
    StringT m_trimLeftKeyword; ///<used for special preprocessing action
    StringT m_inlinePrefix; ///< markup for inline template line
    StringT m_inlinePostfix; ///< markup for inline template line
    StringT m_inlineGeneratedPostfix; ///< marks a generated line
    StringT m_temporaryInlineTemplateLine; ///< stores a line; recursion level is greater than 1 when not empty
    FinalOutputStreamT* m_finalOutputStream; ///<the final ouput file
    bool m_inlineTemplateMode; ///<toggles inline template processing
};

#endif /* INCLUDED_CTOKENIZER_TPL_H_6955377 */
