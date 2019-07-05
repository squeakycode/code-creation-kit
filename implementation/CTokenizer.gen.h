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

    ///Splits text line input into tokens.
    template <
          typename TokenT
        , typename StringT
        , typename OutputStreamT
        , typename FinalOutputStreamT
        , typename LogOutputStreamT = CNul >
    class CTokenizer
    {
    public:
        typedef CTokenizer<TokenT, StringT, OutputStreamT, FinalOutputStreamT, LogOutputStreamT> ThisT;
        typedef std::vector<StringT> KeywordListT;
        typedef regex_namespace::basic_regex<typename StringT::value_type, regex_namespace::regex_traits<typename StringT::value_type> > RegexT;
        typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
        typedef typename StringT::value_type CharT;

        CTokenizer()
            : m_outputStream(0)
            , m_finalOutputStream(0)
            , m_inlineTemplateMode(false)
            , m_logOutputStream(0)
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


        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_logOutputStream = stream;
        }

        ///sets up regex search expression; 
        void setMarkup( const StringT& prefix, const StringT& postfix)
        {
            m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
            m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
            m_trimLeftKeyword = prefix + STRING_LITERAL("TRIM_LEFT") + postfix;
            m_trimRightKeyword = prefix + STRING_LITERAL("TRIM_RIGHT") + postfix;

            StringT regexPrefix = prefix;
            StringT regexPostfix = postfix;
            CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPrefix);
            CSpecialRegexCharacterPrefixer::prefixSpecialCharacters( regexPostfix);

            StringT expression;
            StringT front = StringT() + STRING_LITERAL("|") + STRING_LITERAL("(") + regexPrefix;
            StringT back = regexPostfix + STRING_LITERAL(")");

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

        template <typename WhatT>
        typename TokenT::SharedStringListT getSourceText( WhatT& what, int pos, typename StringT::const_iterator& endPos)
        {
            typename TokenT::SharedStringListT list( new typename TokenT::StringListT(1));
            list->front().assign( what[ pos ].first, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            CAutoLineClear autoClear;
            regex_namespace::match_results<typename StringT::const_iterator> what;
            typename StringT::const_iterator start = line.begin();
            typename StringT::const_iterator fullLineStart = line.begin();
            typename StringT::const_iterator end = line.end(); 
            typename StringT::const_iterator trimLeftTokenTrailingTextBegin = end;
            typename StringT::const_iterator trimLeftTokenTrailingTextEnd = end;

            //check if the line needs to be trimmed or is comment
            for (;;)
            {
                RangeT range = trimRange( line, boost::is_any_of(" \t\n\r"));
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
                        range = trimRange( m_temporaryInlineTemplateLine, boost::is_any_of(" \t\n\r"));
                        start = m_temporaryInlineTemplateLine.begin();
                        fullLineStart = m_temporaryInlineTemplateLine.begin();
                        end = m_temporaryInlineTemplateLine.end(); 
                        trimLeftTokenTrailingTextBegin = end;
                        trimLeftTokenTrailingTextEnd = end;

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
                if ( boost::ends_with( range, m_trimRightKeyword))
                {
                    size_t keywordSize = m_trimRightKeyword.size();
                    end = range.end() - keywordSize;
                trimmedRight = true;
                break;
                }
                break;
            }

            while( regex_search(start, end, what, m_searchExpression)) 
            {

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
                else if ( what[ (TokenT::eComment) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eComment, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eComment, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eComment);
                    }
                }
                else if ( what[ (TokenT::eInclude) ].matched )
                {
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
                else if ( what[ (TokenT::eMarkup) ].matched )
                {
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
                else if ( what[ (TokenT::eTrim) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrim, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrim, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrim);
                    }
                }
                else if ( what[ (TokenT::eTrimLeft) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrimLeft, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrimLeft, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrimLeft);
                    }
                }
                else if ( what[ (TokenT::eTrimRight) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTrimRight, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTrimRight, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTrimRight);
                    }
                }
                else if ( what[ (TokenT::eSetRecursionLevelLimit) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eSetRecursionLevelLimit, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimit);
                    }
                }
                else if ( what[ (TokenT::eSetRecursionLevelLimitOff) ].matched )
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
                else if ( what[ (TokenT::eAny) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eAny, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eAny, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eAny);
                    }
                }
                else if ( what[ (TokenT::eVolatil) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eVolatil, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eVolatil, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eVolatil);
                    }
                }
                else if ( what[ (TokenT::eBegin) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eBegin, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eBegin, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eBegin);
                    }
                }
                else if ( what[ (TokenT::eBlockFormat) ].matched )
                {
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
                else if ( what[ (TokenT::eContains) ].matched )
                {
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
                else if ( what[ (TokenT::eCount) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eCount, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eCount, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eCount);
                    }
                }
                else if ( what[ (TokenT::eEnd) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eEnd, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eEnd, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eEnd);
                    }
                }
                else if ( what[ (TokenT::eEndsWith) ].matched )
                {
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
                else if ( what[ (TokenT::eEntry) ].matched )
                {
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
                else if ( what[ (TokenT::eMatches) ].matched )
                {
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
                else if ( what[ (TokenT::eError_) ].matched )
                {
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
                else if ( what[ (TokenT::eFirstTime) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eFirstTime, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eFirstTime, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eFirstTime);
                    }
                }
                else if ( what[ (TokenT::eFlush) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eFlush, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eFlush, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eFlush);
                    }
                }
                else if ( what[ (TokenT::eForAll) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eForAll, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eForAll, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eForAll);
                    }
                }
                else if ( what[ (TokenT::eHtmlEscape) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eHtmlEscape, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eHtmlEscape, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eHtmlEscape);
                    }
                }
                else if ( what[ (TokenT::eIf_) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIf_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIf_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIf_);
                    }
                }
                else if ( what[ (TokenT::eIgnoreCase) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIgnoreCase, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIgnoreCase, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIgnoreCase);
                    }
                }
                else if ( what[ (TokenT::eIndex) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eIndex, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eIndex, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eIndex);
                    }
                }
                else if ( what[ (TokenT::eLastTime) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eLastTime, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eLastTime, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eLastTime);
                    }
                }
                else if ( what[ (TokenT::eMacroBegin) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMacroBegin, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eMacroBegin, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMacroBegin);
                    }
                }
                else if ( what[ (TokenT::eMacroEnd) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eMacroEnd, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eMacroEnd, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eMacroEnd);
                    }
                }
                else if ( what[ (TokenT::eRegexMatches) ].matched )
                {
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
                else if ( what[ (TokenT::eMerge) ].matched )
                {
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
                else if ( what[ (TokenT::eNot_) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eNot_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eNot_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eNot_);
                    }
                }
                else if ( what[ (TokenT::eOr_) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eOr_, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eOr_, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eOr_);
                    }
                }
                else if ( what[ (TokenT::ePadLeft) ].matched )
                {
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
                else if ( what[ (TokenT::ePadRight) ].matched )
                {
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
                else if ( what[ (TokenT::eLeftToRight) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eLeftToRight, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eLeftToRight, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eLeftToRight);
                    }
                }
                else if ( what[ (TokenT::eTopDown) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eTopDown, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eTopDown, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eTopDown);
                    }
                }
                else if ( what[ (TokenT::eRegexReplace) ].matched )
                {
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
                else if ( what[ (TokenT::eReplace) ].matched )
                {
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
                else if ( what[ (TokenT::eStartsWith) ].matched )
                {
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
                else if ( what[ (TokenT::eToCString) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eToCString, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eToCString, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eToCString);
                    }
                }
                else if ( what[ (TokenT::eToLower) ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::eToLower, typename TokenT::SharedStringListT(), getSourceText( what, TokenT::eToLower, start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::eToLower);
                    }
                }
                else if ( what[ (TokenT::eToUpper) ].matched )
                {
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
                *m_outputStream << TokenT( TokenT::eTextFragment, start, end);
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
        StringT m_commentKeyword; ///<used for special preprocessing action
        StringT m_trimKeyword; ///<used for special preprocessing action
        StringT m_trimLeftKeyword; ///<used for special preprocessing action
        StringT m_trimRightKeyword; ///<used for special preprocessing action
        StringT m_inlinePrefix; ///< markup for inline template line
        StringT m_inlinePostfix; ///< markup for inline template line
        StringT m_inlineGeneratedPostfix; ///< marks a generated line
        StringT m_temporaryInlineTemplateLine; ///< stores a line; recursion level is greater than 1 when not empty
        FinalOutputStreamT* m_finalOutputStream; ///<the final ouput file
        bool m_inlineTemplateMode; ///<toggles inline template processing
        LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
