// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <string>
#include "System.h"

using namespace code_creation_kit;

template <typename StringT>
void run_test()
{
   typedef typename StringT::value_type CharT;

#ifdef _MSC_VER
   _putenv("TEST_VARIABLE=123abc");
#else
    setenv("TEST_VARIABLE","123abc",1);
#endif

    CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE)")) == STRING_LITERAL("123abc"));
    CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start$(TEST_VARIABLE)end")) == STRING_LITERAL("start123abcend"));
    CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start1$(TEST_VARIABLE)end1start2$(TEST_VARIABLE)end2")) == STRING_LITERAL("start1123abcend1start2123abcend2"));
    CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE222222222222)")) == STRING_LITERAL("$(TEST_VARIABLE222222222222)"));
    CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE )")) == STRING_LITERAL("$(TEST_VARIABLE )"));
}

TEST_CASE( "TSystem", "[TSystem]")
{
    run_test<std::string>();
#ifdef _MSC_VER
    run_test<std::wstring>();
#endif
}

