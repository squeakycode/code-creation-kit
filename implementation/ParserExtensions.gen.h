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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#ifndef INCLUDED_PARSEREXTENSIONS_TPL_H_07842654
#define INCLUDED_PARSEREXTENSIONS_TPL_H_07842654

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "Constraints.h"
#include "Conversions.h"

template <typename T, typename PosT>
T* newItem0( PosT&)
{
    return new T();
}

template <typename T, typename PosT>
T* newItem1( PosT& pos)
{
    return new T( pos->getStringList()->front());
}

template <typename T, typename PosT>
T* newItem2( PosT& pos)
{
    return new T( pos->getStringList()->front(), pos->getStringList()->back());
}

///returns true if token at position is a directive for constraint
template <typename PosT>
bool isDirectiveForConstraint( PosT& pos)
{
    if (
           *pos == PosT::value_type::eFlush
        || *pos == PosT::value_type::eForAll
        || *pos == PosT::value_type::eIgnoreCase
    )
    {
        return true;
    }
    return false;
}

///returns true if token at position is a constraint
template <typename PosT>
bool isConstraint( PosT& pos)
{
    if (
           *pos == PosT::value_type::eAny
        || *pos == PosT::value_type::eContains
        || *pos == PosT::value_type::eEndsWith
        || *pos == PosT::value_type::eMatches
        || *pos == PosT::value_type::eRegexMatches
        || *pos == PosT::value_type::eStartsWith
    )
    {
        return true;
    }
    return false;
}

///returns true if token at position is a directive for conversion
template <typename PosT>
bool isDirectiveForConversion( PosT& pos)
{
    if (
           *pos == PosT::value_type::eIgnoreCase
    )
    {
        return true;
    }
    return false;
}

///returns true if token at position is a conversion
template <typename PosT>
bool isConversion( PosT& pos)
{
    if (
           *pos == PosT::value_type::eMerge
        || *pos == PosT::value_type::eRegexReplace
        || *pos == PosT::value_type::eReplace
        || *pos == PosT::value_type::eToCString
        || *pos == PosT::value_type::eToLower
        || *pos == PosT::value_type::eToUpper
    )
    {
        return true;
    }
    return false;
}

///returns true if token at position is a directive for substitution
template <typename PosT>
bool isDirectiveForSubstitution( PosT& pos)
{
    if (
           *pos == PosT::value_type::eVolatil
        || *pos == PosT::value_type::eLeftToRight
        || *pos == PosT::value_type::eTopDown
    )
    {
        return true;
    }
    return false;
}

///returns true if token at position is a substitution
template <typename PosT>
bool isSubstitution( PosT& pos)
{
    if (
           *pos == PosT::value_type::eCount
        || *pos == PosT::value_type::eEntry
        || *pos == PosT::value_type::eError_
        || *pos == PosT::value_type::eFirstTime
        || *pos == PosT::value_type::eIndex
        || *pos == PosT::value_type::eLastTime
    )
    {
        return true;
    }
    return false;
}


template <typename TokenT>
bool isValidCombination( typename TokenT::ETokenT item, typename TokenT::ETokenT appliedItem)
{
    switch( item)
    {
    case TokenT::eContains:
        {
            switch( appliedItem)
            {
            case TokenT::eFlush: return true;
            case TokenT::eForAll: return true;
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eCount:
        {
            switch( appliedItem)
            {
            case TokenT::eRegexMatches: return true;
            case TokenT::eMatches: return true;
            case TokenT::eReplace: return true;
            case TokenT::eRegexReplace: return true;
            case TokenT::eStartsWith: return true;
            case TokenT::eEndsWith: return true;
            case TokenT::eContains: return true;
            }
        }
    case TokenT::eEndsWith:
        {
            switch( appliedItem)
            {
            case TokenT::eFlush: return true;
            case TokenT::eForAll: return true;
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eEntry:
        {
            switch( appliedItem)
            {
            case TokenT::eLeftToRight: return true;
            case TokenT::eTopDown: return true;
            case TokenT::eVolatil: return true;
            case TokenT::eRegexMatches: return true;
            case TokenT::eMatches: return true;
            case TokenT::eAny: return true;
            case TokenT::eMerge: return true;
            case TokenT::eReplace: return true;
            case TokenT::eRegexReplace: return true;
            case TokenT::eToLower: return true;
            case TokenT::eToUpper: return true;
            case TokenT::eStartsWith: return true;
            case TokenT::eEndsWith: return true;
            case TokenT::eContains: return true;
            case TokenT::eToCString: return true;
            }
        }
    case TokenT::eMatches:
        {
            switch( appliedItem)
            {
            case TokenT::eFlush: return true;
            case TokenT::eForAll: return true;
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eIndex:
        {
            switch( appliedItem)
            {
            case TokenT::eRegexMatches: return true;
            case TokenT::eMatches: return true;
            case TokenT::eReplace: return true;
            case TokenT::eRegexReplace: return true;
            case TokenT::eStartsWith: return true;
            case TokenT::eEndsWith: return true;
            case TokenT::eContains: return true;
            }
        }
    case TokenT::eRegexMatches:
        {
            switch( appliedItem)
            {
            case TokenT::eFlush: return true;
            case TokenT::eForAll: return true;
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eRegexReplace:
        {
            switch( appliedItem)
            {
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eReplace:
        {
            switch( appliedItem)
            {
            case TokenT::eIgnoreCase: return true;
            }
        }
    case TokenT::eStartsWith:
        {
            switch( appliedItem)
            {
            case TokenT::eFlush: return true;
            case TokenT::eForAll: return true;
            case TokenT::eIgnoreCase: return true;
            }
        }
    }
    return false;
}

///parses directive for constraint
template <typename PosT, typename ItemT, typename ETokenT>
bool parseDirectiveForConstraint( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool success = false;
    while ( pos != end && isDirectiveForConstraint( pos)) 
    {
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::ExCannotApplyDirectiveToConstraint();
        }

        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eFlush:
            {
                if ( item.flush())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.flush( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        case TokenT::eForAll:
            {
                if ( item.forAll())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.forAll( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        case TokenT::eIgnoreCase:
            {
                if ( item.ignoreCase())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.ignoreCase( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a directive for constraint is missing.");
        }
    }

    return success;
}

///parses constraint
template <typename PosT, typename ItemT, typename ETokenT>
bool parseConstraint( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool not_ = false;
    if ( pos != end && *pos == TokenT::eNot_)
    {
        not_ = true;
        ++pos;
    }

    bool success = false;
    while ( pos != end && isConstraint( pos)) 
    {
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::ExCannotApplyConstraintToSubstitution();
        }

        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eAny:
            {
                CAnyConstraint<StringT>* constraint = newItem0<CAnyConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eContains:
            {
                CContainsConstraint<StringT>* constraint = newItem1<CContainsConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eEndsWith:
            {
                CEndsWithConstraint<StringT>* constraint = newItem1<CEndsWithConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eMatches:
            {
                CMatchesConstraint<StringT>* constraint = newItem1<CMatchesConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eRegexMatches:
            {
                CRegexMatchesConstraint<StringT>* constraint = newItem1<CRegexMatchesConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eStartsWith:
            {
                CStartsWithConstraint<StringT>* constraint = newItem1<CStartsWithConstraint<StringT> >( pos);
                item.attach( constraint);
                ConstraintDirectives<StringT>& newItem = *constraint;
                if ( not_ ) constraint->not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a constraint is missing.");
        }
        not_ = false;
        if ( pos != end && *pos == TokenT::eNot_)
        {
            not_ = true;
            ++pos;
        }
    }

    if ( not_)
    {
        throw CParserExceptions::ExConstraintExpectedAfterNot();
    }

    return success;
}

///parses directive for conversion
template <typename PosT, typename ItemT, typename ETokenT>
bool parseDirectiveForConversion( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool success = false;
    while ( pos != end && isDirectiveForConversion( pos)) 
    {
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::ExCannotApplyDirectiveToConversion();
        }

        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eIgnoreCase:
            {
                if ( item.ignoreCase())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.ignoreCase( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a directive for conversion is missing.");
        }
    }

    return success;
}

///parses conversion
template <typename PosT, typename ItemT, typename ETokenT>
bool parseConversion( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool success = false;
    while ( pos != end && isConversion( pos)) 
    {
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::ExCannotApplyConversionToSubstitution();
        }

        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eMerge:
            {
                CMergeConversion<StringT>* conversion = newItem1<CMergeConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eRegexReplace:
            {
                CRegexReplaceConversion<StringT>* conversion = newItem2<CRegexReplaceConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eReplace:
            {
                CReplaceConversion<StringT>* conversion = newItem2<CReplaceConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eToCString:
            {
                CToCStringConversion<StringT>* conversion = newItem0<CToCStringConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eToLower:
            {
                CToLowerConversion<StringT>* conversion = newItem0<CToLowerConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eToUpper:
            {
                CToUpperConversion<StringT>* conversion = newItem0<CToUpperConversion<StringT> >( pos);
                item.attach( conversion);
                ConversionDirectives<StringT>& newItem = *conversion;
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a conversion is missing.");
        }
    }

    return success;
}

///parses directive for substitution
template <typename PosT, typename ItemT, typename ETokenT>
bool parseDirectiveForSubstitution( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool success = false;
    while ( pos != end && isDirectiveForSubstitution( pos)) 
    {
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::ExCannotApplyDirectiveToSubstitution();
        }

        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eVolatil:
            {
                if ( item.volatil())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.volatil( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        case TokenT::eLeftToRight:
            {
                if ( item.leftToRight())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.leftToRight( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        case TokenT::eTopDown:
            {
                if ( item.topDown())
                {
                    throw CParserExceptions::ExDirectiveAlreadyApplied();
                }
                else
                {
                    item.topDown( true);
                }
                PosT newParentItem = pos;
                ++pos;
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a directive for substitution is missing.");
        }
    }

    return success;
}

///parses substitution
template <typename PosT, typename ItemT>
bool parseSubstitution( PosT& pos, PosT& end, ItemT& item)
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    bool if_ = false;
    bool not_ = false;
    if ( pos != end && *pos == TokenT::eIf_)
    {
        if_ = true;
        ++pos;

        if ( pos != end && *pos == TokenT::eNot_)
        {
            not_ = true;
            ++pos;
        }
    }

    bool success = false;
    if ( pos != end && isSubstitution( pos)) 
    {
        success = true;
        switch ( pos->getToken())
        {
        case TokenT::eCount:
            {
                item = ItemT( ItemT::eCount, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eEntry:
            {
                item = ItemT( ItemT::eEntry, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eError_:
            {
                item = ItemT( ItemT::eError_, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eFirstTime:
            {
                item = ItemT( ItemT::eFirstTime, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eIndex:
            {
                item = ItemT( ItemT::eIndex, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        case TokenT::eLastTime:
            {
                item = ItemT( ItemT::eLastTime, pos->getStringList());
                ItemT& newItem = item;
                if ( if_ ) newItem.if_( if_);
                if_ = false;
                if ( not_ ) newItem.not_( not_);
                PosT newParentItem = pos;
                ++pos;
                parseDirectiveForSubstitution( pos, end, newItem, newParentItem->getToken());
                parseConstraint( pos, end, newItem, newParentItem->getToken());
                parseConversion( pos, end, newItem, newParentItem->getToken());
            }
            break;

        default:
            throw std::runtime_error( "Internal program error, a handler for a substitution is missing.");
        }
    }

    if ( if_)
    {
        throw CParserExceptions::ExSubstitutionExpectedAfterIf();
    }

    return success;
}


#endif /* INCLUDED_PARSEREXTENSIONS_TPL_H_07842654 */
