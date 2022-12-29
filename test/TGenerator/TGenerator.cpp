//  Copyright (c) 2011-2019 Andreas Gau
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
#include <sstream>
#include "CGenerator.h"
#include "TGeneratorTestFiles.h"

using namespace code_creation_kit;

class LogFileT;

TEST_CASE( "TGenerator", "[TGenerator]")
{
    CHECK_NOTHROW(CreateTGeneratorFiles());

    {
        CGenerator<std::string> generator;

        generator.setCsvCommentChars("#");

        //load a table
        generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "TGenerator.xls.csv", "LabelA", true, true, 1, 1, false);

        //test unload feature
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "TGenerator.xls.csv", "LabelB", true, true, 1, 1, false);
        generator.unloadTable( "LabelB");

        //create parameter list
        std::vector<std::string> parameterList;
        parameterList.push_back("a=4");
        parameterList.push_back("b=5");

        //generate the output
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplate.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOut.txt", false, parameterList);

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOut.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutExpected.txt"));

        //generate the output using intermediate file
        generator.generate(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplate.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutIntermediateUsed.txt", true, false, CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutIntermediateUsed.txt.intermediate", false, parameterList, false);

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutIntermediateUsed.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutExpected.txt"));
    }

    //data flow test
    {
        CGenerator<std::string, std::ostream> generator;

        //load a table
        generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TDataflow.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TDataflow.gen.txt");

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.gen.txt", CCK_TEST_INPUT_FILE_PREFIX "TDataflowExpected.txt"));
    }

    //test append
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TDataflow.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TAppend.gen.txt", false);
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TDataflow.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TAppend.gen.txt", true);

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "TAppend.gen.txt", CCK_TEST_INPUT_FILE_PREFIX "TAppendExpected.txt"));
    }

    //test streams
    {
        typedef std::vector<std::vector<std::string> > TableT;
        std::stringstream in;
        std::stringstream out;
        std::stringstream csv, csv2;
        std::shared_ptr<TableT> psTable( new TableT(2));

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

        CHECK( out.str() == "start\n11\n33\n07\nend");
    }

    //test inline
    {
        CGenerator<std::string> generator;
        std::vector<std::string> parameterList;
        CInlineTemplateParameters<std::string> itp( true, "//<>", "//>", "//$", 3);

        generator.setCsvCommentChars("#");

        //load a table
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "TGenerator.xls.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate(
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt",
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt",
            true,
            false,
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt.intermediate",
            false,
            parameterList,
            false,
            itp);

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutInlineExpected.txt"));

        parameterList.push_back("a=4");
        parameterList.push_back("b=5");

        //generate the output
        generator.generate(
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt",
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt",
            true,
            false,
            "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt.intermediate",
            false,
            parameterList,
            false,
            itp);

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt", CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutInlineExpected2.txt"));
    }

    //table with padding test
    {
        CGenerator<std::string> generator;
        //load a table
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "NeedsPadding.csv", "LabelA", true, true, 0, 0, true);

        //generate the output
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "NeedsPaddingDuplicate.tpl", CCK_TEST_INPUT_FILE_PREFIX "NeedsPadding.gen.csv");

        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "NeedsPadding.csv", CCK_TEST_INPUT_FILE_PREFIX "NeedsPadding.gen.csv"));
    }

    //log test
    {
        CGenerator<std::string, std::ostream> generator;
        CTargetFile<std::string, LogFileT> logFile(CCK_TEST_INPUT_FILE_PREFIX "LogOutput.txt", false);

        //connecting log stream
        generator.connectLogOutputStream( &logFile.get());
        generator.connectLogOutputStream( NULL);
        generator.connectLogOutputStream( &std::cout);
        generator.connectLogOutputStream( &logFile.get());

        //reset
        generator.reset();

        //csv options
        CHECK_NOTHROW(generator.setCsvDelimiterChars( ":"));
        CHECK_NOTHROW(generator.setCsvDelimiterChars( ";"));
        CHECK_NOTHROW(generator.setCsvCommentChars( "#*"));
        CHECK_NOTHROW(generator.setCsvCommentChars( ""));
        CHECK_NOTHROW(generator.setCsvQuoteChars("\'l"));
        CHECK_NOTHROW(generator.setCsvQuoteChars( "\""));

        //include
        generator.addIncludeDirectory("IncludeDirectoryThatDoesNotExist");

        //set markup
        generator.setMarkup( "&", "&");
        generator.setMarkup( "[", "]");

        //load a table
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.csv", "LabelB", true, true, 1, 1, false);
        generator.unloadTable( "LabelB");
        generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "TDataflow.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TDataflow.gen.txt");
        //check output is as expected
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.gen.txt", CCK_TEST_INPUT_FILE_PREFIX "TDataflowExpected.txt"));

        //check extended error output
        CHECK_THROWS_AS( generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "LogTest1.tpl", "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "LogTest1.gen.txt"), std::exception);
        CInlineTemplateParameters<std::string> itp( true, "$", "%", "&", 3);
        CHECK_THROWS_AS( generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "LogTest2.tpl", "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX  "LogTest2.tpl", true, false, "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "LogTest2.tpl.intermediate", false, std::vector<std::string>(), false, itp), std::exception);
        CHECK_THROWS_AS( generator.generate( "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX "LogTest3.tpl", "../TGenerator/" CCK_TEST_INPUT_FILE_PREFIX  "LogTest1.gen.txt"), std::exception);

        generator.connectLogOutputStream( NULL);
        logFile.close();
        CHECK( FilesBinaryEqual<std::string>(CCK_TEST_INPUT_FILE_PREFIX "LogOutput.txt", CCK_TEST_INPUT_FILE_PREFIX "LogOutputExpected.txt"));
    }
}
