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
#define CODE_CREATION_KIT_JUST_TESTING
#include "CGenerator.h"
#include "CErrorPrinter.gen.h"
#include "TErrorPrinterTestFiles.h"

using namespace code_creation_kit;

TEST_CASE( "TErrorPrinter", "[TErrorPrinter]")
{
    CHECK_NOTHROW(CreateTErrorPrinterFiles());

    typedef std::string StringT;
    typedef std::vector<StringT> ParameterListT;

    CGenerator<StringT> generatorImpl;
    CErrorPrinter<StringT, CGenerator<StringT> > generator( generatorImpl);

    //bad comment chars
    CHECK_THROWS_AS( generator.setCsvCommentChars( "\""), CErrorPrinted);
    //bad delimiter
    CHECK_THROWS_AS( generator.setCsvDelimiterChars( "\""), CErrorPrinted);
    //bad quote
    CHECK_THROWS_AS(generator.setCsvQuoteChars(";"), CErrorPrinted);
    //require delimiting char
    CHECK_THROWS_AS( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RequireDelimitingChar.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //unexpected quote
    CHECK_THROWS_AS( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "UnexpectedQuote.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //cannot open table file
    CHECK_THROWS_AS( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "NotThere.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //row overflow
    CHECK_THROWS_AS( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RowOverflow.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //row underflow
    CHECK_THROWS_AS( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RowUnderflow.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //label already in use
    {
        CHECK_NOTHROW(generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "RowUnderflow.csv", "LabelA", true, true, 1, 1, true /*pad-rows*/));
        CHECK_THROWS_AS(generator.loadTable(CCK_TEST_INPUT_FILE_PREFIX "RowUnderflow.csv", "LabelA", true, true, 1, 1, true /*pad-rows*/), CErrorPrinted);
        CHECK_NOTHROW(generator.unloadTable("LabelA"));
    }

    //unloaded file not found
    CHECK_THROWS_AS( generator.unloadTable( "not there"), CErrorPrinted);

    //cannot open template file
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "NotThere.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open target file
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", "BadDir/TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open intermediate output file
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", true, false, "BadDir/TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //failed to move intermediate file
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", "BadDir/TargetFile.txt", true, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //bad parameter
    {
        ParameterListT parameterList;
        parameterList.push_back("a=4");
        parameterList.push_back("b-5");
        CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
    }

    //cyclix inclusion
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "CyclicInclusion.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //infinite loop
    {
        ParameterListT parameterList;
        parameterList.push_back("loop=[ENTRY][\"loop\"]");
        CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "InfiniteLoop.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
        generator.reset();
    }

    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "PrefixLeadingWS.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "PostfixTrailingWS.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "ErrorTag.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    CHECK_THROWS_AS( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "BadSetRecursionLevelLimit.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //bad chars passed for padding
    CHECK_THROWS_AS(generator.generate( CCK_TEST_INPUT_FILE_PREFIX "BadCharsPassedForPadding.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //table block errors
    {
        for (int i = 1; i <= 19; ++i)
        {
            if (i == 2 || i == 7 || i == 9) //gaps
            {
                continue;
            }

            generator.setMarkup(std::string("<") + std::to_string(i) + ":>", std::string("<:") + std::to_string(i) + ">");
            CHECK_THROWS_AS(generator.generate(CCK_TEST_INPUT_FILE_PREFIX "TableBlockErrors.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
        }
        generator.setMarkup("[", "]");
    }
}
