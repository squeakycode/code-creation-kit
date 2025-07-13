// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///used to check a constraint for a single table entry 
    template <typename StringT>
    class IConstraint
    {
    public:
        virtual bool matchesConstraint( const StringT& text) const = 0;
        virtual bool forAll() const = 0;
        virtual bool flush() const = 0;

        virtual bool operator==( const IConstraint& conversion) const = 0;

        virtual ~IConstraint() = default;
    };
}
