// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <vector>
#include "GeneratorStatistic.h"
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
