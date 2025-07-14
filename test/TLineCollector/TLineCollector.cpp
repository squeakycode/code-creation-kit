// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "LineCollector.h"

using namespace code_creation_kit;

///represents output stream in test
template <typename StringT>
class THelper
{
public:
    THelper<StringT>& operator <<(const StringT& text)
    {
        result.push_back(text);
        return *this;
    }
    std::vector<StringT> result; 
};

TEST_CASE( "TLineCollector", "[TLineCollector]")
{
    //types in test
    typedef std::string StringT;
    typedef THelper<StringT> OutputStreamT;
    typedef LineCollector<StringT,OutputStreamT> CollectorT;

    //create objects
    OutputStreamT lines;
    OutputStreamT expectedLines;
    CollectorT collector;

    //connect object
    collector.connectOutputStream( &lines);

    //feed text snippets
    collector
        << StringT("ab")
        << StringT("c\ndef")
        << StringT("\ngh")
        << StringT("ijk\nlmn")
        ;

    collector.close();

    //fill expected output
    expectedLines
        << "abc\n" 
        << "def\n"
        << "ghijk\n"
        << "lmn"
        ;

    //compare equal
    CHECK( expectedLines.result == lines.result);
}
