// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <vector>

namespace code_creation_kit
{
    ///used to modify an inserted table entry
    template <typename StringT>
    class IConversion
    {
    public:
        typedef std::vector<StringT> StringListT;

        virtual void modify( StringListT& text) const = 0;

        virtual bool operator==( const IConversion& conversion) const = 0;

        virtual ~IConversion() 
        {
        };
    };
}
