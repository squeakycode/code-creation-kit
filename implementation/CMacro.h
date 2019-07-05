//  Copyright (c) 2011-2019 Andreas Gau
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

#include "CMacroExpression.h"
#include "CSubstitution.h"
#include <vector>
#include <boost/foreach.hpp>

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
}
