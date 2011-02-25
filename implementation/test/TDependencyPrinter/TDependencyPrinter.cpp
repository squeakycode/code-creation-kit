//   Copyright (C) 2011 Andreas Gau
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
#include "boost/test/unit_test.hpp"

#include <string>
#include <set>
#include <sstream>
#include "DependencyPrinter.h"
#include "StringLiteral.h"

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

        BOOST_CHECK( result == STRING_LITERAL("a.agen >> ../../x/y/z/b.txt \"d:/a/b/c/a; .txt\" << a.csv c/b.csv ../../b.tpl ../a.tpl"));
    }


    { //check print vs
        StreamT stream;
        DependencyPrinter::printVs( tables, generatedFiles, templateFiles, StringT(STRING_LITERAL("c:/a/b/a.agen")), stream);
        StringT result = stream.str();

        BOOST_CHECK( result == STRING_LITERAL("Parameters for creating a custom build step for file:\na.agen\n\nCommand Line:\n$(CCK_ROOT)\\bin\\ccktc.exe \"$(InputPath)\"\n\nDescription:\nInvoking code-creation-kit text compiler on a.agen\n\nOutputs:\n../../x/y/z/b.txt;\"d:/a/b/c/a; .txt\"\n\nAdditional Dependencies:\na.csv;c/b.csv;../../b.tpl;../a.tpl\n\n"));
    }
}


BOOST_AUTO_TEST_CASE( TDependencyPrinter)
{
    run_test<std::string>();
    run_test<std::wstring>();
}