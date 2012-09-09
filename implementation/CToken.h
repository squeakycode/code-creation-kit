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

#ifndef INCLUDED_CTOKEN_H_3732416
#define INCLUDED_CTOKEN_H_3732416

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "CComparableSharedObject.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/foreach.hpp>

template <typename ETokenHolderT, typename StringT>
class CToken : public ETokenHolderT
{
public:
    typedef typename ETokenHolderT::token_type ETokenT;
    typedef std::vector<StringT> StringListT;
    typedef boost::shared_ptr<StringListT> SharedStringListT;
    typedef boost::shared_ptr<const StringListT> ConstSharedStringListT;

    CToken()
        : m_token((ETokenT)0)
    {
    }

    CToken( ETokenT token)
        : m_token( token)
    {
    }

    CToken( ETokenT token, SharedStringListT stringList)
        : m_token( token)
        , m_stringList( stringList)
    {
    }

    CToken( ETokenT token, const StringT& textA)
        : m_token( token)
        , m_stringList( new StringListT)
    {
        m_stringList->push_back( textA);
    }

    CToken( ETokenT token, const StringT& textA, const StringT& textB)
        : m_token( token)
        , m_stringList( new StringListT)
    {
        m_stringList->push_back( textA);
        m_stringList->push_back( textB);        
    }

    CToken( const CToken<ETokenHolderT, StringT>& rhs)
    {
        m_token = rhs.m_token;
        m_stringList = rhs.m_stringList;
    }

    CToken<ETokenHolderT, StringT>& operator = ( const CToken<ETokenHolderT, StringT>& rhs)
    {
        m_token = rhs.m_token;
        m_stringList = rhs.m_stringList;
        return *this;
    }

    bool operator == ( ETokenT token) const
    {
        return m_token == token;
    }

    bool operator != ( ETokenT token) const
    {
        return m_token != token;
    }

    bool operator == ( const CToken<ETokenHolderT, StringT>& rhs) const
    {
        if (   m_token != rhs.m_token
            || m_stringList != rhs.m_stringList
            )
        {
            return false;
        }
        return true;
    }


    ETokenT getToken() const
    {
        return m_token;
    }

    ConstSharedStringListT getStringList() const
    {
        return m_stringList;
    }

    size_t getTextSize() const
    {
        if ( m_stringList)
        {
            size_t result = 0;
            const StringListT& textList = *m_stringList;
            BOOST_FOREACH( const StringT& text, textList)
            {
                result += text.size();
            }
            return result;
        }
        return 0;
    }

    template <typename StreamT>
    void toStream( StreamT& stream) const
    {
        if ( m_stringList)
        {
            const StringListT& textList = *m_stringList;
            BOOST_FOREACH( const StringT& text, textList)
            {
                stream << text;
            }
        }
    }

public:
    ETokenT m_token;
    CComparableSharedObject<StringListT> m_stringList;
};

#endif /* INCLUDED_CTOKEN_H_3732416 */
