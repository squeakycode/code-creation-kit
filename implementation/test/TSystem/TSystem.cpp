//   Copyright (C) 2011-2014 Andreas Gau
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
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>
#include <string>
#include "System.h"

template <typename StringT>
void run_test()
{
   typedef typename StringT::value_type CharT;

#ifdef _MSC_VER //TODO
   _putenv("TEST_VARIABLE=123abc");
#else
    putenv("TEST_VARIABLE=123abc");
#endif

    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE)")) == STRING_LITERAL("123abc"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start$(TEST_VARIABLE)end")) == STRING_LITERAL("start123abcend"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start1$(TEST_VARIABLE)end1start2$(TEST_VARIABLE)end2")) == STRING_LITERAL("start1123abcend1start2123abcend2"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE222222222222)")) == STRING_LITERAL("$(TEST_VARIABLE222222222222)"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE )")) == STRING_LITERAL("$(TEST_VARIABLE )"));
}

BOOST_AUTO_TEST_CASE( TSystem)
{
    run_test<std::string>();
#ifdef _MSC_VER //TODO
    run_test<std::wstring>(); //true wchar support is not implemented
#endif
}

