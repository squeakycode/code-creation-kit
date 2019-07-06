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

#include "ESubstitutions.gen.h"
#include "IConstraint.h"
#include "IConversion.h"
#include "CComparableSharedObject.h"
#include <list>
#include <vector>
#include <stdexcept>

namespace code_creation_kit
{
    ///holds the data representing a substitution, e.g. for an entry
    template <typename StringT>
    class CSubstitution : public Substitutions
    {
    public:
        typedef const std::vector<StringT> ConstStringListT;
        typedef std::shared_ptr<ConstStringListT> ConstSharedStringListT;

        typedef std::list<CComparableSharedObject<IConversion<StringT> > > ConversionListT;
        typedef std::list<CComparableSharedObject<IConstraint<StringT> > > ConstraintListT;

        CSubstitution()
            : m_type( eInvalid)
            , m_if_(false)
            , m_not_(false)
            , m_leftToRight(false)
            , m_topDown(false)
            , m_separator(false)
            , m_volatil(false)
        {
        }

        CSubstitution( ESubstitutions type)
            : m_type( type)
            , m_if_(false)
            , m_not_(false)
            , m_leftToRight(false)
            , m_topDown(false)
            , m_separator(false)
            , m_volatil(false)
        {
        }

        CSubstitution( ESubstitutions type, ConstSharedStringListT parameters)
            : m_type( type)
            , m_if_(false)
            , m_not_(false)
            , m_leftToRight(false)
            , m_topDown(false)
            , m_separator(false)
            , m_volatil(false)
            , m_parameters( parameters)
        {
        }

        ///returns true if not equal, used for optimizations
        bool operator != ( const CSubstitution<StringT>& rhs) const
        {
            return !(*this == rhs);
        }

        ///returns true if equal, used for optimizations
        bool operator == ( const CSubstitution<StringT>& rhs) const
        {
            if(
                m_type != rhs.m_type
                || m_if_ != rhs.m_if_
                || m_not_ != rhs.m_not_
                || m_leftToRight != rhs.m_leftToRight
                || m_topDown != rhs.m_topDown
                || m_separator != rhs.m_separator
                || m_volatil != rhs.m_volatil
                || m_separatorText != rhs.m_separatorText
                || m_parameters != rhs.m_parameters
                || m_conversionList != rhs.m_conversionList
                || m_constraintList != rhs.m_constraintList
                )
            {
                return false;
            }

            return true;
        }

        ///used for checking the type of the substitution
        bool operator == ( const ESubstitutions& rhs) const
        {
            return m_type == rhs;
        }

        ///returns entry name
        const StringT& entryName() const
        {
            return getParameters().front();
        }

        ///returns error Message
        const StringT& errorMessage() const
        {
            return getParameters().front();
        }

        ///returns the list of parameters
        ConstStringListT& getParameters() const
        {
            if ( !m_parameters)
            {
                throw std::runtime_error( "Access to missing parameter list.");
            }
            return *m_parameters;
        }

        ///returns the list of constraints
        const ConstraintListT& getConstraints() const
        {
            return m_constraintList;
        }

        ///returns the list of conversions
        const ConversionListT& getConversions() const
        {
            return m_conversionList;
        }

        void attach( std::shared_ptr<IConversion<StringT> > conversion)
        {
            m_conversionList.push_back( conversion);
        }

        void attach(std::shared_ptr <IConstraint<StringT> > constraint)
        {
            m_constraintList.push_back( constraint);
        }

        ///if_
        void if_( bool if_) 
        { 
            m_if_ = if_; 
        }
        bool if_() const { return m_if_; }

        ///not_
        void not_( bool not_) 
        { 
            m_not_ = not_; 
        }
        bool not_() const { return m_not_; }

        ///leftToRight
        void leftToRight( bool leftToRight) 
        { 
            m_leftToRight = leftToRight; 
        }
        bool leftToRight() const { return m_leftToRight; }

        ///topDown
        void topDown( bool topDown) 
        { 
            m_topDown = topDown; 
        }
        bool topDown() const { return m_topDown; }

        ///separator
        void separatorAttachText( StringT& separatorValue)
        { 
            m_separatorText.swap( separatorValue);
            m_separator = true; 
        }
        const StringT& separatorText() const { return m_separatorText; }
        bool separator() const { return m_separator; }

        ///volatil
        void volatil( bool volatil) 
        { 
            m_volatil = volatil; 
        }
        bool volatil() const { return m_volatil; }

        //only the entry keyword needs lookup
        bool noLookUp() const
        {
            return m_type != eEntry;
        }

    private:

        ESubstitutions m_type; ///<the substitution type

        //start possible modifiers
        bool m_if_;
        bool m_not_;
        bool m_leftToRight;
        bool m_topDown;
        bool m_separator;
        bool m_volatil;
        StringT m_separatorText;
        //end possible modifiers

        CComparableSharedObject<ConstStringListT> m_parameters; ///<the parameters of the substitution
        ConversionListT m_conversionList; ///<conversions to apply
        ConstraintListT m_constraintList; ///<constraints to apply
    };
}
