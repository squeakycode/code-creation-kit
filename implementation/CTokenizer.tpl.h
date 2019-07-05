//  Copyright (c) 2011-2015 Andreas Gau
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
#include "CNul.h"

namespace code_creation_kit
{
    [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][TRIM]
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
    [MACRO_END][TRIM]

    ///Splits text line input into tokens.
    template <
          typename TokenT
        , typename StringT
        , typename OutputStreamT
        , typename FinalOutputStreamT[IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"]
        , typename LogOutputStreamT = CNul >
    class [ENTRY]["Tokenizer"]
    {
    public:
        typedef [ENTRY]["Tokenizer"]<TokenT, StringT, OutputStreamT[MACRO_BEGIN], FinalOutputStreamT[IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][MACRO_END], LogOutputStreamT> ThisT;
        typedef std::vector<StringT> KeywordListT;
        typedef regex_namespace::basic_regex<typename StringT::value_type, regex_namespace::regex_traits<typename StringT::value_type> > RegexT;
        typedef boost::iterator_range<typename StringT::const_iterator> RangeT;
        typedef typename StringT::value_type CharT;

        [ENTRY]["Tokenizer"]()
            : m_outputStream(0)
            , m_closing(false)[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
            , m_bypassMode(false)[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
            , m_finalOutputStream(0)[IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"]
            , m_inlineTemplateMode(false)[IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"]
            , m_logOutputStream(0)
        {
        }

        [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][TRIM]

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

        [MACRO_END][TRIM]

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_logOutputStream = stream;
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

            expression += STRING_LITERAL("(\r\n|\n)");
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

        [MACRO_END][TRIM]
        template <typename WhatT>
        typename TokenT::SharedStringListT getSourceText( WhatT& what, int pos, typename StringT::const_iterator& endPos)
        {
            [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][TRIM]
            pos = pos * 2 - 1;
            if ( (what[ pos ].second - what[ pos ].first) > 0 )
            {
                typename TokenT::SharedStringListT list( new typename TokenT::StringListT(2));
                list->front().assign( what[ pos - 1 ].first, what[ pos ].first);
                list->back().assign( what[ pos ].first + 1, endPos);
                return list;
            }
            --pos;
            [MACRO_END][TRIM]
            typename TokenT::SharedStringListT list( new typename TokenT::StringListT(1));
            list->front().assign( what[ pos ].first, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            CAutoLineClear autoClear;[IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"]
            regex_namespace::match_results<typename StringT::const_iterator> what;
            typename StringT::const_iterator start = line.begin();
            typename StringT::const_iterator fullLineStart = line.begin();
            typename StringT::const_iterator end = line.end(); 
            typename StringT::const_iterator trimLeftTokenTrailingTextBegin = end;
            typename StringT::const_iterator trimLeftTokenTrailingTextEnd = end;

            //check if the line needs to be trimmed or is comment
            for (;[MACRO_BEGIN]!m_bypassMode[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][MACRO_END];)
            {
                RangeT range = trimRange( line, boost::is_any_of(" \t\n\r"));
                [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][TRIM]
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

                [MACRO_END][TRIM]
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
                [MACRO_END][TRIM]

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
                [MACRO_BEGIN][TRIM]
                else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
                {
                    [MACRO_BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"][TRIM]
                    if ( removeTick( what, (TokenT::e[ENTRY]["Tag Name Capital"] * 2) - 1))
                    {
                        continue;
                    }
                    [MACRO_END.][TRIM]
                    typename TokenT::SharedStringListT list( new typename TokenT::StringListT);
                    try
                    {
                    [BEGIN][TRIM]
                        KeywordParameterParser::getParameters[ENTRY]["Parameter Format"][STARTS_WITH]["Combi"](start, end, *list);
                    [OR][TRIM]
                        list->resize([ENTRY]["Parameter Count"]);
                        KeywordParameterParser::getParameters<C[ENTRY]["Parameter Format"]ParameterPolicy>( start, end, *list);
                    [END][TRIM]
                        [BEGIN][TRIM]
                        [ENTRY]["Parameter Check Function"](*list);
                    [OR][END][TRIM]
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
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"], list, getSourceText( what, TokenT::e[ENTRY]["Tag Name Capital"], start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"], list);
                    }
                }
                [OR][IF][ENTRY]["Tag Name Capital"][EQUALS]["SetRecursionLevelLimitOff"][TRIM]
                else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"], typename TokenT::SharedStringListT(), getSourceText( what, TokenT::e[ENTRY]["Tag Name Capital"], start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"]);
                    }
                }
                [OR][TRIM]
                else if ( what[ (TokenT::e[ENTRY]["Tag Name Capital"][BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]-1)*2[OR.])[END.] ].matched )
                {
                    [MACRO_BEGIN.][IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"][TRIM]
                    [BEGIN][IF][ENTRY]["Tag Name Capital"][EQUALS]["SetRecursionLevelLimit"][TRIM]
                    //turn limit off to make sure that the new limit gets processed
                    *m_outputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);

                    [OR][END][TRIM]
                    if ( removeTick( what, (TokenT::e[ENTRY]["Tag Name Capital"] * 2) - 1))
                    {
                        continue;
                    }
                    [MACRO_END.][TRIM]
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"], typename TokenT::SharedStringListT(), getSourceText( what, TokenT::e[ENTRY]["Tag Name Capital"], start));
                    }
                    else
                    {
                        *m_outputStream << TokenT( TokenT::e[ENTRY]["Tag Name Capital"]);
                    }
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
                *m_outputStream << TokenT( (start == fullLineStart && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, start, end);
                [OR][TRIM]
                *m_outputStream << TokenT( TokenT::eTextFragment, start, end);
                [MACRO_END][TRIM]
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
        bool m_closing;///<output line fragments as full line if closing to force flush[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
        bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal[IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"]
        [MACRO_BEGIN][IF][ENTRY]["Tokenizer Preprocessor Action"][TRIM]
        StringT m_[ENTRY]["Tag Name Small"]Keyword; ///<used for special preprocessing action
        StringT m_[ENTRY]["Tag Name Small"]DotKeyword; ///<used for special preprocessing action[IF.][ENTRY.]["Tokenizer"][EQUALS.]["CBackEndTokenizer"]
        [MACRO_END][TRIM]
        [MACRO_BEGIN][IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][TRIM]
        StringT m_inlinePrefix; ///< markup for inline template line
        StringT m_inlinePostfix; ///< markup for inline template line
        StringT m_inlineGeneratedPostfix; ///< marks a generated line
        StringT m_temporaryInlineTemplateLine; ///< stores a line; recursion level is greater than 1 when not empty
        FinalOutputStreamT* m_finalOutputStream; ///<the final ouput file
        bool m_inlineTemplateMode; ///<toggles inline template processing
        [MACRO_END][TRIM]
        LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
