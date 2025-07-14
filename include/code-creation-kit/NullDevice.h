// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///Null device.
    class NullDevice
    {
    public:
        template <typename T>
        NullDevice& operator<<( const T&) { return *this; }
    };
}
