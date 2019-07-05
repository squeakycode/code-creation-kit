//  Copyright (c) 2011-2015 Andreas Gau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//      * Neither the name of the copyright holder nor the
//        names of contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

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
#ifdef _MSC_VER //TODO
    run_test<std::string>();
    run_test<std::wstring>();
#endif
}
