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
#include "cpptokenfinder.hpp"
#include "cppstringx.hpp"
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
            if (   prefix != cppstringx::trim_copy( prefix)
                || postfix != cppstringx::trim_copy( postfix)
                || generatedPostfix != cppstringx::trim_copy( generatedPostfix)
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
            if (cppstringx::ends_with( prefix+postfix, generatedPostfix))
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
            m_tokenFinder.clear();
            m_tokenData.clear();
            
            m_commentKeyword = prefix + STRING_LITERAL("COMMENT") + postfix;
            m_trimKeyword = prefix + STRING_LITERAL("TRIM") + postfix;
            m_trimLeftKeyword = prefix + STRING_LITERAL("TRIM_LEFT") + postfix;
            m_trimRightKeyword = prefix + STRING_LITERAL("TRIM_RIGHT") + postfix;

            m_tokenFinder.add_token(prefix + STRING_LITERAL("COMMENT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eComment));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("INCLUDE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eInclude, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_MARKUP") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMarkup, 2, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrim));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM_LEFT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrimLeft));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TRIM_RIGHT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTrimRight));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePart, 0, KeywordParameterParser::getParametersCombi1Plain1CStyleOptional1UIntOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_BEGIN") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartBegin, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_END") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_LAZY") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartLazy, 0, KeywordParameterParser::getParametersCombi1Plain1CStyleOptional1UIntOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_PADDING") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartPadding));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PART_REMOVE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePartRemove, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eSetRecursionLevelLimit));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("SET_RECURSION_LEVEL_LIMIT_OFF") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eSetRecursionLevelLimitOff));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_BEGIN") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableBegin, 0, KeywordParameterParser::getParametersCombi1Plain2CStyleOptional1PlainOptional1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_END") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_LOAD") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableLoad, 0, KeywordParameterParser::getParametersCombi2Plain2CStyleOptional1PlainOptional1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TABLE_REMOVE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTableRemove, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ANY") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eAny));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("AS_VOLATILE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eVolatil));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("BEGIN") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eBegin));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("BLOCK_FORMAT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eBlockFormat, 1, KeywordParameterParser::getParameters<CUIntParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("CALC") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eCalc, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("CONTAINS") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eContains, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("COUNT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eCount));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("END") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ENDS_WITH") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEndsWith, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ENTRY") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eEntry, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("EQUALS") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMatches, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("ERROR") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eError_, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FIRST_TIME") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eFirstTime));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FLUSH") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eFlush));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("FOR_ALL") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eForAll));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("HTML_ESCAPE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eHtmlEscape));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("IF") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIf_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("IGNORE_CASE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIgnoreCase));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("INDEX") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eIndex));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("LAST_TIME") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eLastTime));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MACRO_BEGIN") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMacroBegin));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MACRO_END") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMacroEnd));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MATCHES_REGEX") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eRegexMatches, 1, KeywordParameterParser::getParameters<CRegexParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("MERGE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eMerge, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("NOT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eNot_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("OR") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eOr_));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PAD_LEFT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePadLeft, 0, KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional<IteratorT, StringListT>, checkPadParameters));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("PAD_RIGHT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::ePadRight, 0, KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional<IteratorT, StringListT>, checkPadParameters));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("READ_LEFT_TO_RIGHT") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eLeftToRight));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("READ_TOP_DOWN") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eTopDown));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("REGEX_REPLACE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eRegexReplace, 2, KeywordParameterParser::getParameters<CRegexParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("REPLACE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eReplace, 2, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("STARTS_WITH") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eStartsWith, 1, KeywordParameterParser::getParameters<CCStyleParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_CSTRING") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToCString));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_CSV") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToCsv, 0, KeywordParameterParser::getParametersCombi1CStyle1CStyleOptional<IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_LOWER") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToLower));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_SIZE") + postfix, m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::eToSize, 1, KeywordParameterParser::getParameters<CPlainParameterPolicy, IteratorT, StringListT>));
            m_tokenFinder.add_token(prefix + STRING_LITERAL("TO_UPPER") + postfix, m_tokenData.size());
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

        SharedStringListT getSourceText(
            IteratorT tokenBegin,
            IteratorT& endPos
            )
        {
            SharedStringListT list = std::make_shared<typename TokenT::StringListT>(1);
            list->front().assign( tokenBegin, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            CAutoLineClear autoClear;
            IteratorT textBegin = line.begin();
            IteratorT fullLineBegin = line.begin();
            IteratorT textEnd = line.end(); 
            IteratorT trimLeftTokenTrailingTextBegin = textEnd;
            IteratorT trimLeftTokenTrailingTextEnd = textEnd;

            //check if the line needs to be trimmed or is comment
            for (;;)
            {
                RangeT range = trimRange( line, cppstringx::utility::is_any_of<const char*>(" \t\n\r"));
                //if in inline processing Mode
                if ( m_inlineTemplateMode && m_temporaryInlineTemplateLine.empty())
                {
                    //check whether the line contains generated content
                    if ( cppstringx::ends_with( range, m_inlineGeneratedPostfix))
                    {
                        //generated content is ignored/removed
                        return *this;
                    }

                    //write the line to the output file
                    *m_finalOutputStream << line;

                    //check whether the line contains template content
                    if (    cppstringx::starts_with( range, m_inlinePrefix) // must start with inline prefix
                        &&  (m_inlinePostfix.empty() // either no postfix
                            || 
                            (cppstringx::ends_with( range, m_inlinePostfix) // or ends with inline postfix
                            &&
                            static_cast<size_t>(range.end() - range.begin()) >= ( m_inlinePostfix.size() + m_inlinePrefix.size())) //and no overlap
                            )
                    )
                    {
                        //remove markup and create a new line used for processing
                        m_temporaryInlineTemplateLine.clear();
                        m_temporaryInlineTemplateLine.append( textBegin, range.begin());
                        m_temporaryInlineTemplateLine.append( range.begin() + m_inlinePrefix.size(), range.end() - m_inlinePostfix.size());
                        m_temporaryInlineTemplateLine.append( range.end(), textEnd);
                        
                        //switch to processing of this line
                        range = trimRange( m_temporaryInlineTemplateLine, cppstringx::utility::is_any_of<const char*>(" \t\n\r"));
                        textBegin = m_temporaryInlineTemplateLine.begin();
                        fullLineBegin = m_temporaryInlineTemplateLine.begin();
                        textEnd = m_temporaryInlineTemplateLine.end(); 
                        trimLeftTokenTrailingTextBegin = textEnd;
                        trimLeftTokenTrailingTextEnd = textEnd;

                        autoClear.set( &m_temporaryInlineTemplateLine);
                    }
                    else
                    {
                        //not template content, we are done 
                        return *this;
                    }
                }

                if ( cppstringx::starts_with( range, m_commentKeyword))
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
                if ( cppstringx::ends_with( range, m_trimLeftKeyword))
                {
                    size_t keywordSize = m_trimLeftKeyword.size();
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
                break;
            }

            IteratorT tokenBegin;
            IteratorT tokenEnd;
            size_t index = 0;

            while(m_tokenFinder.find_token(textBegin, textEnd, tokenBegin, tokenEnd, index)) 
            {
                const TokenData& tokenData = m_tokenData[index];
                
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
                        *m_outputStream << TokenT( tokenData.TokenId, list, getSourceText( tokenBegin, textBegin /*has been updated*/));
                    }
                    else
                    {
                        *m_outputStream << TokenT( tokenData.TokenId, list);
                    }
                }
                else
                {
                    if ( isLoggingEnabled())
                    {
                        *m_outputStream << TokenT( tokenData.TokenId, SharedStringListT(), getSourceText( tokenBegin, tokenEnd));
                    }
                    else
                    {
                        *m_outputStream << TokenT( tokenData.TokenId);
                    }
                }
            }

            if ( textBegin != textEnd)
            {
                *m_outputStream << TokenT( TokenT::eTextFragment, textBegin, textEnd);
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
