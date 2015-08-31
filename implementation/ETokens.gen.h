//   Copyright (C) 2011-2012 Andreas Gau
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

#ifndef INCLUDED_ETOKENS_TPL_H_132635
#define INCLUDED_ETOKENS_TPL_H_132635

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

class Tokens
{
public:
    ///the tokens
    enum ETokens
    {
        eNewLine = 1,
        eComment,
        eInclude,
        eMarkup,
        eTrim,
        eTrimLeft,
        eSetRecursionLevelLimit,
        eSetRecursionLevelLimitOff,
        eAny,
        eVolatil,
        eBegin,
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
        eLeftToRight,
        eTopDown,
        eRegexReplace,
        eReplace,
        eStartsWith,
        eToCString,
        eToLower,
        eToUpper,
        eTextFragment,
        eFullLineWithoutTags
    };

    typedef ETokens token_type; 
};

#endif /* INCLUDED_ETOKENS_TPL_H_132635 */
