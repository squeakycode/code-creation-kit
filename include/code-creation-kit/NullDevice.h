// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///Nul device.
    class CNul
    {
    public:
        template <typename T>
        CNul& operator<<( const T&) { return *this; }
    };
}
