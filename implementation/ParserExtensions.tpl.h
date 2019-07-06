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
    std::shared_ptr<T> newItem2VariableArguments(PosT& pos)
    {
        return std::make_shared<T>(pos->getStringList()->at(0), pos->getStringList()->at(1), pos->getStringList()->begin() + 2, pos->getStringList()->end());
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
                default: return false;
                }
            }
            [MACRO_END][TRIM]
        default:
            //results in return false
            break;
        }
        return false;
    }

    [MACRO_BEGIN][TRIM]
    ///parses [ENTRY]["Subtype"]
    template <typename PosT, typename ItemT[BEGIN][IF][ENTRY]["Validation Error"], typename ETokenT[OR][END]>
    bool parse[ENTRY]["Name for Subtype"]( PosT& pos, PosT& end, ItemT& item[BEGIN][IF][ENTRY]["Validation Error"], ETokenT parentItem[OR][END])
    {
        typedef typename PosT::value_type TokenT;

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
                    [ENTRY]["Action"][REGEX_REPLACE]['\n', '\n                    ']
                    PosT newParentItem = pos;[IF..][FIRST_TIME..][IF..][ENTRY..]["[ENTRY]["Subtype"]"][IF..][ENTRY..]["Subtype"][EQUALS..]["Parse"]
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
}
