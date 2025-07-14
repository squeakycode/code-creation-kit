// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "TemplateLoader.h"
#include <string>
#include <sstream>
#include "TTemplateLoaderTestFiles.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

using namespace code_creation_kit;

///simulates triggering include of another file by stream processing
class TestHelper
{
public:
    TestHelper( TemplateLoader<TestHelper, std::string>& loader): m_loader( loader) {}
    template <typename T>
    void operator << (const T& text)
    {
        m_loader.loadTemplateFile( text);
    }

    TemplateLoader<TestHelper, std::string>& m_loader;
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

TEST_CASE( "TTemplateLoader", "[TTemplateLoader]")
{
    REQUIRE(CreateDirForTesting("InclusionTest"));
    CHECK_NOTHROW(CreateTTemplateLoaderFiles());

    typedef std::string StringT;
    {
        TemplateLoader<std::stringstream, StringT> loader;

        //check result of loading last line with new line
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest1.txt");
            StringT s = str.str();
            CHECK(s == "a\nb\nc\n");
            CHECK(loader.getInclusionHierarchy().size() == 0);
        }

        //check result of loading last line without new line
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest2.txt");
            StringT s = str.str();
            CHECK(s == "x\ny\nz");
            CHECK(loader.getInclusionHierarchy().size() == 0);
        }

        //check load using include directory
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.addIncludeDirectory("InclusionTest");
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest5.txt");
            StringT s = str.str();
            CHECK(s == CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest4.txt");
            CHECK(loader.getInclusionHierarchy().size() == 0);
        }
    }

    //check trigger loading another file by output processing, ends in cyclic inclusion error
    {
        typedef TemplateLoader<TestHelper, StringT> LoaderT;
        TemplateLoader<TestHelper, StringT> loader;
        TestHelper helper( loader);
        loader.connectOutputStream( &helper);
        CHECK_THROWS_AS( loader.loadTemplateFile( CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest3.txt"), TemplateLoaderExceptions::ExCyclicInclusion);
        REQUIRE( loader.getInclusionHierarchy().size() == 3 );
        std::string expected[] = {CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest3.txt", "InclusionTest/" CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest4.txt", "InclusionTest/" CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest5.txt"};

        int count = 0;
        for ( const LoaderT::FileDataListT::value_type& filedata : loader.getInclusionHierarchy())
        {
            CHECK( filedata.name == expected[count++]);
        }

        loader.reset();
        CHECK( loader.getInclusionHierarchy().size() == 0);
    }
}
