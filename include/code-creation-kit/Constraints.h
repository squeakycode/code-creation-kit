// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "ConstraintDirectives.gen.h"

#include <stdexcept>
#include <regex>
#include <cppstringx.hpp>

namespace code_creation_kit
{
    template <typename StringT>
    class ConstraintBase : public ConstraintDirectives<StringT>
    {
    public:
        ConstraintBase()
            : m_flush(false)
            , m_forAll(false)
            , m_not_(false)
        {
        }

        ///flush
        void flush( bool flush) override
        {
            m_flush = flush; 
        }
        [[nodiscard]] bool flush() const override { return m_flush; }

        ///forAll
        void forAll( bool forAll) override
        {
            m_forAll = forAll; 
        }
        [[nodiscard]] bool forAll() const override { return m_forAll; }

        ///forAll
        virtual void not_( bool not_)
        {
            m_not_ = not_;
        }
        [[nodiscard]] virtual bool not_() const { return m_not_; }

        bool equalBaseProperties( const ConstraintBase<StringT>& rhs) const
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
    class MatchesConstraint : public ConstraintBase<StringT>
    {
    public:
        typedef MatchesConstraint<StringT> ThisT;
        using ConstraintBase<StringT>::m_not_;

        explicit MatchesConstraint( const StringT& matchesText)
            : m_ignoreCase( false)
            , m_matches( matchesText)
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

        bool operator==( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            if (pRhs)
            {
                if ( !this->equalBaseProperties(*pRhs)
                    || m_ignoreCase != pRhs->m_ignoreCase
                    || m_matches != pRhs->m_matches
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool matchesConstraint( const StringT& text) const override
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
    class StartsWithConstraint : public ConstraintBase<StringT>
    {
    public:
        typedef StartsWithConstraint<StringT> ThisT;
        using ConstraintBase<StringT>::m_not_;

        explicit StartsWithConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        bool operator==( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            if (pRhs)
            {
                if ( !this->equalBaseProperties(*pRhs)
                    || m_ignoreCase != pRhs->m_ignoreCase
                    || m_testText != pRhs->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool matchesConstraint( const StringT& text) const override
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
    class EndsWithConstraint : public ConstraintBase<StringT>
    {
    public:
        typedef EndsWithConstraint<StringT> ThisT;
        using ConstraintBase<StringT>::m_not_;

        explicit EndsWithConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        bool operator==( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            if (pRhs)
            {
                if ( !this->equalBaseProperties(*pRhs)
                    || m_ignoreCase != pRhs->m_ignoreCase
                    || m_testText != pRhs->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool matchesConstraint( const StringT& text) const override
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
    class ContainsConstraint : public ConstraintBase<StringT>
    {
    public:
        typedef ContainsConstraint<StringT> ThisT;
        using ConstraintBase<StringT>::m_not_;

        explicit ContainsConstraint( const StringT& testText)
            : m_ignoreCase( false)
            , m_testText( testText)
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

        bool operator==( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            if (pRhs)
            {
                if ( !this->equalBaseProperties(*pRhs)
                    || m_ignoreCase != pRhs->m_ignoreCase
                    || m_testText != pRhs->m_testText
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool matchesConstraint( const StringT& text) const override
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

    ///defines exceptions thrown by RegexMatchesConstraint for template argument independent access
    class RegexMatchesConstraintExceptions
    {
    public:
        class ExRegexSyntaxError : public std::runtime_error
        { public: ExRegexSyntaxError() : std::runtime_error( "Syntax error in regular expression") {}};
    };

    template <typename StringT>
    class RegexMatchesConstraint : public ConstraintBase<StringT>, public RegexMatchesConstraintExceptions
    {
    public:
        typedef std::basic_regex<typename StringT::value_type, std::regex_traits<typename StringT::value_type> > RegexT;
        typedef RegexMatchesConstraint<StringT> ThisT;
        using ConstraintBase<StringT>::m_not_;

        explicit RegexMatchesConstraint( const StringT& matchesText)
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

        void ignoreCase( bool ignoreCase) override
        {
            m_ignoreCase = ignoreCase; 
        }

        [[nodiscard]] bool ignoreCase() const override
        {
            return m_ignoreCase;
        }

        bool operator == ( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            if (pRhs)
            {
                if ( !this->equalBaseProperties(*pRhs)
                    || m_ignoreCase != pRhs->m_ignoreCase
                    || m_matches != pRhs->m_matches
                    )
                {
                    return false;
                }
                return true;
            }
            return false;
        }

        bool matchesConstraint( const StringT& text) const override
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
    class AnyConstraint : public ConstraintDirectives<StringT>
    {
    public:
        typedef AnyConstraint<StringT> ThisT;

        bool operator==( const IConstraint<StringT>& constraint) const override
        {
            auto pRhs = dynamic_cast<const ThisT*>(&constraint);
            return pRhs != nullptr;
        }

        [[nodiscard]] bool forAll() const override
        {
            return false;
        }

        [[nodiscard]] bool flush() const override
        {
            return true;
        }

        static void not_(bool)
        {
        }

        bool matchesConstraint( const StringT&) const override
        {
            return true;
        }
    };
}
