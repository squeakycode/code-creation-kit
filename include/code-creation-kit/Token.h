// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "ComparableSharedObject.h"
#include <memory>
#include <vector>

namespace code_creation_kit
{
    template <typename ETokenHolderT, typename StringT>
    class CToken : public ETokenHolderT
    {
    public:
        typedef typename StringT::value_type CharT;
        typedef typename ETokenHolderT::token_type ETokenT;
        typedef std::vector<StringT> StringListT;
        typedef std::shared_ptr<StringListT> SharedStringListT;
        typedef std::shared_ptr<const StringListT> ConstSharedStringListT;

        CToken()
            : m_token(static_cast<ETokenT>(0))
        {
        }

        explicit CToken( ETokenT token)
            : m_token( token)
        {
        }

        CToken( ETokenT token, SharedStringListT stringList)
            : m_token( token)
            , m_stringList( stringList)
        {
        }

        CToken( ETokenT token, SharedStringListT stringList, SharedStringListT sourceText)
            : m_token( token)
            , m_stringList( stringList)
            , m_sourceText( sourceText)
        {
        }

        CToken( ETokenT token, const StringT& textA)
            : m_token( token)
            , m_stringList(std::make_shared<StringListT>(1))
        {
            m_stringList->front() = textA;
        }

        CToken( ETokenT token,  const typename StringT::const_iterator& start,  const typename StringT::const_iterator& end)
            : m_token( token)
            , m_stringList(std::make_shared<StringListT>(1))
        {
            m_stringList->back().assign( start, end);
        }

        CToken( ETokenT token, const StringT& textA, const StringT& textB)
            : m_token( token)
            , m_stringList(std::make_shared<StringListT>(2))
        {
            m_stringList->front() = textA;
            m_stringList->back() = textB;
        }

        CToken( ETokenT token, const CharT* textA, const CharT* textB)
                : m_token( token)
                , m_stringList(std::make_shared<StringListT>(2))
        {
            m_stringList->front() = textA;
            m_stringList->back() = textB;
        }

        CToken( const CToken<ETokenHolderT, StringT>& rhs)
            : m_token( rhs.m_token)
            , m_stringList( rhs.m_stringList)
            , m_sourceText( rhs.m_sourceText)
        {
        }

        CToken<ETokenHolderT, StringT>& operator = ( const CToken<ETokenHolderT, StringT>& rhs)
        {
            m_token = rhs.m_token;
            m_stringList = rhs.m_stringList;
            m_sourceText = rhs.m_sourceText;
            return *this;
        }

        CToken<ETokenHolderT, StringT> cloneChangingParameter(StringT parameter0) const
        {
            CToken<ETokenHolderT, StringT> clone;
            clone.m_token = m_token;
            clone.m_stringList = std::make_shared<StringListT>(*m_stringList);
            clone.m_sourceText = m_sourceText;
            if (!clone.m_stringList->empty())
            {
                clone.m_stringList->front() = parameter0;
            }
            else
            {
                clone.m_stringList->push_back(parameter0);
            }
            return clone;
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

        ConstSharedStringListT getSourceTextList() const
        {
            return m_sourceText;
        }



        [[nodiscard]] size_t getTextSize() const
        {
            if ( m_stringList)
            {
                size_t result = 0;
                const StringListT& textList = *m_stringList;
                for (const StringT& text : textList)
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
                for (const StringT& text : textList)
                {
                    stream << text;
                }
            }
        }

        template <typename StreamT>
        void sourceTextToStream( StreamT& stream) const
        {
            if ( m_sourceText || m_stringList)
            {
                const StringListT& textList = *(m_sourceText ? m_sourceText : m_stringList);
                for (const StringT& text : textList)
                {
                    stream << text;
                }
            }
        }

    private:
        ETokenT m_token;
        CComparableSharedObject<StringListT> m_stringList;
        CComparableSharedObject<StringListT> m_sourceText;
    };
}
