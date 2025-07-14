// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <set>
#include <sstream>
#include "DependencyPrinter.h"
#include "StringLiteral.h"

using namespace code_creation_kit;

template <typename StringT>
void run_test()
{
    typedef typename StringT::value_type CharT;
    typedef std::set<StringT> FileSetT;
    typedef std::basic_stringstream<typename StringT::value_type> StreamT;

    FileSetT tables;
    FileSetT generatedFiles;
    FileSetT templateFiles;

    tables.insert( STRING_LITERAL("c:/a/b/a.csv"));
    tables.insert( STRING_LITERAL("c:/a/b/c/b.csv"));

    generatedFiles.insert( STRING_LITERAL("d:/a/b/c/a; .txt"));
    generatedFiles.insert( STRING_LITERAL("c:/x/y/z/b.txt"));

    templateFiles.insert( STRING_LITERAL("c:/a/a.tpl"));    
    templateFiles.insert( STRING_LITERAL("c:/b.tpl"));

    { //check print mpc
        StreamT stream;
        DependencyPrinter::printMpc( tables, generatedFiles, templateFiles, StringT(STRING_LITERAL("c:/a/b/a.agen")), stream);
        StringT result = stream.str();

        CHECK( result == STRING_LITERAL("a.agen >> ../../x/y/z/b.txt \"d:/a/b/c/a; .txt\" << a.csv c/b.csv ../../b.tpl ../a.tpl"));
    }


    { //check print vs
        StreamT stream;
        DependencyPrinter::printVs( tables, generatedFiles, templateFiles, StringT(STRING_LITERAL("c:/a/b/a.agen")), stream);
        StringT result = stream.str();

        CHECK( result == STRING_LITERAL("Parameters for creating a custom build step for file:\na.agen\n\nCommand Line:\n$(CCK_ROOT)\\bin\\ccktc.exe \"$(InputPath)\"\n\nDescription:\nInvoking code-creation-kit text compiler on a.agen\n\nOutputs:\n../../x/y/z/b.txt;\"d:/a/b/c/a; .txt\"\n\nAdditional Dependencies:\na.csv;c/b.csv;../../b.tpl;../a.tpl\n\n"));
    }
}


TEST_CASE( "TDependencyPrinter", "[TDependencyPrinter]")
{
#ifdef _MSC_VER //TODO
    run_test<std::string>();
    run_test<std::wstring>();
#endif
}
