// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

namespace code_creation_kit
{
    class Tokens
    {
    public:
        ///the tokens
        enum ETokens
        {
            eInvalid = 0,
            eNewLine = 1,
            eComment,
            eInclude,
            eMarkup,
            eTrim,
            eTrimLeft,
            eTrimRight,
            ePart,
            ePartBegin,
            ePartEnd,
            ePartLazy,
            ePartPadding,
            ePartRemove,
            eSetRecursionLevelLimit,
            eSetRecursionLevelLimitOff,
            eTableBegin,
            eTableEnd,
            eTableLoad,
            eTableRemove,
            eAny,
            eVolatil,
            eBegin,
            eBlockFormat,
            eCalc,
            eContains,
            eCount,
            eEnd,
            eEndsWith,
            eEntry,
            eMatches,
            eError_,
            eFirstTime,
            eFlush,
            eForAll,
            eHtmlEscape,
            eIf_,
            eIgnoreCase,
            eIndex,
            eLastTime,
            eMacroBegin,
            eMacroEnd,
            eRegexMatches,
            eMerge,
            eNot_,
            eOr_,
            ePadLeft,
            ePadRight,
            eLeftToRight,
            eTopDown,
            eRegexReplace,
            eReplace,
            eStartsWith,
            eToCString,
            eToCsv,
            eToLower,
            eToSize,
            eToUpper,
            eTextFragment,
            eFullLineWithoutTags
        };

        typedef ETokens token_type; 
    };
}
