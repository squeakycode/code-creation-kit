// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include "Constraints.h"
#include "Conversions.h"

namespace code_creation_kit
{
    template <typename T, typename PosT>
    std::shared_ptr<T> newItem0(PosT&)
    {
        return std::make_shared<T>();
    }

    template <typename T, typename PosT>
    std::shared_ptr<T> newItem1(PosT& pos)
    {
        return std::make_shared<T>(pos->getStringList()->front());
    }

    template <typename T, typename PosT>
    std::shared_ptr<T> newItem2(PosT& pos)
    {
        return std::make_shared<T>(pos->getStringList()->front(), pos->getStringList()->back());
    }

    template <typename T, typename PosT>
    std::shared_ptr<T> newItem2Combi1CStyle1UIntRepeatUIntOptional(PosT& pos)
    {
        return std::make_shared<T>(pos->getStringList()->at(0), pos->getStringList()->at(1), pos->getStringList()->begin() + 2, pos->getStringList()->end());
    }

    template <typename T, typename PosT>
    std::shared_ptr<T> newItem1Combi1CStyle1CStyleOptional(PosT& pos)
    {
        if (pos->getStringList()->size() > 1)
        {
            assert(pos->getStringList()->size() == 2);
            return newItem2<T>(pos);
        }
        else
        {
            return newItem1<T>(pos);
        }
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
               *pos == PosT::value_type::eBlockFormat
            || *pos == PosT::value_type::eCalc
            || *pos == PosT::value_type::eHtmlEscape
            || *pos == PosT::value_type::eMerge
            || *pos == PosT::value_type::ePadLeft
            || *pos == PosT::value_type::ePadRight
            || *pos == PosT::value_type::eRegexReplace
            || *pos == PosT::value_type::eReplace
            || *pos == PosT::value_type::eToCString
            || *pos == PosT::value_type::eToCsv
            || *pos == PosT::value_type::eToLower
            || *pos == PosT::value_type::eToSize
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
                default: return false;
                }
            }
        case TokenT::eCount:
            {
                switch( appliedItem)
                {
                case TokenT::eContains: return true;
                case TokenT::eMatches: return true;
                case TokenT::eRegexMatches: return true;
                case TokenT::eStartsWith: return true;
                case TokenT::eEndsWith: return true;
                case TokenT::eCalc: return true;
                case TokenT::ePadLeft: return true;
                case TokenT::ePadRight: return true;
                case TokenT::eReplace: return true;
                case TokenT::eRegexReplace: return true;
                default: return false;
                }
            }
        case TokenT::eEndsWith:
            {
                switch( appliedItem)
                {
                case TokenT::eFlush: return true;
                case TokenT::eForAll: return true;
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        case TokenT::eEntry:
            {
                switch( appliedItem)
                {
                case TokenT::eLeftToRight: return true;
                case TokenT::eTopDown: return true;
                case TokenT::eVolatil: return true;
                case TokenT::eAny: return true;
                case TokenT::eContains: return true;
                case TokenT::eMatches: return true;
                case TokenT::eRegexMatches: return true;
                case TokenT::eStartsWith: return true;
                case TokenT::eEndsWith: return true;
                case TokenT::eBlockFormat: return true;
                case TokenT::eCalc: return true;
                case TokenT::eHtmlEscape: return true;
                case TokenT::eMerge: return true;
                case TokenT::ePadLeft: return true;
                case TokenT::ePadRight: return true;
                case TokenT::eReplace: return true;
                case TokenT::eRegexReplace: return true;
                case TokenT::eToLower: return true;
                case TokenT::eToUpper: return true;
                case TokenT::eToCsv: return true;
                case TokenT::eToCString: return true;
                case TokenT::eToSize: return true;
                default: return false;
                }
            }
        case TokenT::eMatches:
            {
                switch( appliedItem)
                {
                case TokenT::eFlush: return true;
                case TokenT::eForAll: return true;
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        case TokenT::eIndex:
            {
                switch( appliedItem)
                {
                case TokenT::eContains: return true;
                case TokenT::eMatches: return true;
                case TokenT::eRegexMatches: return true;
                case TokenT::eStartsWith: return true;
                case TokenT::eEndsWith: return true;
                case TokenT::eCalc: return true;
                case TokenT::ePadLeft: return true;
                case TokenT::ePadRight: return true;
                case TokenT::eReplace: return true;
                case TokenT::eRegexReplace: return true;
                default: return false;
                }
            }
        case TokenT::eRegexMatches:
            {
                switch( appliedItem)
                {
                case TokenT::eFlush: return true;
                case TokenT::eForAll: return true;
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        case TokenT::eRegexReplace:
            {
                switch( appliedItem)
                {
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        case TokenT::eReplace:
            {
                switch( appliedItem)
                {
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        case TokenT::eStartsWith:
            {
                switch( appliedItem)
                {
                case TokenT::eFlush: return true;
                case TokenT::eForAll: return true;
                case TokenT::eIgnoreCase: return true;
                default: return false;
                }
            }
        default:
            //results in return false
            break;
        }
        return false;
    }

    ///parses directive for constraint
    template <typename PosT, typename ItemT, typename ETokenT>
    bool parseDirectiveForConstraint( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
    {
        typedef typename PosT::value_type TokenT;

        bool success = false;
        while ( pos != end && isDirectiveForConstraint( pos)) 
        {
            if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
            {
                throw ParserExceptions::ExCannotApplyDirectiveToConstraint();
            }

            success = true;
            switch ( pos->getToken())
            {
            case TokenT::eFlush:
                {
                    if ( item.flush())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.flush( true);
                    }
                    ++pos;
                }
                break;

            case TokenT::eForAll:
                {
                    if ( item.forAll())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.forAll( true);
                    }
                    ++pos;
                }
                break;

            case TokenT::eIgnoreCase:
                {
                    if ( item.ignoreCase())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.ignoreCase( true);
                    }
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
                throw ParserExceptions::ExCannotApplyConstraintToSubstitution();
            }

            success = true;
            switch ( pos->getToken())
            {
            case TokenT::eAny:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<AnyConstraint<StringT> > ptrConstraint = newItem0<AnyConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eContains:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<ContainsConstraint<StringT> > ptrConstraint = newItem1<ContainsConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eEndsWith:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<EndsWithConstraint<StringT> > ptrConstraint = newItem1<EndsWithConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eMatches:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<MatchesConstraint<StringT> > ptrConstraint = newItem1<MatchesConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eRegexMatches:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<RegexMatchesConstraint<StringT> > ptrConstraint = newItem1<RegexMatchesConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConstraint( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eStartsWith:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    std::shared_ptr<StartsWithConstraint<StringT> > ptrConstraint = newItem1<StartsWithConstraint<StringT> >( pos);
                    item.attach( ptrConstraint);
                    ConstraintDirectives<StringT>& newItem = *ptrConstraint;
                    if ( not_ ) ptrConstraint->not_( not_);
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
            throw ParserExceptions::ExConstraintExpectedAfterNot();
        }

        return success;
    }

    ///parses directive for conversion
    template <typename PosT, typename ItemT, typename ETokenT>
    bool parseDirectiveForConversion( PosT& pos, PosT& end, ItemT& item, ETokenT parentItem)
    {
        typedef typename PosT::value_type TokenT;

        bool success = false;
        while ( pos != end && isDirectiveForConversion( pos)) 
        {
            if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
            {
                throw ParserExceptions::ExCannotApplyDirectiveToConversion();
            }

            success = true;
            switch ( pos->getToken())
            {
            case TokenT::eIgnoreCase:
                {
                    if ( item.ignoreCase())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.ignoreCase( true);
                    }
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

        bool success = false;
        while ( pos != end && isConversion( pos)) 
        {
            if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
            {
                throw ParserExceptions::ExCannotApplyConversionToSubstitution();
            }

            success = true;
            switch ( pos->getToken())
            {
            case TokenT::eBlockFormat:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<BlockFormatConversion<StringT> > ptrConversion = newItem1<BlockFormatConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eCalc:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<CalcConversion<StringT> > ptrConversion = newItem1<CalcConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eHtmlEscape:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<HtmlEscapeConversion<StringT> > ptrConversion = newItem0<HtmlEscapeConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eMerge:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<MergeConversion<StringT> > ptrConversion = newItem1<MergeConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::ePadLeft:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<PadLeftConversion<StringT> > ptrConversion = newItem2Combi1CStyle1UIntRepeatUIntOptional<PadLeftConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::ePadRight:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<PadRightConversion<StringT> > ptrConversion = newItem2Combi1CStyle1UIntRepeatUIntOptional<PadRightConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eRegexReplace:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<RegexReplaceConversion<StringT> > ptrConversion = newItem2<RegexReplaceConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eReplace:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ReplaceConversion<StringT> > ptrConversion = newItem2<ReplaceConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eToCString:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ToCStringConversion<StringT> > ptrConversion = newItem0<ToCStringConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eToCsv:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ToCsvConversion<StringT> > ptrConversion = newItem1Combi1CStyle1CStyleOptional<ToCsvConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eToLower:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ToLowerConversion<StringT> > ptrConversion = newItem0<ToLowerConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eToSize:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ToSizeConversion<StringT> > ptrConversion = newItem1<ToSizeConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
                    PosT newParentItem = pos;
                    ++pos;
                    parseDirectiveForConversion( pos, end, newItem, newParentItem->getToken());
                }
                break;

            case TokenT::eToUpper:
                {
                    typedef typename TokenT::StringListT::value_type StringT;
                    
                    std::shared_ptr<ToUpperConversion<StringT> > ptrConversion = newItem0<ToUpperConversion<StringT> >( pos);
                    item.attach( ptrConversion);
                    ConversionDirectives<StringT>& newItem = *ptrConversion;
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

        bool success = false;
        while ( pos != end && isDirectiveForSubstitution( pos)) 
        {
            if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
            {
                throw ParserExceptions::ExCannotApplyDirectiveToSubstitution();
            }

            success = true;
            switch ( pos->getToken())
            {
            case TokenT::eVolatil:
                {
                    if ( item.volatil())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.volatil( true);
                    }
                    ++pos;
                }
                break;

            case TokenT::eLeftToRight:
                {
                    if ( item.leftToRight())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.leftToRight( true);
                    }
                    ++pos;
                }
                break;

            case TokenT::eTopDown:
                {
                    if ( item.topDown())
                    {
                        throw ParserExceptions::ExDirectiveAlreadyApplied();
                    }
                    else
                    {
                        item.topDown( true);
                    }
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
            throw ParserExceptions::ExSubstitutionExpectedAfterIf();
        }

        return success;
    }

}
