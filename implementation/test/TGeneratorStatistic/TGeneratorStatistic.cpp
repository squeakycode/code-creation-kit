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
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include "CGeneratorStatistic.h"

using namespace code_creation_kit;

BOOST_AUTO_TEST_CASE( TGeneratorStatistic)
{
    // standard
    {
        typedef std::string StringT;
        CGeneratorStatistic<StringT> generator;
        double dummy = 0;

        generator.loadTable( "a", "LabelA", true, true, 1, 1, false);
        generator.loadTable( "a", "LabelB", true, true, 1, 1, false);
        generator.addIncludeDirectory("IncludeDirectory");
        generator.generate( "test1.tpl.txt", "out.txt", false, false, "dummy", false, dummy, CInlineTemplateParameters<StringT>());

        BOOST_REQUIRE( generator.getTableFiles().size() == 1);
        BOOST_REQUIRE( generator.getGeneratedFiles().size() == 1);
        BOOST_REQUIRE( generator.getTemplateFiles().size() == 2);

        BOOST_CHECK( *generator.getTableFiles().begin() == "a");
        BOOST_CHECK( *generator.getGeneratedFiles().begin() == "out.txt");
        BOOST_CHECK( *generator.getTemplateFiles().begin() == "IncludeDirectory/test1.tpl.txt");
        BOOST_CHECK( *(++generator.getTemplateFiles().begin()) == "test2.tpl.txt");
    }
    // inline
    {
        typedef std::string StringT;
        CGeneratorStatistic<StringT> generator;
        double dummy = 0;

        generator.loadTable( "a", "LabelA", true, true, 1, 1, false);
        generator.loadTable( "a", "LabelB", true, true, 1, 1, false);
        generator.addIncludeDirectory("IncludeDirectory");
        generator.generate( "test1Inline.tpl.txt", "out.txt", false, false, "dummy", false, dummy, CInlineTemplateParameters<StringT>( true, "$$$", "", "&&&&", 0));

        BOOST_REQUIRE( generator.getTableFiles().size() == 1);
        BOOST_REQUIRE( generator.getGeneratedFiles().size() == 1);
        BOOST_REQUIRE( generator.getTemplateFiles().size() == 2);

        BOOST_CHECK( *generator.getTableFiles().begin() == "a");
        BOOST_CHECK( *generator.getGeneratedFiles().begin() == "out.txt");
        BOOST_CHECK( *generator.getTemplateFiles().begin() == "IncludeDirectory/test1Inline.tpl.txt");
        BOOST_CHECK( *(++generator.getTemplateFiles().begin()) == "test2.tpl.txt");
    }
}