// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "ConversionDirectives.gen.h"
#include "StringLiteral.h"
#include <stdexcept>
#include <cassert>
#include <memory>
#include <map>
#include <regex>
#include "cppstringx.hpp"
#include "StringConvert.h"

namespace code_creation_kit
{
    template <typename StringT>
    class CReplaceConversion : public ConversionDirectives<StringT>
    {
    public:
        typedef CReplaceConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        CReplaceConversion( const StringT& replaceText, const StringT& withText)
            : m_ignoreCase( false)
            , m_replace( replaceText)
            , m_with( withText)
        {
        }

        void ignoreCase( bool ignoreCase) override
        {
            m_ignoreCase = ignoreCase; 
        }

        [[nodiscard]] bool ignoreCase() const override
        {
            return m_ignoreCase;
        }

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                if ( 
                    m_ignoreCase != pConversionRhs->m_ignoreCase
                    || m_replace != pConversionRhs->m_replace
                    || m_with != pConversionRhs->m_with
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        void modify( StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                if ( m_ignoreCase)
                {
                    cppstringx::ireplace_all_in_place( text, m_replace, m_with);
                }
                else
                {
                    cppstringx::replace_all_in_place( text, m_replace, m_with);
                }
            }
        }
    private:
        bool    m_ignoreCase;
        StringT m_replace;
        StringT m_with;
    };

    ///defines exceptions thrown by CRegexReplaceConversion for template argument independent access
    class RegexReplaceConversionExceptions
    {
    public:
        class ExRegexSyntaxError : public std::runtime_error
        { public: ExRegexSyntaxError() : std::runtime_error( "Syntax error in regular expression") {}};
    };

    template <typename StringT>
    class CRegexReplaceConversion : public ConversionDirectives<StringT>, public RegexReplaceConversionExceptions
    {
    public:
        typedef std::basic_regex<typename StringT::value_type, std::regex_traits<typename StringT::value_type> > RegexT;
        typedef CRegexReplaceConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        CRegexReplaceConversion( const StringT& replaceText, const StringT& withText)
            : m_ignoreCase( false)
            , m_replace( replaceText)
            , m_with( withText)
        {
            try
            {
                m_regexReplace = RegexT( m_replace);
                m_regexReplaceIgnoreCase = RegexT( m_replace, std::regex::icase);
            }
            catch(...)
            {
                throw ExRegexSyntaxError();
            }
        }

        void ignoreCase( bool ignoreCase) override
        {
            m_ignoreCase = ignoreCase; 
        }

        [[nodiscard]] bool ignoreCase() const override
        {
            return m_ignoreCase;
        }

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                if ( 
                    m_ignoreCase != pConversionRhs->m_ignoreCase
                    || m_replace != pConversionRhs->m_replace
                    || m_with != pConversionRhs->m_with
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        void modify( StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                if ( m_ignoreCase)
                {
                    text = std::regex_replace( text, m_regexReplaceIgnoreCase, m_with);
                }
                else
                {
                    text = std::regex_replace( text, m_regexReplace, m_with);
                }
            }
        }

    private:
        bool m_ignoreCase;
        StringT m_replace;
        StringT m_with;
        RegexT m_regexReplace;
        RegexT m_regexReplaceIgnoreCase;
    };

    template <typename StringT>
    class CMergeConversion : public ConversionDirectives<StringT>
    {
    public:
        typedef CMergeConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        explicit CMergeConversion( const StringT& separator)
            : m_separator( separator)
        {
        }

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                if ( 
                    m_separator != pConversionRhs->m_separator
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        void modify( StringListT& textList) const override
        {
            StringT merged;
            bool first = true;
            for (StringT& text : textList)
            {
                if ( first)
                {
                    first = false;
                }
                else
                {
                    merged += m_separator;
                }
                merged += text;
            }
            textList.resize( 1);
            textList[0].swap( merged);
        }
    private:
        StringT m_separator;
    };

    template <typename StringT>
    class CToLowerConversion : public ConversionDirectives<StringT>
    {
    public:
        typedef CToLowerConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                return true;
            }
            return false;
        }

        void modify( StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                cppstringx::to_lower_in_place( text);
            }
        }
    };

    template <typename StringT>
    class CToUpperConversion : public ConversionDirectives<StringT>
    {
    public:
        typedef CToUpperConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                return true;
            }
            return false;
        }

        void modify( StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                cppstringx::to_upper_in_place( text);
            }
        }
    };


    template <typename StringT>
    class CToCStringConversion : public ConversionDirectives<StringT>
    {
    public:
        typedef CToUpperConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==( const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if ( pConversionRhs)
            {
                return true;
            }
            return false;
        }

        StringT convertSpecialCharacters( const StringT& text) const
        {
            typedef typename StringT::value_type CharT;
            const CharT* specialChars = STRING_LITERAL("\'\"\?\\\a\b\f\n\r\t\v");
            const CharT* specialCharLiteral = STRING_LITERAL( "\'\"\?\\abfnrtv");

            StringT result;
            for (CharT c : text)
            {
                for ( const CharT* p = specialChars; *p; ++p)
                {
                    if ( c == *p )
                    {
                        result += STRING_LITERAL('\\');
                        c = specialCharLiteral[ p - specialChars ];
                        break;
                    }
                }
                result += c;
            }
            return result;
        }

        void modify( StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                text = convertSpecialCharacters( text);
            }
        }
    };

    template <typename StringT>
    class CHtmlEscapeConversion : public ConversionDirectives<StringT>
    {
        typedef typename StringT::value_type CharT;
        struct SCharTable
        {
            const CharT c;
            const CharT* htmlEscaped;
        };

    public:
        typedef CHtmlEscapeConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                return true;
            }
            return false;
        }

        StringT convertSpecialCharacters(const StringT& text) const
        {
            static const SCharTable table[] =
                {
                    {'&' , STRING_LITERAL("&amp;")},
                    {'<' , STRING_LITERAL("&lt;")  },
                    {'>' , STRING_LITERAL("&gt;")  },
                    {'"' , STRING_LITERAL("&quot;")},
                    {'\'', nullptr   },
                    {'`' , nullptr   },
                    {'(' , nullptr   },
                    {')' , nullptr   },
                    {'{' , nullptr   },
                    {'}' , nullptr   },
                    {'[' , nullptr   },
                    {']' , nullptr   },
                    {'!' , nullptr   },
                    {'@' , nullptr   },
                    {'$' , nullptr   },
                    {'%' , nullptr   },
                    {'=' , nullptr   },
                    {'+' , nullptr   },
                    {  0 , nullptr   }
                };

            // iterate all chars of text
            StringT result;
            for (CharT c : text)
            {
                //check for characters to escape
                const SCharTable* p = table;
                for (; p->c; ++p)
                {
                    if (c == p->c)
                    {
                        //character to escape found
                        if (p->htmlEscaped)
                        {
                            result += p->htmlEscaped;
                        }
                        else
                        {
                            result += STRING_LITERAL("&#");
                            result += ToString<StringT>(static_cast<int>(c));
                            result += STRING_LITERAL(";");
                        }
                        break;
                    }
                }
                //has not been escaped?
                if (!p->c)
                {
                    result += c;
                }
            }
            return result;
        }

        void modify(StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                text = convertSpecialCharacters(text);
            }
        }
    };

    template <typename StringT>
    class CPadConversionBase : public ConversionDirectives<StringT>
    {
    protected:
        typedef typename StringT::value_type CharT;

        template<typename IteratorT>
        CPadConversionBase(
            const StringT& padText
            , const StringT& padWidthFirst
            , const IteratorT& padWidthOptionalBegin
            , const IteratorT& padWidthOptionalEnd
            , bool padLeft
        )
        {
            //any input to use for padding?
            if (!padText.empty())
            {
                //get text that is always added
                if (padText.size() > 1)
                {
                    if (padLeft)
                    {
                        m_extensionText.assign(padText.begin(), padText.end() - 1);
                    }
                    else
                    {
                        m_extensionText.assign(padText.begin() + 1, padText.end());
                    }
                }
                //get char to repeat for padding
                if (padLeft)
                {
                    m_padChar = *padText.rbegin();
                }
                else
                {
                    m_padChar = *padText.begin();
                }
            }
            else
            {
                m_padChar = 0;
            }

            //get line based info used for padding
            m_padWidthInfos.reserve(1 + (padWidthOptionalEnd == padWidthOptionalBegin ? 0 : padWidthOptionalEnd - padWidthOptionalBegin));
            m_padWidthInfos.push_back(SPadWidthInfo(padWidthFirst));
            for (IteratorT it = padWidthOptionalBegin; it != padWidthOptionalEnd; ++it)
            {
                m_padWidthInfos.push_back(SPadWidthInfo(*it));
            }
            
            //remove unsupported chars
            const CharT newLine = STRING_LITERAL('\n');
            const CharT cr = STRING_LITERAL('\r');
            const CharT tab = STRING_LITERAL('\t');
            for (typename StringT::iterator it = m_extensionText.begin(); it != m_extensionText.end();)
            {
                if (*it == newLine || *it == tab || *it == cr)
                {
                    it = m_extensionText.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            if (m_padChar == newLine || m_padChar == tab || m_padChar == cr)
            {
                m_padChar = STRING_LITERAL(' ');
            }
       }


        StringT pad(const StringT& text, bool padLeft) const
        {
            StringT result;
            const CharT newLine = STRING_LITERAL('\n');
            const CharT tab = STRING_LITERAL('\t');
            size_t lineWidth = 0;
            size_t lineNumber = 0;
            size_t extensionTextSize = m_extensionText.size();
            for (typename StringT::const_iterator it = text.begin(), lineStart = text.begin();;++it)
            {
                if (it == text.end() || *it == newLine)
                {
                    //get number pad width info
                    SPadWidthInfo padInfo;
                    if (lineNumber < m_padWidthInfos.size())
                    {
                        padInfo = m_padWidthInfos[lineNumber];
                    }
                    else
                    {
                        padInfo = m_padWidthInfos.back();
                    }

                    if (padInfo.padWidth == 0 && m_padChar) //if pad width == 0 do not pad
                    {
                        result.append(lineStart, it);
                    }
                    else if (padInfo.extend) //padding by extension needed?
                    {
                        if (padLeft)
                        {
                            //pad left
                            result += m_extensionText;
                            if (padInfo.padWidth > extensionTextSize)
                            {
                                result.append(padInfo.padWidth - extensionTextSize, m_padChar);
                            }
                            result.append(lineStart, it);
                        }
                        else
                        {
                            //pad right
                            result.append(lineStart, it);
                            if (padInfo.padWidth > extensionTextSize)
                            {
                                result.append(padInfo.padWidth - extensionTextSize, m_padChar);
                            }
                            result += m_extensionText;
                        }
                    }
                    else
                    {
                        if (padLeft)
                        {
                            result += m_extensionText;
                        }
                        else
                        {
                            result.append(lineStart, it);
                        }

                        lineWidth += extensionTextSize;
                        if ( lineWidth < padInfo.padWidth)
                        {
                            result.append(padInfo.padWidth - lineWidth, m_padChar);
                        }

                        if (padLeft)
                        {
                            result.append(lineStart, it);
                        }
                        else
                        {
                            result += m_extensionText;
                        }
                    }

                    if (it == text.end())
                    {
                        break;
                    }

                    result += *it;
                    lineStart = it + 1;
                    ++lineNumber;
                    lineWidth = 0;
                }
                else if (*it == tab)
                {
                    lineWidth += cTabSize;
                }
                else
                {
                    ++lineWidth;
                }
            }
            return result;
        }

        bool baseEquals(const CPadConversionBase<StringT>& rhs) const
        {
            if (m_extensionText != rhs.m_extensionText
                || m_padChar != rhs.m_padChar
                || m_padWidthInfos.size() != rhs.m_padWidthInfos.size()
                )
            {
                return false;
            }
            //direct comparison triggers MSVC Compiler Error C2672: 'operator __surrogate_func': no matching overloaded function found
            auto itA = m_padWidthInfos.cbegin();
            auto itAEnd = m_padWidthInfos.cend();
            auto itB = rhs.m_padWidthInfos.cbegin();
            for (; itA != itAEnd; ++itA, ++itB)
            {
                if (*itA != *itB)
                {
                    return false;
                }
            }
            return true;
        }

        class SPadWidthInfo
        {
        public:
            explicit SPadWidthInfo(size_t padWidth_ = 0, bool extend_ = false)
                : padWidth(padWidth_)
                , extend(extend_)
            {
            }

            explicit SPadWidthInfo(const StringT& padWidth)
                : padWidth(static_cast<size_t>(std::stoul(padWidth)))
                , extend(!padWidth.empty() && padWidth[0] == STRING_LITERAL('+'))
            {
            }

            ~SPadWidthInfo() = default;

            bool operator != (const SPadWidthInfo& rhs) const
            {
                bool result = padWidth != rhs.padWidth || extend != rhs.extend;
                return result;
            }

            size_t padWidth;
            bool extend;
        };

        StringT m_extensionText;
        CharT m_padChar;
        std::vector<SPadWidthInfo> m_padWidthInfos;
        constexpr static size_t cTabSize = 4;
    };


    template <typename StringT>
    class CPadLeftConversion : public CPadConversionBase<StringT>
    {
    public:
        
        template<typename IteratorT>
        CPadLeftConversion(
            const StringT& padText
            , const StringT& padWidthFirst
            , const IteratorT& padWidthOptionalBegin
            , const IteratorT& padWidthOptionalEnd
            )
            : CPadConversionBase<StringT>(padText, padWidthFirst, padWidthOptionalBegin, padWidthOptionalEnd, true)
        {
        }

        typedef CPadLeftConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                bool result = this->baseEquals(*pConversionRhs);
                return result;
            }
            return false;
        }

        template <typename StringLocalT>
        static StringLocalT getStaticExtendingPadLeftText(const StringLocalT& padText, const StringLocalT& padWidthFirst)
        {
            typedef typename StringLocalT::value_type CharT;
            typename CPadLeftConversion<StringLocalT>::StringListT dummy; //no additional width values
            if (padWidthFirst.empty()) //nothing provided
            {
                CPadLeftConversion<StringLocalT> helper(padText, padWidthFirst, dummy.begin(), dummy.end());
                dummy.resize(1); //modify an empty sting to get the result
                helper.modify(dummy);
            }
            else
            {
                //use the special extending behavior used when the width is having the + sign prepended
                CPadLeftConversion<StringLocalT> helper(padText, ((padWidthFirst[0] == STRING_LITERAL('+')) ? padWidthFirst : (STRING_LITERAL('+') + padWidthFirst)), dummy.begin(), dummy.end());
                dummy.resize(1); //modify an empty sting to get the result
                helper.modify(dummy);
            }
            return dummy.front();
        }

        void modify(StringListT& textList) const override
        {
            if (this->m_padChar)
            {
                for (StringT& text : textList)
                {
                    text = this->pad(text, true);
                }
            }
        }
    };

    template <typename StringT>
    class CPadRightConversion : public CPadConversionBase<StringT>
    {
    public:

        template<typename IteratorT>
        CPadRightConversion(
            const StringT& padText
            , const StringT& padWidthFirst
            , const IteratorT& padWidthOptionalBegin
            , const IteratorT& padWidthOptionalEnd
            )
            : CPadConversionBase<StringT>(padText, padWidthFirst, padWidthOptionalBegin, padWidthOptionalEnd, false)
        {
        }

        typedef CPadRightConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                bool result = this->baseEquals(*pConversionRhs);
                return result;
            }
            return false;
        }

        void modify(StringListT& textList) const override
        {
            if (this->m_padChar)
            {
                for (StringT& text : textList)
                {
                    text = this->pad(text, false);
                }
            }
        }
    };

    template <typename StringT>
    class CBlockFormatConversion : public ConversionDirectives<StringT>
    {
        typedef typename StringT::value_type CharT;
    public:
        explicit CBlockFormatConversion(const StringT& blockWidth)
        {
            m_blockWidth = static_cast<size_t>(std::stoul(blockWidth));
        }

        typedef CBlockFormatConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                if (this->m_blockWidth != pConversionRhs->m_blockWidth)
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        StringT blockFormat(const StringT& text) const
        {
            const CharT newLine = STRING_LITERAL('\n');
            const CharT tab = STRING_LITERAL('\t');
            const CharT space = STRING_LITERAL(' ');
            StringT result;

            size_t count = 0;
            typename StringT::const_iterator lineStart = text.begin();
            typename StringT::const_iterator lastWhiteSpace = text.begin();
            typename StringT::const_iterator it = text.begin();
            while (it != text.end())
            {
                CharT c = *it;
                if (c == tab)
                {
                    lastWhiteSpace = it;
                    count += cTabSize;
                    ++it;
                }
                else if (c == space)
                {
                    lastWhiteSpace = it;
                    count += 1;
                    ++it;
                }
                else if (c == newLine)
                {
                    ++it;
                    result += StringT(lineStart, it);
                    lastWhiteSpace = lineStart = it;
                    count = 0;
                    continue;
                }
                else
                {
                    ++it;
                    ++count;
                }

                if (count >= m_blockWidth)
                {
                    if (it != text.end() && *it == newLine)
                    {
                        //next char is new line anyway
                        ++it;
                        result += StringT(lineStart, it);
                        lastWhiteSpace = lineStart = it;
                        count = 0;
                    }
                    else if (lineStart != lastWhiteSpace)
                    {
                        //there is whitespace in the parsed text
                        //whitespace is replaced by new line

                        if (it != text.end() && (*it == space || *it == tab))
                        {
                            //next char is space anyway
                            result += StringT(lineStart, it);
                            result += newLine;
                            ++it;
                            lastWhiteSpace = lineStart = it;
                            count = 0;
                        }
                        else
                        {
                            result += StringT(lineStart, lastWhiteSpace);
                            result += newLine;
                            ++lastWhiteSpace;
                            lineStart = lastWhiteSpace;
                            count = it - lastWhiteSpace;
                        }
                    }
                    else
                    {
                        //forcibly insert a new line into text
                        result += StringT(lineStart, it);
                        if (it != text.end())
                        {
                            result += newLine;
                        }
                        lastWhiteSpace = lineStart = it;
                        count = 0;
                    }
                }
            }
            if (lineStart != text.end())
            {
                result += StringT(lineStart, text.end());
            }

            return result;
        }

        void modify(StringListT& textList) const override
        {
            if (m_blockWidth)
            {
                for (StringT& text : textList)
                {
                    text = blockFormat(text);
                }
            }
        }

    private:
        size_t m_blockWidth;
        constexpr static size_t cTabSize = 4;
    };

    ///defines exceptions thrown by CCaclulationConversion for template argument independent access
    class CalcConversionExceptions
    {
    public:
        class ExArithmeticExpressionSyntaxError : public std::runtime_error
        {
        public: ExArithmeticExpressionSyntaxError() : std::runtime_error("Syntax error in arithmetic expression.") {}
        };

        class ExDivisionByZero : public std::runtime_error
        {
        public: ExDivisionByZero() : std::runtime_error("Division by zero.") {}
        };
        class ExCalculationError : public std::runtime_error //internal error
        {
        public: ExCalculationError() : std::runtime_error("Error in calculation.") {}
        };
    };

    template <typename StringT>
    class CCalcConversion : public ConversionDirectives<StringT>, public CalcConversionExceptions
    {
        typedef typename StringT::value_type CharT;
        typedef typename StringT::const_iterator IteratorT;
    public:
        typedef CCalcConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        explicit CCalcConversion(const StringT& expression)
            : m_expression(expression)
        {
            IteratorT pos = expression.cbegin();
            IteratorT end = expression.cend();
            if (!parseExpression(pos, end, m_parsedExpression, m_variablesMap) || m_variablesMap.size() > 1)
            {
                throw ExArithmeticExpressionSyntaxError();
            }
        }

        //noncopyable
        CCalcConversion(const ThisT&) = delete;
        CCalcConversion& operator=(const ThisT&) = delete;

    
        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                if (this->m_expression != pConversionRhs->m_expression)
                {
                    return false;
                }
                return true;
            }
            return false;
        }
    
    
        void modify(StringListT& textList) const override
        {
            if (m_parsedExpression)
            {
                for (StringT& text : textList)
                {
                    //parse text as expression
                    //attach expression from text as variable
                    if (!m_variablesMap.empty())
                    {
                        VariablesMapT variablesMapLocal;
                        SharedCalculationT parsedExpressionLocal;
                        IteratorT pos = text.cbegin();
                        IteratorT end = text.cend();
                        if (!parseExpression(pos, end, parsedExpressionLocal, variablesMapLocal) || !variablesMapLocal.empty())
                        {
                            throw ExArithmeticExpressionSyntaxError();
                        }
                        m_variablesMap.begin()->second->setResolvedExpression(parsedExpressionLocal);
                    }

                    //calculate
                    Value v = m_parsedExpression->calculate();
                    text = ToString<StringT>(v.data.intValue);

                    //detach expression from text as variable
                    if (!m_variablesMap.empty())
                    {
                        m_variablesMap.begin()->second->reset();
                    }
                }
            }
        }
    private:
        class Value
        {
        public:
            explicit Value(int64_t intValue_ = 0)
            {
                data.intValue = intValue_;
            }

            void applyAddTo(const Value& v)
            {
                data.intValue += v.data.intValue;
            }

            void applySubtract(const Value& v)
            {
                data.intValue -= v.data.intValue;
            }

            void applyMultiplyWith(const Value& v)
            {
                data.intValue *= v.data.intValue;
            }

            void applyDivideBy(const Value& v)
            {
                if (v.data.intValue == 0)
                {
                    throw ExDivisionByZero();
                }
                data.intValue /= v.data.intValue;
            }

            void applyModuloOperation(const Value& v)
            {
                if (v.data.intValue == 0)
                {
                    throw ExDivisionByZero();
                }
                data.intValue %= v.data.intValue;
            }

            Value& applyMinusSign()
            {
                data.intValue = -data.intValue;
                return *this;
            }

            union
            {
                int64_t intValue;
            }
            data;
        };

        class ICalculation
        {
        public:
            virtual Value calculate() const = 0;
            virtual ~ICalculation() = default;
        };

        typedef std::shared_ptr<ICalculation> SharedCalculationT;

        class Variable : public ICalculation
        {
        public:
            Variable() = default;
            Value calculate() const override
            {
                if (m_resolvedExpression)
                {
                    return m_resolvedExpression->calculate();
                }
                else
                {
                    throw ExCalculationError();
                }
            }
            void setResolvedExpression(SharedCalculationT resolvedExpression)
            {
                m_resolvedExpression = resolvedExpression;
            }
            void reset()
            {
                m_resolvedExpression.reset();
            }
            ~Variable() override = default;
            SharedCalculationT m_resolvedExpression;
        };

        typedef std::shared_ptr<Variable> SharedVariableT;
        typedef std::map<StringT, SharedVariableT> VariablesMapT;

        class IntegerValue : public ICalculation
        {
        public:
            explicit IntegerValue(int64_t intValue_ = 0)
                : intValue(intValue_)
            {
            }
            Value calculate() const override
            {
                return Value(intValue);
            }
            ~IntegerValue() override = default;
            int64_t intValue;
        };

        class Operation : public ICalculation
        {
        public:
            Value calculate() const override
            {
                return calculateImpl();
            }
            ~Operation() override = default;
            void addOperand(SharedCalculationT ptr)
            {
                assert(ptr);
                if (ptr)
                {
                    m_operands.push_back(ptr);
                }
            }
        protected:
            virtual Value calculateImpl() const = 0;
            std::vector<SharedCalculationT> m_operands;
        };
        typedef std::shared_ptr<Operation> SharedOperationT;

        typedef void (Value::*ApplyMemberT)(const Value&);
        template <ApplyMemberT applyMember>
        class OperationT : public Operation
        {
        public:
            explicit OperationT(SharedCalculationT ptr = SharedCalculationT())
            {
                this->addOperand(ptr);
            }
            ~OperationT() override = default;
        protected:
            Value calculateImpl() const override
            {
                assert(!this->m_operands.empty());
                if (!this->m_operands.empty())
                {
                    Value result = this->m_operands.front()->calculate();
                    for (auto it = (this->m_operands.cbegin() + 1); it != this->m_operands.cend(); ++it)
                    {
                        (result.*applyMember)((*it)->calculate());
                    }
                    return result;
                }
                throw ExCalculationError();
            }
        };
        typedef OperationT<&Value::applyAddTo> Addition;
        typedef OperationT<&Value::applySubtract> Subtraction;
        typedef OperationT<&Value::applyMultiplyWith> Multiplication;
        typedef OperationT<&Value::applyDivideBy> Division;
        typedef OperationT<&Value::applyModuloOperation> Modulo;

        class MinusSign : public Operation
        {
        public:
            explicit MinusSign(SharedCalculationT ptr = SharedCalculationT())
            {
                this->addOperand(ptr);
            }
            ~MinusSign() override = default;
        protected:
            Value calculateImpl() const override
            {
                assert(this->m_operands.size() == 1);
                if (this->m_operands.size() == 1)
                {
                    Value result = this->m_operands.front()->calculate();
                    result.applyMinusSign();
                    return result;
                }
                throw ExCalculationError();
            }
        };

        bool isWhiteSpace(CharT c) const
        {
            bool result = c == STRING_LITERAL(' ') || c == STRING_LITERAL('\t');
            return result;
        }

        void ignoreWhiteSpace(IteratorT& pos, IteratorT& end) const
        {
            for (;pos != end && isWhiteSpace(*pos); ++pos)
            {
                //nothing to do here
            }
        }

        //value; value with sign; variable; parentheses
        bool parseTerminal(IteratorT& posCurrent, IteratorT& end, SharedCalculationT& ptrOut, VariablesMapT& variablesMap) const
        {
            bool result = false;
            ignoreWhiteSpace(posCurrent, end);
            bool minusSign = false;
            //sign-------------------------------------------------------------
            if (posCurrent != end && *posCurrent == STRING_LITERAL('+')) //plus sign -> no op
            {
                ++posCurrent;
                ignoreWhiteSpace(posCurrent, end);
            }
            else if (posCurrent != end && *posCurrent == STRING_LITERAL('-')) //minus sign
            {
                ++posCurrent;
                minusSign = true;
                ignoreWhiteSpace(posCurrent, end);
            }
            //terminal---------------------------------------------------------
            if (posCurrent != end && *posCurrent == STRING_LITERAL('a')) //variable
            {
                StringT varName(posCurrent, posCurrent + 1);
                {
                    auto varpos = variablesMap.find(varName);
                    if (varpos != variablesMap.end())
                    {
                        ptrOut = varpos->second;
                    }
                    else
                    {
                        ptrOut = variablesMap[varName] = std::make_shared<Variable>();
                    }
                }
                if (minusSign)
                {
                    ptrOut = std::make_shared<MinusSign>(ptrOut);
                }
                ++posCurrent;
                result = true;
            }
            else if (posCurrent != end && *posCurrent >= STRING_LITERAL('0') && *posCurrent <= STRING_LITERAL('9')) //number, first digit
            {
                int64_t resultValue = (*posCurrent - STRING_LITERAL('0'));
                ++posCurrent;
                while (resultValue /*skip next if first is 0 */ && posCurrent != end && *posCurrent >= STRING_LITERAL('0') && *posCurrent <= STRING_LITERAL('9')) //following digits
                {
                    //note: overflow is not handled
                    resultValue *= 10;
                    resultValue += (*posCurrent - STRING_LITERAL('0'));
                    ++posCurrent;
                }
                result = true;
                ptrOut = std::make_shared<IntegerValue>(minusSign ? -resultValue : resultValue);
            }
            else if (posCurrent != end && *posCurrent == STRING_LITERAL('(')) //parentheses
            {
                ++posCurrent;
                if (parseOperationOrTerminal(posCurrent, end, ptrOut, variablesMap))
                {
                    ignoreWhiteSpace(posCurrent, end);
                    if (posCurrent != end && *posCurrent == STRING_LITERAL(')'))
                    {
                        if (minusSign)
                        {
                            ptrOut = std::make_shared<MinusSign>(ptrOut);
                        }
                        ++posCurrent;
                        result = true;
                    }
                }
            }

            return result;
        }

        enum EOperation
        {
            Operation_Addition,
            Operation_Subtraction,
            Operation_Multiplication,
            Operation_Division,
            Operation_Modulo,
            Operator_Unknown
        };

        int getPrecedence(EOperation op) const
        {
            if (op == Operation_Addition || op == Operation_Subtraction)
            {
                return 0;
            }
            return 1;
        }

        bool parseOperator(IteratorT& posCurrent, IteratorT& end, EOperation& operatorOut) const
        {
            bool result = false;
            operatorOut = Operator_Unknown;
            ignoreWhiteSpace(posCurrent, end);
            if (posCurrent != end && (*posCurrent == STRING_LITERAL('+')))
            {
                operatorOut = Operation_Addition;
            }
            else if (posCurrent != end && (*posCurrent == STRING_LITERAL('-')))
            {
                operatorOut = Operation_Subtraction;
            }
            else if (posCurrent != end && (*posCurrent == STRING_LITERAL('*')))
            {
                operatorOut = Operation_Multiplication;
            }
            else if (posCurrent != end && (*posCurrent == STRING_LITERAL('/')))
            {
                operatorOut = Operation_Division;
            }
            else if (posCurrent != end && (*posCurrent == STRING_LITERAL('%')))
            {
                operatorOut = Operation_Modulo;
            }

            if (operatorOut != Operator_Unknown)
            {
                ++posCurrent;
                result = true;
            }
            return result;
        }

        SharedOperationT makeOperation(EOperation operatorIn, SharedCalculationT ptr = SharedCalculationT()) const
        {
            if (operatorIn == Operation_Addition)
            {
                return std::make_shared<Addition>(ptr);
            }
            else if (operatorIn == Operation_Subtraction)
            {
                return std::make_shared<Subtraction>(ptr);
            }
            else if (operatorIn == Operation_Multiplication)
            {
                return std::make_shared<Multiplication>(ptr);
            }
            else if (operatorIn == Operation_Division)
            {
                return std::make_shared<Division>(ptr);
            }
            else if (operatorIn == Operation_Modulo)
            {
                return std::make_shared<Modulo>(ptr);
            }
            assert(false);
            throw ExArithmeticExpressionSyntaxError();
        }

        bool parseOperationFromFirstOperator(const EOperation myOperator, IteratorT& posCurrent, IteratorT& end, SharedCalculationT& ptrInOut, VariablesMapT& variablesMap) const
        {
            bool result = false;
            //note: ptrInOut contains the first operand
            //make corresponding operation
            SharedOperationT ptrOperation = makeOperation(myOperator, ptrInOut);
            bool skipParseTerminal = false; //operand is a subexpression
            while (true)
            {
                //parse next terminal value
                if (skipParseTerminal || parseTerminal(posCurrent, end, ptrInOut, variablesMap))
                {
                    //get next operator if any
                    EOperation nextOperator = Operator_Unknown;
                    IteratorT preOperatorPos = posCurrent;
                    if (parseOperator(posCurrent, end, nextOperator))
                    {
                        if (nextOperator == myOperator)
                        {
                            //same operator; add; read next terminal value
                            ptrOperation->addOperand(ptrInOut);
                            continue;
                        }
                        else if (getPrecedence(nextOperator) > getPrecedence(myOperator))
                        {
                            //operator of higher precedence follows, start parsing, current terminal value belongs to operator
                            //result is the subexpression in ptrInOut
                            if (!parseOperationFromFirstOperator(nextOperator, posCurrent, end, ptrInOut, variablesMap))
                            {
                                break; //failure
                            }
                            skipParseTerminal = true;
                            continue;
                        }
                        else if (getPrecedence(nextOperator) <= getPrecedence(myOperator))
                        {
                            //success: operator of same or lower precedence follows
                            ptrOperation->addOperand(ptrInOut);
                            ptrInOut = ptrOperation;
                            posCurrent = preOperatorPos;
                            result = true;
                            break;
                        }
                    }
                    else
                    {
                        //success: no more operators found, consider done
                        ptrOperation->addOperand(ptrInOut);
                        ptrInOut = ptrOperation;
                        result = true;
                        break;
                    }
                }
                else
                {
                    //failure: expected terminal value not found -> exit
                    break;
                }
            }
            return result;
        }

        bool parseOperationOrTerminal(IteratorT& posCurrent, IteratorT& end, SharedCalculationT& ptrOut, VariablesMapT& variablesMap) const
        {
            bool result = true;
            //parse first terminal value
            if (parseTerminal(posCurrent, end, ptrOut, variablesMap))
            {
                //loop: get next operator if any
                EOperation nextOperator = Operator_Unknown;
                while(parseOperator(posCurrent, end, nextOperator))
                {
                    if (parseOperationFromFirstOperator(nextOperator, posCurrent, end, ptrOut, variablesMap))
                    {
                        continue;
                    }
                    else
                    {
                        //failure: parsing operation failed
                        result = false;
                        break;
                    }
                }
                //no more operators -> done (or failure)
            }
            else
            {
                //failure: expected terminal value not found -> exit
                result = false;
            }
            return result;
        }

        bool parseExpression(IteratorT& posCurrent, IteratorT& end, SharedCalculationT& ptrOut, VariablesMapT& variablesMap) const
        {
            bool result = false;
            ignoreWhiteSpace(posCurrent, end);
            if (parseOperationOrTerminal(posCurrent, end, ptrOut, variablesMap))
            {
                ignoreWhiteSpace(posCurrent, end);
                result = (posCurrent == end);
            }
            return result;
        }

        VariablesMapT m_variablesMap;
        SharedCalculationT m_parsedExpression;
        StringT m_expression;
    };


    template <typename StringT>
    class CToCsvConversion : public ConversionDirectives<StringT>
    {
        typedef typename StringT::value_type CharT;
    public:
        explicit CToCsvConversion(
            const StringT& csvDelimiterChars
        )
            : m_csvDelimiterChars(csvDelimiterChars)
            , m_csvQuoteChars(STRING_LITERAL("\""))
        {
        }

        CToCsvConversion(
            const StringT& csvDelimiterChars,
            const StringT& csvQuoteChars
        )
            : m_csvDelimiterChars(csvDelimiterChars)
            , m_csvQuoteChars(csvQuoteChars)
        {
        }

        typedef CToCsvConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                if (this->m_csvDelimiterChars != pConversionRhs->m_csvDelimiterChars
                    || this->m_csvQuoteChars != pConversionRhs->m_csvQuoteChars
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        void toCsv(StringT& text) const
        {
            const CharT new_line = STRING_LITERAL('\n');
        
            //check if the text contains a delimiter character
            bool textNeedsToBePutInQuotes = false;
            for (CharT c : text)
            {
                if (c == new_line)
                {
                    textNeedsToBePutInQuotes = true;
                    break;
                }
                for (CharT d : m_csvDelimiterChars)
                {
                    if (c == d)
                    {
                        textNeedsToBePutInQuotes = true;
                        break;
                    }
                }
                for (CharT q : m_csvQuoteChars)
                {
                    if (c == q)
                    {
                        textNeedsToBePutInQuotes = true;
                        break;
                    }
                }
                if (textNeedsToBePutInQuotes)
                {
                    break;
                }
            }

            //if the text needs to be put in quotes
            if (textNeedsToBePutInQuotes)
            {
                StringT result;
                CharT quote = *m_csvQuoteChars.begin();
                result.reserve(text.size() + 2); //+2 -> left and right quote char
                result += quote;
                for (CharT c : text)
                {
                    if (c == quote)
                    {
                        //quotes need to be doubled in this case
                        result += c;
                    }
                    result += c;
                }
                result += quote;
                text.swap(result);
            }
        }

        void modify(StringListT& textList) const override
        {
            if (!m_csvQuoteChars.empty())
            {
                for (StringT& text : textList)
                {
                    toCsv(text);
                }
            }
        }

    private:
        const StringT m_csvDelimiterChars; //The first character specifies the delimiter for the next CSV-files to load. Cannot use 'tab' for tab separated items. Use an empty string for no delimiter.
        const StringT m_csvQuoteChars; //Specifies a list of characters as string that are used for quoting text items in CSV-files. The default is the double quote character.
    };


    ///defines exceptions thrown by CToSizeConversion for template argument independent access
    class ToSizeConversionExceptions
    {
    public:
        class ExUnexpectedToSizeProperty : public std::runtime_error
        {
        public: ExUnexpectedToSizeProperty() : std::runtime_error("Unexpected property found for to size conversion.") {}
        };
    };


    template <typename StringT>
    class CToSizeConversion : public ConversionDirectives<StringT>, public ToSizeConversionExceptions
    {
        typedef typename StringT::value_type CharT;

        enum ERequestedSizeType
        {
            RequestedSizeType_ArrayElements //depends on encoding and string type
            //RequestedSizeType_Chars //depends on encoding
        };

    public:
        explicit CToSizeConversion(
            const StringT& properties
        )
            : m_requestedSizeType(RequestedSizeType_ArrayElements)
        {
            if (properties != STRING_LITERAL("array-elements"))
            {
                throw ExUnexpectedToSizeProperty();
            }
        }

        typedef CToSizeConversion<StringT> ThisT;
        typedef typename IConversion<StringT>::StringListT StringListT;

        bool operator==(const IConversion<StringT>& conversion) const override
        {
            auto pConversionRhs = dynamic_cast<const ThisT*>(&conversion);
            if (pConversionRhs)
            {
                if (this->m_requestedSizeType != pConversionRhs->m_requestedSizeType
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        StringT toSize(StringT& text) const
        {
            StringT result = ToString<StringT>(text.size());
            return result;
        }

        void modify(StringListT& textList) const override
        {
            for (StringT& text : textList)
            {
                text = toSize(text);
            }
        }

    private:
        const ERequestedSizeType m_requestedSizeType;
    };
}
