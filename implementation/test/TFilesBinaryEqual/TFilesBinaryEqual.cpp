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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "CFilesBinaryEqual.h"
#include <string>

BOOST_AUTO_TEST_CASE( TFilesBinaryEqual)
{
    BOOST_CHECK( FilesBinaryEqual<std::string>( "a.txt", "a_.txt"));
    BOOST_CHECK( !FilesBinaryEqual<std::string>( "a.txt", "b.txt"));
    //TODO: make compile under linux
#ifdef WIN32
    BOOST_CHECK( FilesBinaryEqual<std::wstring>( L"a.txt", L"a_.txt"));
    BOOST_CHECK( !FilesBinaryEqual<std::wstring>( L"a.txt", L"b.txt"));
#endif
}
