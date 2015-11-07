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
#include <sstream>
#include "CGenerator.h"

using namespace code_creation_kit;

class LogFileT;

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
        generator.generate( "TGeneratorTemplate.txt", "TGeneratorOutIntermediateUsed.txt", true, false, "TGeneratorOutIntermediateUsed.txt.intermediate", false, parameterList);

        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TGeneratorOutIntermediateUsed.txt", "TGeneratorOutExpected.txt"));
    }

    //data flow test
    {
        CGenerator<std::string, std::ostream> generator;

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
            false,
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
            false,
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

    //log test
    {
        CGenerator<std::string, std::ostream> generator;
        CTargetFile<std::string, LogFileT> logFile("LogOutput.txt", false);

        //connecting log stream
        generator.connectLogOutputStream( &logFile.get());
        generator.connectLogOutputStream( NULL);
        generator.connectLogOutputStream( &std::cout);
        generator.connectLogOutputStream( &logFile.get());

        //reset
        generator.reset();

        //csv options
        generator.setCsvDelimiter( ':');
        generator.setCsvDelimiter( ';');
        generator.setCsvCommentChars( "#*");
        generator.setCsvCommentChars( "");
        generator.setCsvIgnoreDoubleQuotes( true);
        generator.setCsvIgnoreDoubleQuotes( false);

        //include
        generator.addIncludeDirectory("IncludeDirectoryThatDoesNotExist");

        //set markup
        generator.setMarkup( "&", "&");
        generator.setMarkup( "[", "]");

        //load a table
        generator.loadTable( "TDataflow.csv", "LabelB", true, true, 1, 1, false);
        generator.unloadTable( "LabelB");
        generator.loadTable( "TDataflow.csv", "LabelA", true, true, 1, 1, false);

        //generate the output
        generator.generate( "../TGenerator/TDataflow.tpl.txt", "TDataflow.gen.txt");
        //check output is as expected
        BOOST_CHECK( FilesBinaryEqual<std::string>( "TDataflow.gen.txt", "TDataflowExpected.txt"));

        //check extended error output
        BOOST_CHECK_THROW( generator.generate( "../TGenerator/LogTest1.tpl", "../TGenerator/LogTest1.gen.txt"), std::exception);
        CInlineTemplateParameters<std::string> itp( true, "$", "%", "&", 3);
        BOOST_CHECK_THROW( generator.generate( "../TGenerator/LogTest2.tpl", "../TGenerator/LogTest2.tpl", true, false, "../TGenerator/LogTest2.tpl.intermediate", false, std::vector<std::string>(), itp), std::exception);
        BOOST_CHECK_THROW( generator.generate( "../TGenerator/LogTest3.tpl", "../TGenerator/LogTest1.gen.txt"), std::exception);

        generator.connectLogOutputStream( NULL);
        logFile.close();
#ifdef _MSC_VER //TODO
        BOOST_CHECK( FilesBinaryEqual<std::string>( "LogOutput.txt", "LogOutputExpected.txt"));
#endif
    }
}