// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "FilesBinaryEqual.h"
#include <string>
#include "TFilesBinaryEqualTestFiles.h"

using namespace code_creation_kit;

TEST_CASE( "TFilesBinaryEqual", "[TFilesBinaryEqual]")
{
    CHECK_NOTHROW(CreateTFilesBinaryEqualFiles());

    CHECK( FilesBinaryEqual<std::string>( CCK_TEST_INPUT_FILE_PREFIX "a.txt", CCK_TEST_INPUT_FILE_PREFIX "a_.txt"));
    CHECK( !FilesBinaryEqual<std::string>( CCK_TEST_INPUT_FILE_PREFIX "a.txt", CCK_TEST_INPUT_FILE_PREFIX "b.txt"));
#ifdef WIN32
    CHECK( FilesBinaryEqual<std::wstring>( CCK_TEST_INPUT_FILE_PREFIX_WCHAR L"a.txt", CCK_TEST_INPUT_FILE_PREFIX_WCHAR L"a_.txt"));
    CHECK( !FilesBinaryEqual<std::wstring>( CCK_TEST_INPUT_FILE_PREFIX_WCHAR L"a.txt", CCK_TEST_INPUT_FILE_PREFIX_WCHAR L"b.txt"));
#endif
}
