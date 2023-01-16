//  Copyright (c) 2011-2023 Andreas Gau
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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <vector>
#include "CGeneratorStatistic.h"
#include "TGeneratorStatisticTestFiles.h"

using namespace code_creation_kit;

TEST_CASE( "TGeneratorStatistic", "[TGeneratorStatistic]")
{
    REQUIRE(CreateDirForTesting("IncludeDirectory"));
    CHECK_NOTHROW(CreateTGeneratorStatisticFiles());

    // standard
    {
        typedef std::string StringT;
        CGeneratorStatistic<StringT> generator;
        double dummy = 0;

        generator.loadTable( "a", "LabelA", true, true, 1, 1, false);
        generator.loadTable( "a", "LabelB", true, true, 1, 1, false);
        generator.addIncludeDirectory("IncludeDirectory");
        generator.generate( CCK_TEST_INPUT_FILE_PREFIX "test1.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "out.txt", false, false, "dummy", false, dummy, false, CInlineTemplateParameters<StringT>());

        REQUIRE( generator.getTableFiles().size() == 1);
        REQUIRE( generator.getGeneratedFiles().size() == 1);
        REQUIRE( generator.getTemplateFiles().size() == 2);

        CHECK( *generator.getTableFiles().begin() == "a");
        CHECK( *generator.getGeneratedFiles().begin() == CCK_TEST_INPUT_FILE_PREFIX "out.txt");
        CHECK( *generator.getTemplateFiles().begin() == "IncludeDirectory/" CCK_TEST_INPUT_FILE_PREFIX "test1.tpl.txt");
        CHECK( *(++generator.getTemplateFiles().begin()) == CCK_TEST_INPUT_FILE_PREFIX "test2.tpl.txt");
    }
    // inline
    {
        typedef std::string StringT;
        CGeneratorStatistic<StringT> generator;
        double dummy = 0;

        generator.loadTable( "a", "LabelA", true, true, 1, 1, false);
        generator.loadTable( "a", "LabelB", true, true, 1, 1, false);
        generator.addIncludeDirectory("IncludeDirectory");
        generator.generate( CCK_TEST_INPUT_FILE_PREFIX "test1Inline.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "out.txt", false, false, "dummy", false, dummy, false, CInlineTemplateParameters<StringT>( true, "$$$", "", "&&&&", 0));

        REQUIRE( generator.getTableFiles().size() == 1);
        REQUIRE( generator.getGeneratedFiles().size() == 1);
        REQUIRE( generator.getTemplateFiles().size() == 2);

        CHECK( *generator.getTableFiles().begin() == "a");
        CHECK( *generator.getGeneratedFiles().begin() == CCK_TEST_INPUT_FILE_PREFIX "out.txt");
        CHECK( *generator.getTemplateFiles().begin() == "IncludeDirectory/" CCK_TEST_INPUT_FILE_PREFIX "test1Inline.tpl.txt");
        CHECK( *(++generator.getTemplateFiles().begin()) == CCK_TEST_INPUT_FILE_PREFIX "test2.tpl.txt");
    }
}
