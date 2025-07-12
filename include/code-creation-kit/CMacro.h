// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "CMacroExpression.h"
#include "CSubstitution.h"
#include <vector>

namespace code_creation_kit
{
    ///data structure representing a macro, created from parsed macro
    template <typename StringT>
    class CMacro
    {
    public:
        typedef CMacroExpression<StringT> MacroExpressionT;
        typedef CSubstitution<StringT> SubstitutionT;
        typedef std::vector<SubstitutionT> SubstitutionVectorT;
        typedef std::size_t SizeT;
        typedef SizeT IndexT;

        ///swaps in expression and returns an empty expression
        void attach( MacroExpressionT& expression)
        {
            return m_expression.swap( expression);
        }

        ///returns expression for processing
        const MacroExpressionT& getExpression() const
        {
            return m_expression;
        }

        ///adds a substitution and returns an index for access
        IndexT addSubstitution( const SubstitutionT& substitution)
        {
            //check if substitution exists already
            for ( IndexT  i = 0; i < m_substitutions.size(); ++i)
            {
                if ( m_substitutions[ i ] == substitution )
                {
                    return i; ///same substitution exists
                }
            }
            //add new substitution
            m_substitutions.push_back( substitution);
            return m_substitutions.size() - 1;
        }

        ///provides non modifying access to the substitutions
        const SubstitutionVectorT& getSubstitutions() const
        {
            return m_substitutions;
        }

        ///returns true if macro requires no table look up
        bool noLookUp() const
        {
            for (const SubstitutionT& substitution : m_substitutions)
            {
                if ( !substitution.noLookUp())
                {
                    return false;
                }
            }
            return true;
        }

        ///returns true if macro contains a last time keyword
        bool lastTime() const
        {
            for (const SubstitutionT& substitution : m_substitutions)
            {
                if ( substitution == SubstitutionT::eLastTime)
                {
                    return true;
                }
            }
            return false;
        }

        MacroExpressionT m_expression; ///<the expression tree
        SubstitutionVectorT m_substitutions; ///<holds the data representing a substitution, e.g. for an entry
    };
}
