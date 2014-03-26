//   Copyright (C) 2011-2014 Andreas Gau
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
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include "CTemplateLoader.h"
#include <string>
#include <sstream>
#include <boost/foreach.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

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
    typedef std::string StringT;
    CTemplateLoader<std::stringstream, StringT> loader;

    //check result of loading last line with new line
    {
        std::stringstream str;
        loader.connectOutputStream( &str);
        loader.loadTemplateFile( "TemplateLoaderTest1.txt");
        StringT s = str.str();
#ifdef _MSC_VER //TODO
        BOOST_CHECK( s == "a\nb\nc\n");
#else
        BOOST_CHECK( s == "a\r\nb\r\nc\r\n");
#endif
        BOOST_CHECK( loader.getInclusionHierarchy().size() == 0 );
    }

    //check result of loading last line without new line
    {
        std::stringstream str;
        loader.connectOutputStream( &str);
        loader.loadTemplateFile( "TemplateLoaderTest2.txt");
        StringT s = str.str();
#ifdef _MSC_VER //TODO
        BOOST_CHECK( s == "x\ny\nz");
#else
        BOOST_CHECK( s == "x\r\ny\r\nz");
#endif
        BOOST_CHECK( loader.getInclusionHierarchy().size() == 0 );
    }

    //check load using include directory
    {
        std::stringstream str;
        loader.connectOutputStream( &str);
        loader.addIncludeDirectory( "InclusionTest");
        loader.loadTemplateFile( "TemplateLoaderTest5.txt");
        StringT s = str.str();
        BOOST_CHECK( s == "TemplateLoaderTest4.txt");
        BOOST_CHECK( loader.getInclusionHierarchy().size() == 0 );
    }

    //check trigger loading another file by output processing, ends in cyclic inclusion error
    {
        typedef CTemplateLoader<TestHelper, StringT> LoaderT;
        CTemplateLoader<TestHelper, StringT> loader;
        TestHelper helper( loader);
        loader.connectOutputStream( &helper);
        BOOST_CHECK_THROW( loader.loadTemplateFile( "TemplateLoaderTest3.txt"), CTemplateLoaderExceptions::ExCyclicInclusion);
        BOOST_REQUIRE( loader.getInclusionHierarchy().size() == 3 );
        std::string expected[] = {"TemplateLoaderTest3.txt", "InclusionTest/TemplateLoaderTest4.txt", "InclusionTest/TemplateLoaderTest5.txt"};

        int count = 0;
        BOOST_FOREACH( const LoaderT::FileDataListT::value_type& filedata, loader.getInclusionHierarchy())
        {
            BOOST_CHECK( filedata.name == expected[count++]);
        }

        loader.reset();
        BOOST_CHECK( loader.getInclusionHierarchy().size() == 0);        
    }
}
