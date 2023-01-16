//  Copyright (c) 2011-2023 Andreas Gau
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

#pragma once

#include "ConstraintDirectives.gen.h"

#include <stdexcept>
#include <regex>
#include <cppstringx.hpp>

namespace code_creation_kit
{
    template <typename StringT>
    class CConstraintBase : public ConstraintDirectives<StringT>
    {
    public:
        CConstraintBase()
            : m_flush(false)
            , m_forAll(false)
            , m_not_(false)
        {
        }

        ///flush
        virtual void flush( bool flush) 
        { 
            m_flush = flush; 
        }
        virtual bool flush() const { return m_flush; }

        ///forAll
        virtual void forAll( bool forAll) 
        { 
            m_forAll = forAll; 
        }
        virtual bool forAll() const { return m_forAll; }

        ///forAll
        virtual void not_( bool not_) 
        { 
            m_not_ = not_;
        }
        virtual bool not_() const { return m_not_; }

        bool operator == ( const CConstraintBase<StringT>& rhs)
        {
            if ( 
                m_flush != rhs.m_flush
                || m_forAll != rhs.m_forAll
                || m_not_ != rhs.m_not_
                )
            {
                return false;
            }
            return true;
        }

    protected:
        bool m_flush;
        bool m_forAll;
        bool m_not_;
    };


    template <typename StringT>
    class CMatchesConstraint : public CConstraintBase<StringT>
    {
    public:
        typedef CMatchesConstraint<StringT> ThisT;
        using CConstraintBase<StringT>::m_not_;

        CMatchesConstraint( const StringT& matchesText)
            : m_ignoreCase( false)
            , m_matches( matchesText)
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

        virtual bool operator==( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            if ( m)
            {
                if ( !(((CConstraintBase<StringT>&)*this) == ((CConstraintBase<StringT>&)*m))
                    || m_ignoreCase != m->m_ignoreCase
                    || m_matches != m->m_matches
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        virtual bool matchesConstraint( const StringT& text) const
        {
            if ( m_ignoreCase)
            {
                return cppstringx::iequals( m_matches, text) != m_not_;
            }
            else
            {
                return (m_matches == text) != m_not_;
            }
        }
    private:
        bool    m_ignoreCase;
        StringT m_matches;
    };

    template <typename StringT>
    class CStartsWithConstraint : public CConstraintBase<StringT>
    {
    public:
        typedef CStartsWithConstraint<StringT> ThisT;
        using CConstraintBase<StringT>::m_not_;

        CStartsWithConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        virtual bool operator==( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            if ( m)
            {
                if ( !(((CConstraintBase<StringT>&)*this) == ((CConstraintBase<StringT>&)*m))
                    || m_ignoreCase != m->m_ignoreCase
                    || m_testText != m->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        virtual bool matchesConstraint( const StringT& text) const
        {
            if ( m_ignoreCase)
            {
                return cppstringx::istarts_with( text, m_testText) != m_not_;
            }
            else
            {
                return cppstringx::starts_with( text, m_testText) != m_not_;
            }
        }
    private:
        bool    m_ignoreCase;
        StringT m_testText;
    };

    template <typename StringT>
    class CEndsWithConstraint : public CConstraintBase<StringT>
    {
    public:
        typedef CEndsWithConstraint<StringT> ThisT;
        using CConstraintBase<StringT>::m_not_;

        CEndsWithConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        virtual bool operator==( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            if ( m)
            {
                if ( !(((CConstraintBase<StringT>&)*this) == ((CConstraintBase<StringT>&)*m))
                    || m_ignoreCase != m->m_ignoreCase
                    || m_testText != m->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        virtual bool matchesConstraint( const StringT& text) const
        {
            if ( m_ignoreCase)
            {
                return cppstringx::iends_with( text, m_testText) != m_not_;
            }
            else
            {
                return cppstringx::ends_with( text, m_testText) != m_not_;
            }
        }
    private:
        bool    m_ignoreCase;
        StringT m_testText;
    };

    template <typename StringT>
    class CContainsConstraint : public CConstraintBase<StringT>
    {
    public:
        typedef CContainsConstraint<StringT> ThisT;
        using CConstraintBase<StringT>::m_not_;

        CContainsConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        virtual bool operator==( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            if ( m)
            {
                if ( !(((CConstraintBase<StringT>&)*this) == ((CConstraintBase<StringT>&)*m))
                    || m_ignoreCase != m->m_ignoreCase
                    || m_testText != m->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        virtual bool matchesConstraint( const StringT& text) const
        {
            if ( m_ignoreCase)
            {
                return cppstringx::icontains( text, m_testText) != m_not_;
            }
            else
            {
                return cppstringx::contains( text, m_testText) != m_not_;
            }
        }
    private:
        bool    m_ignoreCase;
        StringT m_testText;
    };

    ///defines exceptions thrown by CRegexMatchesConstraint for template argument independent access
    class CRegexMatchesConstraintExceptions
    {
    public:
        class ExRegexSyntaxError : public std::runtime_error
        { public: ExRegexSyntaxError() : std::runtime_error( "Syntax error in regular expression") {}};
    };

    template <typename StringT>
    class CRegexMatchesConstraint : public CConstraintBase<StringT>, public CRegexMatchesConstraintExceptions
    {
    public:
        typedef std::basic_regex<typename StringT::value_type, std::regex_traits<typename StringT::value_type> > RegexT;
        typedef CRegexMatchesConstraint<StringT> ThisT;
        using CConstraintBase<StringT>::m_not_;

        CRegexMatchesConstraint( const StringT& matchesText)
            : m_ignoreCase( false)
            , m_matches( matchesText)
        {
            try
            {
                m_regexMatches = RegexT( m_matches);
                m_regexMatchesIgnoreCase = RegexT( m_matches, std::regex::icase);
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

        virtual bool operator == ( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            if ( m)
            {
                if ( !(((CConstraintBase<StringT>&)*this) == ((CConstraintBase<StringT>&)*m))
                    || m_ignoreCase != m->m_ignoreCase
                    || m_matches != m->m_matches
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        virtual bool matchesConstraint( const StringT& text) const
        {
            if ( m_ignoreCase)
            {
                return regex_match( text, m_regexMatchesIgnoreCase)  != m_not_;
            }
            else
            {
                return regex_match( text, m_regexMatches)  != m_not_;
            }
        }

    private:
        bool m_ignoreCase;
        StringT m_matches;
        StringT m_with;
        RegexT m_regexMatches;
        RegexT m_regexMatchesIgnoreCase;
    };

    template <typename StringT>
    class CAnyConstraint : public ConstraintDirectives<StringT>
    {
    public:
        typedef CAnyConstraint<StringT> ThisT;

        virtual bool operator==( const IConstraint<StringT>& constraint) const
        {
            const ThisT* m = dynamic_cast<const ThisT*>(&constraint);
            return m != 0;
        }

        virtual bool forAll() const
        {
            return false;
        }

        virtual bool flush() const
        {
            return true;
        }

        void not_(bool)
        {
        }

        virtual bool matchesConstraint( const StringT&) const
        {
            return true;
        }
    };
}
