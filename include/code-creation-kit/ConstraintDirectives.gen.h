// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include "IConstraint.h"
#include <stdexcept>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif

namespace code_creation_kit
{
    template <typename StringT>
    class ConstraintDirectives : public IConstraint<StringT>
    {
        void throwInternalProgramError() const
        {
            throw std::runtime_error( "Internal program error, the implementation for a constraint directive is missing.");
        }
    public:
        virtual void flush( bool)
        {
            throwInternalProgramError();
        }
        virtual bool flush() const
        {
            throwInternalProgramError();
            return false;
        }

        virtual void forAll( bool)
        {
            throwInternalProgramError();
        }
        virtual bool forAll() const
        {
            throwInternalProgramError();
            return false;
        }

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
}

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
