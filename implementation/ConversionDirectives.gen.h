//   Copyright (C) 2011 Andreas Gau
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

#ifndef INCLUDED_CONVERSIONDIRECTIVES_TPL_H_09888422
#define INCLUDED_CONVERSIONDIRECTIVES_TPL_H_09888422

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "IConversion.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif

template <typename StringT>
class ConversionDirectives : public IConversion<StringT>
{
    void throwInternalProgramError() const
    {
        throw std::runtime_error( "Internal program error, the implementation for a conversion directive is missing.");
    }
public:
    virtual void ignoreCase( bool)
    {
        throwInternalProgramError();
    }
    virtual bool ignoreCase() const
    {
        throwInternalProgramError();
        return false;
    }

};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif /* INCLUDED_CONVERSIONDIRECTIVES_TPL_H_09888422 */
