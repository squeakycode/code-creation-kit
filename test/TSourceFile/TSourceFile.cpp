// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "SourceFile.h"
#include "TSourceFileTestFiles.h"
#include "StringConvert.h"

using namespace code_creation_kit;

///represents streams in test
template <typename StringT>
class TStreamHelper
{
public:
    template <typename InputT>
    TStreamHelper<StringT>& operator <<(const InputT& text)
    {
        result.push_back( StringConvert<StringT>(text));
        return *this;
    }
    std::vector<StringT> result; 
};

class TestFile;

template <typename StringT>
void run_test()
{
    CHECK_NOTHROW(CreateTSourceFileFiles());

    typedef TStreamHelper<StringT> SinkT;
    typedef SourceFile<StringT,TestFile> SourceFileT;

    {
        SinkT result;
        SinkT expected;
        int counter = 0;
        SourceFileT( StringConvert<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "a.txt")).feedLineSink( result, true, counter);
        CHECK( counter == 0);
        CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a";
        int counter = 0;
        SourceFileT( StringConvert<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "b.txt")).feedLineSink( result, true, counter);
        CHECK( counter == 1);
        CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n";
        int counter = 0;
        SourceFileT( StringConvert<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "c.txt")).feedLineSink( result, true, counter);
        CHECK( counter == 1);
        CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n" << "b";

        int counter = 0;
        SourceFileT( StringConvert<StringT>(CCK_TEST_INPUT_FILE_PREFIX "d.txt")).feedLineSink( result, true, counter);
        CHECK( counter == 2);
        CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a" << "b";
        int counter = 0;
        SourceFileT( StringConvert<StringT>(CCK_TEST_INPUT_FILE_PREFIX "d.txt")).feedLineSink( result, false, counter);
        CHECK( counter == 2);
        CHECK( result.result == expected.result);
    }
}

TEST_CASE( "TSourceFile", "[TSourceFile]")
{
    run_test<std::string>();
#ifdef _MSC_VER
    run_test<std::wstring>();
#endif
}

