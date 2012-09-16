//   Copyright (C) 2011-2012 Andreas Gau
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
#include <sstream>
#include "CGenerator.h"

BOOST_AUTO_TEST_CASE( TGenerator)
{
    {
        CGenerator<std::string> generator;

        generator.setCsvCommentChars("#");

        //load a table
        generator.loadTable( "TGenerator.xls.csv", "LabelA", true, true, 1, 1, false);

        //test unload feature
        generator.loadTable( "TGenerator.xls.csv", "LabelB", true, true, 1, 1, false);
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
        generator.loadTable( "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TDataflow.gen.txt");

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TDataflow.gen.txt", "TDataflowExpected.txt"));
    }

    //test append
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable( "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TAppend.gen.txt", false);
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TAppend.gen.txt", true);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TAppend.gen.txt", "TAppendExpected.txt"));
    }

    //test streams
    {
        typedef std::vector<std::vector<std::string> > TableT;
        std::stringstream in;
        std::stringstream out;
        std::stringstream csv, csv2;
        boost::shared_ptr<TableT> psTable( new TableT(2));

        (*psTable)[0].push_back("b");
        (*psTable)[0].push_back("7");
        (*psTable)[1].push_back("a");
        (*psTable)[1].push_back("0");

        csv << "a;b;\n1;1;\n2;;\n3;3;\n;4;\n";

        in << "start" << std::endl;
        in << "[ENTRY][\"a\"][ENTRY][\"b\"]" << std::endl;
        in << "end";

        CGenerator<std::string> generator;

        //load a table
        generator.loadTable( csv, "LabelA", true, true, 1, 1, false);

        csv2  << "a;b;\n1;6;\n8;;\n3;3;\n;4;\n";

        //test unload feature
        generator.loadTable( csv2, "LabelB", true, true, 1, 1, false);
        generator.unloadTable( "LabelB");

        generator.loadTable( psTable, "LabelB", true, true, 1, 1);

        generator.generate( in, out);

        BOOST_CHECK_EQUAL( out.str(), "start\n11\n33\n07\nend");
    }

    //test inline
    {
        CGenerator<std::string> generator;
        std::vector<std::string> parameterList;
        CInlineTemplateParameters<std::string> itp( true, "//<>", "//>", "//$", 3);

        generator.setCsvCommentChars("#");

        //load a table
        generator.loadTable( "TGenerator.xls.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate(
            "../TGenerator/TGeneratorTemplateInline.txt",
            "../TGenerator/TGeneratorTemplateInline.txt",
            true,
            "../TGenerator/TGeneratorTemplateInline.txt.intermediate",
            false,
            parameterList,
            itp);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "../TGenerator/TGeneratorTemplateInline.txt", "TGeneratorOutInlineExpected.txt"));

        parameterList.push_back("a=4");
        parameterList.push_back("b=5");

        //generate the output
        generator.generate(
            "../TGenerator/TGeneratorTemplateInline.txt",
            "../TGenerator/TGeneratorTemplateInline.txt",
            true,
            "../TGenerator/TGeneratorTemplateInline.txt.intermediate",
            false,
            parameterList,
            itp);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "../TGenerator/TGeneratorTemplateInline.txt", "TGeneratorOutInlineExpected2.txt"));
    }

    //table with padding test
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable( "NeedsPadding.csv", "LabelA", true, true, 0, 0, true);

        //generate the output
        generator.generate( "../TGenerator/NeedsPaddingDuplicate.tpl", "NeedsPadding.gen.csv");

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "NeedsPadding.csv", "NeedsPadding.gen.csv"));
    }
}