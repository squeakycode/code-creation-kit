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
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include "CGeneratorStatistic.h"

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