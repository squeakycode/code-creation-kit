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
#include "NullDevice.h"

[PART_BEGIN]["if front end"][IF][ENTRY]["Tokenizer"][EQUALS]["CTokenizer"][PART_END][TRIM]
[PART_BEGIN]["if back end"][IF][ENTRY]["Tokenizer"][EQUALS]["CBackEndTokenizer"][PART_END][TRIM]

namespace code_creation_kit
{
    [MACRO_BEGIN][PART]["if front end"][TRIM]
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
        , typename FinalOutputStreamT[PART]["if front end"]
        , typename LogOutputStreamT = CNul >
    class [ENTRY]["Tokenizer"]
    {
    public:
        typedef [ENTRY]["Tokenizer"]<TokenT, StringT, OutputStreamT[MACRO_BEGIN], FinalOutputStreamT[PART]["if front end"][MACRO_END], LogOutputStreamT> ThisT;
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
            TokenData(typename TokenT::ETokenT tokenId = TokenT::eInvalid, size_t parameterCount = 0, ParseParameters parseFunction = nullptr, CheckParameters checkParameters = nullptr)
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

        [ENTRY]["Tokenizer"]()
            : m_pOutputStream(nullptr)
            , m_closing(false)[PART]["if back end"]
            , m_bypassMode(false)[PART]["if back end"]
            , m_pFinalOutputStream(nullptr)[PART]["if front end"]
            , m_inlineTemplateMode(false)[PART]["if front end"]
            , m_pLogOutputStream(nullptr)
        {
        }

        [MACRO_BEGIN][PART]["if front end"][TRIM]

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
        void connectFinalOutputStream( FinalOutputStreamT* pStream)
        {
            m_pFinalOutputStream = pStream;
        }

        ///switches inline template mode
        void setInlineTemplateMode( bool enable)
        {
            m_inlineTemplateMode = enable;
        }

        [MACRO_END][TRIM]

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* pStream)
        {
            m_pLogOutputStream = pStream;
        }

        [MACRO_BEGIN][PART]["if back end"][TRIM]
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
            m_tokenFinder.clear();
            m_tokenData.clear();
            m_markupPostfix = postfix;[PART]["if back end"]
            
            [MACRO_BEGIN][IF][ENTRY]["Tokenizer Preprocessor Action"][TRIM]
            m_[ENTRY]["Tag Name Small"]Keyword = prefix + STRING_LITERAL("[ENTRY]["Tag Name"]") + postfix;
            m_[ENTRY]["Tag Name Small"]DotKeyword = prefix + STRING_LITERAL("[ENTRY]["Tag Name"].") + postfix;[PART.]["if back end"]
            [MACRO_END][TRIM]

            [MACRO_BEGIN][TRIM]
            m_tokenFinder.add_token(prefix + STRING_LITERAL("[ENTRY]["Tag Name"][READ_TOP_DOWN]")[BEGIN.][PART.]["if front end"] + postfix[OR.][END.], m_tokenData.size());
            m_tokenData.push_back(TokenData(TokenT::e[ENTRY]["Tag Name Capital"][BEGIN], 0, KeywordParameterParser::getParameters[ENTRY]["Parameter Format"][STARTS_WITH]["Combi"]<IteratorT, StringListT>[OR], [ENTRY]["Parameter Count"], KeywordParameterParser::getParameters<C[ENTRY]["Parameter Format"]ParameterPolicy, IteratorT, StringListT>[OR][END][BEGIN], [ENTRY]["Parameter Check Function"][OR][END]));
            [MACRO_END][TRIM]
            m_tokenFinder.add_token(STRING_LITERAL("\r\n"), m_tokenData.size());
            m_tokenFinder.add_token(STRING_LITERAL("\n"), m_tokenData.size());
            m_tokenData.emplace_back(TokenT::eNewLine);
        }

        ///connect receiver of processed stream
        void connectOutputStream( OutputStreamT* pStream)
        {
            m_pOutputStream = pStream;
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

        [MACRO_BEGIN][PART]["if back end"][TRIM]
        ///removes a delay mark if needed
        bool removeTick( IteratorT tokenBegin, IteratorT dotsBegin, IteratorT dotsEnd, IteratorT tokenEnd)
        {
            // if there is more than one delay mark, remove delay mark and output as text
            // one delay mark triggers processing
            // remove delay mark and output in bypass mode in any case
            if ( (dotsEnd - dotsBegin) > (m_bypassMode ? 0 : 1) )
            {
                *m_pOutputStream << TokenT( TokenT::eTextFragment, tokenBegin, dotsBegin);
                *m_pOutputStream << TokenT( TokenT::eTextFragment, dotsBegin + 1, tokenEnd);
                return true;
            }
            else if ( m_bypassMode) // output in bypass mode
            {
                *m_pOutputStream << TokenT( TokenT::eTextFragment, tokenBegin, tokenEnd);
                return true;
            }

            return false;
        }

        [MACRO_END][TRIM]
        SharedStringListT getSourceText(
            IteratorT tokenBegin,
            IteratorT dotsBegin,[PART]["if back end"]
            IteratorT dotsEnd,[PART]["if back end"]
            IteratorT& endPos
            )
        {
            [MACRO_BEGIN][PART]["if back end"][TRIM]
            if ( (dotsEnd - dotsBegin) > 0 )
            {
                SharedStringListT list = std::make_shared<typename TokenT::StringListT>(2);
                list->front().assign( tokenBegin, dotsBegin);
                list->back().assign( dotsBegin + 1, endPos);
                return list;
            }
            [MACRO_END][TRIM]
            SharedStringListT list = std::make_shared<typename TokenT::StringListT>(1);
            list->front().assign( tokenBegin, endPos);
            return list;
        }

        ///tokenize input line
        ThisT& operator <<( const StringT& line)
        {
            bool trimmedRight = false;
            CAutoLineClear autoClear;[PART]["if front end"]
            IteratorT textBegin = line.begin();
            IteratorT fullLineBegin = line.begin();
            IteratorT textEnd = line.end(); 
            IteratorT trimLeftTokenTrailingTextBegin = textEnd;
            IteratorT trimLeftTokenTrailingTextEnd = textEnd;

            //check if the line needs to be trimmed or is comment
            for (;[MACRO_BEGIN]!m_bypassMode[PART]["if back end"][MACRO_END];)
            {
                RangeT range = trimRange( line, cppstringx::utility::is_any_of<const char*>(" \t\n\r"));
                [MACRO_BEGIN][PART]["if front end"][TRIM]
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
                    *m_pFinalOutputStream << line;

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

                [MACRO_END][TRIM]
                [MACRO_BEGIN][TRIM]
                if ( cppstringx::[ENTRY]["Tokenizer Preprocessor Check"]( range, m_[ENTRY]["Tag Name Small"]Keyword))
                {
                    [BEGIN][TRIM]
                    size_t keywordSize = m_[ENTRY]["Tag Name Small"][STARTS_WITH]["trim"]Keyword.size();
                    [OR][END][TRIM]
                    [ENTRY]["Tokenizer Preprocessor Action"][REPLACE]["\n","\n                    "]
                }
                [MACRO_BEGIN.][PART.]["if back end"][TRIM.]
                if ( cppstringx::[ENTRY]["Tokenizer Preprocessor Check"]( range, m_[ENTRY]["Tag Name Small"]DotKeyword))
                {
                    [BEGIN][TRIM]
                    size_t keywordSize = m_[ENTRY]["Tag Name Small"][STARTS_WITH]["trim"]DotKeyword.size();
                    [OR][END][TRIM]
                    [ENTRY]["Tokenizer Preprocessor Action"][REPLACE]["\n","\n                    "]
                }
                [MACRO_END.][TRIM.]
                [MACRO_END][TRIM]
                break;
            }

            IteratorT tokenBegin;
            IteratorT tokenEnd;
            size_t index = 0;

            while(m_tokenFinder.find_token(textBegin, textEnd, tokenBegin, tokenEnd, index)) 
            {
                const TokenData& tokenData = m_tokenData[index];
                
                [MACRO_BEGIN][PART]["if back end"][TRIM]
                //if full line without tags, output as special token
                if ( TokenT::eNewLine == tokenData.TokenId && textBegin == fullLineBegin )
                {
                    *m_pOutputStream << TokenT( TokenT::eFullLineWithoutTags, textBegin, tokenEnd);
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
                
                [MACRO_END][TRIM]
                //forward preceding text as token, if not empty
                if ( textBegin != tokenBegin )
                {
                    *m_pOutputStream << TokenT( TokenT::eTextFragment, textBegin, tokenBegin);
                }

                //set textBegin position for next loop iteration
                textBegin = tokenEnd;

                //process tokens
                if ( TokenT::eNewLine == tokenData.TokenId )
                {
                    *m_pOutputStream << TokenT( TokenT::eNewLine, tokenBegin, tokenEnd);
                }
                else if ( tokenData.ParseFunction )
                {
                    [MACRO_BEGIN.][PART.]["if back end"][TRIM]
                    if ( removeTick( tokenBegin, dotsBegin, dotsEnd, tokenEnd))
                    {
                        continue;
                    }
                    [MACRO_END.][TRIM]
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
                            *m_pLogOutputStream << "Error parsing parameters in line:\n";
                            *m_pLogOutputStream << line;
                            *m_pLogOutputStream << StringT(fullLineBegin,textBegin) << "\n";
                        }
                        throw;
                    }
                    if ( isLoggingEnabled())
                    {
                        *m_pOutputStream << TokenT( tokenData.TokenId, list, getSourceText( tokenBegin, [BEGIN.]dotsBegin, dotsEnd, [PART.]["if back end"][OR.][END.]textBegin /*has been updated*/));
                    }
                    else
                    {
                        *m_pOutputStream << TokenT( tokenData.TokenId, list);
                    }
                }
                else
                {
                    [MACRO_BEGIN.][PART.]["if back end"][TRIM]
                    if (tokenData.TokenId == TokenT::eSetRecursionLevelLimit)
                    {
                        // Turn limit off to make sure that the new limit gets processed
                        *m_pOutputStream << TokenT( TokenT::eSetRecursionLevelLimitOff);
                    }
                    if (tokenData.TokenId == TokenT::eSetRecursionLevelLimitOff)
                    {
                        // This tag is always processed. Delay dots have no effect.
                    }
                    else if ( removeTick( tokenBegin, dotsBegin, dotsEnd, tokenEnd))
                    {
                        continue;
                    }
                    [MACRO_END.][TRIM]
                    if ( isLoggingEnabled())
                    {
                        *m_pOutputStream << TokenT( tokenData.TokenId, SharedStringListT(), getSourceText( tokenBegin, [BEGIN.]dotsBegin, dotsEnd, [PART.]["if back end"][OR.][END.]tokenEnd));
                    }
                    else
                    {
                        *m_pOutputStream << TokenT( tokenData.TokenId);
                    }
                }
            }

            if ( textBegin != textEnd)
            {
                [MACRO_BEGIN][PART]["if back end"][TRIM]
                //if full line without tags, output as special token used for optimizations, otherwise output text fragment
                *m_pOutputStream << TokenT( (textBegin == fullLineBegin && m_closing) ? TokenT::eFullLineWithoutTags : TokenT::eTextFragment, textBegin, textEnd);
                [OR][TRIM]
                *m_pOutputStream << TokenT( TokenT::eTextFragment, textBegin, textEnd);
                [MACRO_END][TRIM]
            }
            if ( trimmedRight)
            {
                //a right trimmed line is treated as line macro
                *m_pOutputStream << TokenT( TokenT::eNewLine);
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
                        *m_pOutputStream << TokenT(TokenT::eTextFragment, trimLeftTokenTrailingTextBegin, trimLeftTokenTrailingTextNewLine);
                    }
                    *m_pOutputStream << TokenT(TokenT::eNewLine, trimLeftTokenTrailingTextNewLine, trimLeftTokenTrailingTextEnd);
                }
                else
                {
                    *m_pOutputStream << TokenT(TokenT::eNewLine, trimLeftTokenTrailingTextBegin, trimLeftTokenTrailingTextEnd);
                }
            }
            return *this;
        }
    private:
        bool isLoggingEnabled()
        {
            return m_pLogOutputStream != nullptr;
        }
    private:
        std::vector<TokenData> m_tokenData;
        cpptokenfinder::token_finder<CharT, size_t, size_t, size_t(-1)> m_tokenFinder;
        OutputStreamT* m_pOutputStream; ///<sink for tokens
        bool m_closing;///<output line fragments as full line if closing to force flush[PART]["if back end"]
        bool m_bypassMode;///<used when limiting recursion level, forces text output with tick removal[PART]["if back end"]
        StringT m_markupPostfix;///<used when processing tick marks in back textEnd[PART]["if back end"]
        [MACRO_BEGIN][IF][ENTRY]["Tokenizer Preprocessor Action"][TRIM]
        StringT m_[ENTRY]["Tag Name Small"]Keyword; ///<used for special preprocessing action
        StringT m_[ENTRY]["Tag Name Small"]DotKeyword; ///<used for special preprocessing action[PART.]["if back end"]
        [MACRO_END][TRIM]
        [MACRO_BEGIN][PART]["if front end"][TRIM]
        StringT m_inlinePrefix; ///< markup for inline template line
        StringT m_inlinePostfix; ///< markup for inline template line
        StringT m_inlineGeneratedPostfix; ///< marks a generated line
        StringT m_temporaryInlineTemplateLine; ///< stores a line; recursion level is greater than 1 when not empty
        FinalOutputStreamT* m_pFinalOutputStream; ///<the final ouput file
        bool m_inlineTemplateMode; ///<toggles inline template processing
        [MACRO_END][TRIM]
        LogOutputStreamT* m_pLogOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
