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
#include <boost/test/unit_test.hpp>

#include <string>
#include "CGenerator.h"

BOOST_AUTO_TEST_CASE( TGenerator)
{
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable( "TGenerator.xls.csv", "LabelA", true, true, 1, 1);

        //test unload feature
        generator.loadTable( "TGenerator.xls.csv", "LabelB", true, true, 1, 1);
        generator.unloadTable( "LabelB");

        //create parameter list
        std::vector<std::string> parameterList;
        parameterList.push_back("a=4");
        parameterList.push_back("b=5");

        //generate the output
        generator.generate( "../TGenerator/TGeneratorTemplate.txt", "TGeneratorOut.txt", false, parameterList);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TGeneratorOut.txt", "TGeneratorOutExpected.txt"));

        //generate the output using intermediate file
        generator.generate( "TGeneratorTemplate.txt", "TGeneratorOutIntermediateUsed.txt", true, "TGeneratorOutIntermediateUsed.txt.intermediate", false, parameterList);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TGeneratorOutIntermediateUsed.txt", "TGeneratorOutExpected.txt"));
    }
    //data flow test
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable( "TDataflow.csv", "LabelA", true, true, 1, 1);

        //generate the output
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TDataflow.gen.txt");

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TDataflow.gen.txt", "TDataflowExpected.txt"));
    }

    //test append
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable( "TDataflow.csv", "LabelA", true, true, 1, 1);

        //generate the output
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TAppend.gen.txt", false);
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TAppend.gen.txt", true);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TAppend.gen.txt", "TAppendExpected.txt"));
    }

}