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

#include "CTemplateLoader.h"
#include <string>
#include <sstream>
#include <boost/foreach.hpp>
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
    TestHelper( CTemplateLoader<TestHelper, std::string>& loader): m_loader( loader) {}
    template <typename T>
    void operator << (const T& text)
    {
        m_loader.loadTemplateFile( text);
    }

    CTemplateLoader<TestHelper, std::string>& m_loader;
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

BOOST_AUTO_TEST_CASE( TTemplateLoader)
{
    BOOST_REQUIRE(CreateDirForTesting("InclusionTest"));
    BOOST_CHECK_NO_THROW(CreateTTemplateLoaderFiles());

    typedef std::string StringT;
    {
        CTemplateLoader<std::stringstream, StringT> loader;

        //check result of loading last line with new line
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest1.txt");
            StringT s = str.str();
            BOOST_CHECK(s == "a\nb\nc\n");
            BOOST_CHECK(loader.getInclusionHierarchy().size() == 0);
        }

        //check result of loading last line without new line
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest2.txt");
            StringT s = str.str();
            BOOST_CHECK(s == "x\ny\nz");
            BOOST_CHECK(loader.getInclusionHierarchy().size() == 0);
        }

        //check load using include directory
        {
            std::stringstream str;
            loader.connectOutputStream(&str);
            loader.addIncludeDirectory("InclusionTest");
            loader.loadTemplateFile(CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest5.txt");
            StringT s = str.str();
            BOOST_CHECK(s == CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest4.txt");
            BOOST_CHECK(loader.getInclusionHierarchy().size() == 0);
        }
    }

    //check trigger loading another file by output processing, ends in cyclic inclusion error
    {
        typedef CTemplateLoader<TestHelper, StringT> LoaderT;
        CTemplateLoader<TestHelper, StringT> loader;
        TestHelper helper( loader);
        loader.connectOutputStream( &helper);
        BOOST_CHECK_THROW( loader.loadTemplateFile( CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest3.txt"), CTemplateLoaderExceptions::ExCyclicInclusion);
        BOOST_REQUIRE( loader.getInclusionHierarchy().size() == 3 );
        std::string expected[] = {CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest3.txt", "InclusionTest/" CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest4.txt", "InclusionTest/" CCK_TEST_INPUT_FILE_PREFIX "TemplateLoaderTest5.txt"};

        int count = 0;
        BOOST_FOREACH( const LoaderT::FileDataListT::value_type& filedata, loader.getInclusionHierarchy())
        {
            BOOST_CHECK( filedata.name == expected[count++]);
        }

        loader.reset();
        BOOST_CHECK( loader.getInclusionHierarchy().size() == 0);
    }
}
