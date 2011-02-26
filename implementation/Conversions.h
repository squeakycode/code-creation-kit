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

#endif /* INCLUDED_CONVERSIONS_H_8639185 */
