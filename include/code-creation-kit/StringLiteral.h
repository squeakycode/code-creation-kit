// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    inline const char* getStringLiteral( const char* a, const wchar_t*, const char*)
    {
        return a;
    }

    inline const wchar_t* getStringLiteral( const char*, const wchar_t* b, const wchar_t*)
    {
        return b;
    }

    inline char getStringLiteral( char a, wchar_t, const char*)
    {
        return a;
    }

    inline wchar_t getStringLiteral( char, wchar_t b, const wchar_t*)
    {
        return b;
    }
}

#define STRING_LITERAL( stringLiteral) code_creation_kit::getStringLiteral( stringLiteral, L##stringLiteral, (CharT*)0)
