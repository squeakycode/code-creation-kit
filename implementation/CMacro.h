//   Copyright (C) 2011-2014 Andreas Gau
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

#ifndef INCLUDED_AMACRO_H_07629031
#define INCLUDED_AMACRO_H_07629031

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "CMacroExpression.h"
#include "CSubstitution.h"
#include <vector>
#include <boost/foreach.hpp>

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
        BOOST_FOREACH( const SubstitutionT& substitution, m_substitutions)
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
        BOOST_FOREACH( const SubstitutionT& substitution, m_substitutions)
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

#endif /* INCLUDED_AMACRO_H_07629031 */
