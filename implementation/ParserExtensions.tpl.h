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

[MACRO_BEGIN][TRIM]
///returns true if token at position is a [ENTRY]["Subtype"]
template <typename PosT>
bool is[ENTRY]["Name for Subtype"]( PosT& pos)
{
    if (
        [BEGIN.][IF.][FIRST_TIME.]   [OR.]|| [END.]*pos == PosT::value_type::e[ENTRY.]["Tag Name Capital"][IF.][ENTRY.]["Subtype"][EQUALS.]["[ENTRY]["Subtype"]"]
    )
    {
        return true;
    }
    return false;
}

[MACRO_END][TRIM]

template <typename TokenT>
bool isValidCombination( typename TokenT::ETokenT item, typename TokenT::ETokenT appliedItem)
{
    switch( item)
    {
        [MACRO_BEGIN][IF][ENTRY]["Applyable"][TRIM]
    case TokenT::e[ENTRY]["Tag Name Capital"]:
        {
            switch( appliedItem)
            {
            case TokenT::e[ENTRY..]["Tag Name Capital"][IF..][ENTRY..]["Tag Name"][EQUALS..]["[ENTRY.]["[ENTRY]["Tag Name"]"]"][IF.][ENTRY.]["Tag Name"][EQUALS.]["Applyable"]: return true;
            }
        }
        [MACRO_END][TRIM]
    }
    return false;
}

[MACRO_BEGIN][TRIM]
///parses [ENTRY]["Subtype"]
template <typename PosT, typename ItemT[BEGIN][IF][ENTRY]["Validation Error"], typename ETokenT[OR][END]>
bool parse[ENTRY]["Name for Subtype"]( PosT& pos, PosT& end, ItemT& item[BEGIN][IF][ENTRY]["Validation Error"], ETokenT parentItem[OR][END])
{
    typedef typename PosT::value_type TokenT;
    typedef typename TokenT::StringListT::value_type StringT;

    [BEGIN][IF][ENTRY]["Subtype"][EQUALS]["substitution"][TRIM]
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

    [OR][END][TRIM]
    [BEGIN][IF][ENTRY]["Subtype"][EQUALS]["constraint"][TRIM]
    bool not_ = false;
    if ( pos != end && *pos == TokenT::eNot_)
    {
        not_ = true;
        ++pos;
    }

    [OR][END][TRIM]
    bool success = false;
    [BEGIN]if[IF][ENTRY]["Subtype"][EQUALS]["substitution"][OR]while[END] ( pos != end && is[ENTRY]["Name for Subtype"]( pos)) 
    {
        [BEGIN][TRIM]
        if ( !isValidCombination<TokenT>( parentItem, pos->getToken()))
        {
            throw CParserExceptions::[ENTRY]["Validation Error"]();
        }

        [OR][END][TRIM]
        success = true;
        switch ( pos->getToken())
        {
        [MACRO_BEGIN.][TRIM.]
        case TokenT::e[ENTRY.]["Tag Name Capital"][IF.][ENTRY.]["Subtype"][EQUALS.]["[ENTRY]["Subtype"]"]:
            {
                [ENTRY]["Action"][REGEX_REPLACE]['\n', '\n                ']
                PosT newParentItem = pos;
                ++pos;
                parse[ENTRY..]["[ENTRY]["Subtype"]"][IF..][ENTRY..]["Subtype"][EQUALS..]["Parse"]( pos, end, newItem, newParentItem->getToken());
            }
            break;

        [MACRO_END.][TRIM.]
        default:
            throw std::runtime_error( "Internal program error, a handler for a [ENTRY]["Subtype"] is missing.");
        }
        [BEGIN][IF][ENTRY]["Subtype"][EQUALS]["constraint"][TRIM]
        not_ = false;
        if ( pos != end && *pos == TokenT::eNot_)
        {
            not_ = true;
            ++pos;
        }
        [OR][END][TRIM]
    }

    [BEGIN][IF][ENTRY]["Subtype"][EQUALS]["constraint"][TRIM]
    if ( not_)
    {
        throw CParserExceptions::ExConstraintExpectedAfterNot();
    }

    [OR][END][TRIM]
    [BEGIN][IF][ENTRY]["Subtype"][EQUALS]["substitution"][TRIM]
    if ( if_)
    {
        throw CParserExceptions::ExSubstitutionExpectedAfterIf();
    }

    [OR][END][TRIM]
    return success;
}

[MACRO_END][TRIM]

#endif /* INCLUDED_PARSEREXTENSIONS_TPL_H_07842654 */
