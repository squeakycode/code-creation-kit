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

#include <list>
#include <algorithm>

namespace code_creation_kit
{
    ///represents the macro tree structure
    template <typename StringT>
    class CMacroExpression
    {
    public:
        typedef std::size_t SizeT;
        typedef SizeT IndexT;
        typedef std::list<CMacroExpression> SubExpressionListT;

        CMacroExpression() 
            : m_ored(false)
            , m_substitution(false)
            , m_index(0)
        {
        }

        //getters for expression data
        const SubExpressionListT& getSubExpressions() const { return m_subExpressions; }
        const StringT& getText() const { return m_macroExpression; }
        StringT& getText() { return m_macroExpression; }
        IndexT getIndex() const { return m_index; }
        bool isOred() const { return m_ored; }
        bool isSubstitution() const { return m_substitution; }

        ///true if node is leaf of expression tree
        bool isLeaf() const
        {
            return m_subExpressions.empty();
        }

        ///true if node is a leaf of expression tree that contains text
        bool isTextLeaf() const
        {
            return isLeaf() && !m_substitution;
        }

        ///true if text only and no text
        bool isEmpty() const 
        { 
            return isTextLeaf() && m_macroExpression.empty(); 
        }

        ///adds a text range to the tree
        template <typename IteratorT>
        void add( IteratorT start, IteratorT end)
        {
            CMacroExpression<StringT> temp;
            temp.m_macroExpression = StringT( start, end);
            attach( temp);
        }

        ///adds a substitution to the tree
        void add( IndexT index) 
        {
            CMacroExpression<StringT> temp;
            temp.m_index = index;
            temp.m_substitution = true;
            attach( temp);
        }

        ///adds a node to the tree, may use swap thus returning an empty node
        void attach( CMacroExpression<StringT>& subExpression)
        {   
            //do not add empty nodes as they have no effect
            if ( subExpression.isEmpty())
            {
                return;
            }

            //if ored expressions add to last alternative
            if ( isOred())
            {
                m_subExpressions.back().attach( subExpression);
                return;
            }

            //concatenate text fragments if possible
            if ( subExpression.isTextLeaf())
            {
                if ( isTextLeaf())
                {
                    m_macroExpression += subExpression.m_macroExpression;
                    return;
                }
                else if ( !isLeaf() &&  m_subExpressions.back().isTextLeaf())
                {
                    m_subExpressions.back().m_macroExpression += subExpression.m_macroExpression;
                    return;
                }
            }

            //if current is empty just replace it, to void useless empty nodes
            if ( isEmpty())
            {
                swap( subExpression);
                //if added node represents or add empty node to create list
                if ( isOred())
                {
                    //turn into list node
                    pushThisALevelDown(); 
                    m_subExpressions.push_back( CMacroExpression<StringT>());
                }
                return;
            }

            //turn into list node
            if ( isLeaf())
            {
                pushThisALevelDown();
            }

            m_subExpressions.push_back( CMacroExpression<StringT>());
            m_subExpressions.back().swap( subExpression);
        }

        ///sets the current and the next expression as alternatives
        void ored()
        {
            if (!m_ored)
            {
                pushThisALevelDown();
                m_ored = true;
            }
            //add expression for adding following expressions
            m_subExpressions.push_back( CMacroExpression<StringT>());
        }

        ///swap implementation
        void swap( CMacroExpression<StringT>& rhs)
        {
            m_subExpressions.swap( rhs.m_subExpressions);
            m_macroExpression.swap( rhs.m_macroExpression);
            std::swap( m_ored, rhs.m_ored);
            std::swap( m_substitution, rhs.m_substitution);
            std::swap( m_index, rhs.m_index);
        }

    private:
        ///creates a new parent node containing this as child by using swap 
        void pushThisALevelDown()
        {
            CMacroExpression<StringT> temp;
            temp.swap( *this);
            m_subExpressions.push_back( CMacroExpression<StringT>());
            m_subExpressions.back().swap( temp);
        }

        bool m_ored; ///<expressions are ored
        bool m_substitution; ///<substitution at position
        IndexT  m_index; ///<index of substitution

        SubExpressionListT m_subExpressions; ///list of expressions ored or sequential
        StringT m_macroExpression; ///text when text leaf
    };
}
