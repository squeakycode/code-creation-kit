//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_STRINGLITERAL_H_8481256
#define INCLUDED_STRINGLITERAL_H_8481256

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/preprocessor/cat.hpp>

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

#define STRING_LITERAL( stringLiteral) getStringLiteral( stringLiteral, BOOST_PP_CAT(L, stringLiteral), (CharT*)0)

#endif /* INCLUDED_STRINGLITERAL_H_8481256 */
