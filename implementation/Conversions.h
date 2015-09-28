//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_CONVERSIONS_H_8639185
#define INCLUDED_CONVERSIONS_H_8639185

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "ConversionDirectives.gen.h"
#include "StringLiteral.h"
#include <boost/foreach.hpp>
#include <stdexcept>
#include <boost/regex.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

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

    virtual void ignoreCase( bool ignoreCase) 
    { 
        m_ignoreCase = ignoreCase; 
    }

    virtual bool ignoreCase() const
    { 
        return m_ignoreCase;
    }

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
        {
            if ( 
                m_ignoreCase != m->m_ignoreCase
                || m_replace != m->m_replace
                || m_with != m->m_with
                )
            {
                return false;
            }
            return true;
        }
        return false;
    }

    virtual void modify( StringListT& textList) const
    {
        BOOST_FOREACH( StringT& text, textList)
        {
            if ( m_ignoreCase)
            {
                boost::ireplace_all( text, m_replace, m_with);
            }
            else
            {
                boost::replace_all( text, m_replace, m_with);
            }
        }
    }
private:
    bool    m_ignoreCase;
    StringT m_replace;
    StringT m_with;
};

///defines exceptions thrown by CRegexReplaceConversion for template argument independent access
class CRegexReplaceConversionExceptions
{
public:
    class ExRegexSyntaxError : public std::runtime_error
    { public: ExRegexSyntaxError() : std::runtime_error( "Syntax error in regular expression") {}};
};

template <typename StringT>
class CRegexReplaceConversion : public ConversionDirectives<StringT>, public CRegexReplaceConversionExceptions
{
public:
    typedef boost::basic_regex<typename StringT::value_type, boost::regex_traits<typename StringT::value_type> > RegexT;
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
            m_regexReplaceIgnoreCase = RegexT( m_replace, boost::regex::icase);
        }
        catch(...)
        {
            throw ExRegexSyntaxError();
        }
    }

    virtual void ignoreCase( bool ignoreCase) 
    { 
        m_ignoreCase = ignoreCase; 
    }

    virtual bool ignoreCase() const
    { 
        return m_ignoreCase;
    }

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
        {
            if ( 
                m_ignoreCase != m->m_ignoreCase
                || m_replace != m->m_replace
                || m_with != m->m_with
                )
            {
                return false;
            }
            return true;
        }
        return false;
    }

    virtual void modify( StringListT& textList) const
    {
        BOOST_FOREACH( StringT& text, textList)
        {
            if ( m_ignoreCase)
            {
                text = boost::regex_replace( text, m_regexReplaceIgnoreCase, m_with);
            }
            else
            {
                text = boost::regex_replace( text, m_regexReplace, m_with);
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

    CMergeConversion( const StringT& separator)
        : m_separator( separator)
    {
    }

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
        {
            if ( 
                m_separator != m->m_separator
                )
            {
                return false;
            }
            return true;
        }
        return false;
    }

    virtual void modify( StringListT& textList) const
    {
        StringT merged;
        bool first = true;
        BOOST_FOREACH( StringT& text, textList)
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

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
        {
            return true;
        }
        return false;
    }

    virtual void modify( StringListT& textList) const
    {
        BOOST_FOREACH( StringT& text, textList)
        {
            boost::to_lower( text);
        }
    }
};

template <typename StringT>
class CToUpperConversion : public ConversionDirectives<StringT>
{
public:
    typedef CToUpperConversion<StringT> ThisT;
    typedef typename IConversion<StringT>::StringListT StringListT;

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
        {
            return true;
        }
        return false;
    }

    virtual void modify( StringListT& textList) const
    {
        BOOST_FOREACH( StringT& text, textList)
        {
            boost::to_upper( text);
        }
    }
};


template <typename StringT>
class CToCStringConversion : public ConversionDirectives<StringT>
{
public:
    typedef CToUpperConversion<StringT> ThisT;
    typedef typename IConversion<StringT>::StringListT StringListT;

    virtual bool operator==( const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if ( m)
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
        BOOST_FOREACH( CharT c, text)
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

    virtual void modify( StringListT& textList) const
    {
        BOOST_FOREACH( StringT& text, textList)
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

    virtual bool operator==(const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if (m)
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
                {'<' , STRING_LITERAL("&lt")  },
                {'>' , STRING_LITERAL("&gt")  },
                {'"' , STRING_LITERAL("&quot")},
                {'\'', NULL   },
                {'`' , NULL   },
                {'(' , NULL   },
                {')' , NULL   },
                {'{' , NULL   },
                {'}' , NULL   },
                {'[' , NULL   },
                {']' , NULL   },
                {'!' , NULL   },
                {'@' , NULL   },
                {'$' , NULL   },
                {'%' , NULL   },
                {'=' , NULL   },
                {'+' , NULL   },
                {  0 , NULL   }
            };

        // iterate all chars of text
        StringT result;
        BOOST_FOREACH(CharT c, text)
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
                        result += boost::lexical_cast<StringT>((int)c);
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

    virtual void modify(StringListT& textList) const
    {
        BOOST_FOREACH(StringT& text, textList)
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
        , const StringT& padUpToWidthFirst
        , const IteratorT& padUpToWidthOptionalBegin
        , const IteratorT& padUpToWidthOptionalEnd
    )
    {
        m_padText = padText;
        m_padUpToWidths.reserve(1 + (padUpToWidthOptionalEnd == padUpToWidthOptionalBegin ? 0 : padUpToWidthOptionalEnd - padUpToWidthOptionalBegin));
        m_padUpToWidths.push_back(boost::lexical_cast<size_t>(padUpToWidthFirst));
        for (IteratorT it = padUpToWidthOptionalBegin; it != padUpToWidthOptionalEnd; ++it)
        {
            m_padUpToWidths.push_back(boost::lexical_cast<size_t>(*it));
        }
        
        //remove unsupported chars
        const CharT newLine = STRING_LITERAL('\n');
        const CharT cr = STRING_LITERAL('\r');
        const CharT tab = STRING_LITERAL('\t');
        for (typename StringT::iterator it = m_padText.begin(); it != m_padText.end();)
        {
            if (*it == newLine || *it == tab || *it == cr)
            {
                it = m_padText.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }


    void addMultipliedTextPadding(StringT& result, size_t padCharsNeeded) const
    {
        const size_t max = padCharsNeeded / m_padText.size();
        for (size_t i = 0; i < max; ++i)
        {
            result += m_padText;
        }
        const size_t rest = padCharsNeeded % m_padText.size();
        if (rest)
        {
            result.append(m_padText.begin(), m_padText.begin() + rest);
        }
    }


    StringT pad(const StringT& text, bool padLeft) const
    {
        StringT result;
        const CharT newLine = STRING_LITERAL('\n');
        const CharT tab = STRING_LITERAL('\t');
        size_t lineWidth = 0;
        size_t lineNumber = 0;
        for (typename StringT::const_iterator it = text.begin(), lineStart = text.begin();;++it)
        {
            if (it == text.end() || *it == newLine)
            {
                //get number of chars to pad up to
                size_t padUpToWidth = 0;
                if (lineNumber < m_padUpToWidths.size())
                {
                    padUpToWidth = m_padUpToWidths[lineNumber];
                }
                else
                {
                    padUpToWidth = m_padUpToWidths.back();
                }

                //padding needed?
                if (lineWidth < padUpToWidth && !m_padText.empty())
                {
                    size_t padCharsNeeded = padUpToWidth - lineWidth;
                    result.reserve(result.size() + padCharsNeeded);

                    if (m_padText.size() == 1)
                    {
                        if (padLeft)
                        {
                            result.append(padCharsNeeded, m_padText[0]);
                        }
                        result.append(lineStart, it);
                        if (!padLeft)
                        {
                            result.append(padCharsNeeded, m_padText[0]);
                        }
                    }
                    else
                    {
                        if (padLeft)
                        {
                            addMultipliedTextPadding(result, padCharsNeeded);
                        }
                        result.append(lineStart, it);
                        if (!padLeft)
                        {
                            addMultipliedTextPadding(result, padCharsNeeded);
                        }
                    }
                }
                else
                {
                    result.append(lineStart, it);
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

    StringT m_padText;
    std::vector<size_t> m_padUpToWidths;
    static const size_t cTabSize = 4;
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
        : CPadConversionBase<StringT>(padText, padWidthFirst, padWidthOptionalBegin, padWidthOptionalEnd)
    {
    }

    typedef CPadLeftConversion<StringT> ThisT;
    typedef typename IConversion<StringT>::StringListT StringListT;

    virtual bool operator==(const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if (m)
        {
            return true;
        }
        return false;
    }

    virtual void modify(StringListT& textList) const
    {
        if (!this->m_padText.empty())
        {
            BOOST_FOREACH(StringT& text, textList)
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
        : CPadConversionBase<StringT>(padText, padWidthFirst, padWidthOptionalBegin, padWidthOptionalEnd)
    {
    }

    typedef CPadRightConversion<StringT> ThisT;
    typedef typename IConversion<StringT>::StringListT StringListT;

    virtual bool operator==(const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if (m)
        {
            return true;
        }
        return false;
    }

    virtual void modify(StringListT& textList) const
    {
        if (!this->m_padText.empty())
        {
            BOOST_FOREACH(StringT& text, textList)
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
    CBlockFormatConversion(const StringT& blockWidth)
    {
        m_blockWidth = boost::lexical_cast<size_t>(blockWidth);
    }

    typedef CBlockFormatConversion<StringT> ThisT;
    typedef typename IConversion<StringT>::StringListT StringListT;

    virtual bool operator==(const IConversion<StringT>& conversion) const
    {
        const ThisT* m = dynamic_cast<const ThisT*>(&conversion);
        if (m)
        {
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
        for (;it != text.end();)
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

                    if (it != text.end() && *it == space || *it == tab)
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

    virtual void modify(StringListT& textList) const
    {
        if (m_blockWidth)
        {
            BOOST_FOREACH(StringT& text, textList)
            {
                text = blockFormat(text);
            }
        }
    }

private:
    size_t m_blockWidth;
    static const size_t cTabSize = 4;
};


#endif /* INCLUDED_CONVERSIONS_H_8639185 */
