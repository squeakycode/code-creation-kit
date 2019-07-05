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
#include <string>
#include "CGenerator.h"
#include "CErrorPrinter.gen.h"
#include "TErrorPrinterTestFiles.h"

using namespace code_creation_kit;

BOOST_AUTO_TEST_CASE( TErrorPrinter)
{
    BOOST_CHECK_NO_THROW(CreateTErrorPrinterFiles());

    typedef std::string StringT;
    typedef std::vector<StringT> ParameterListT;

    CGenerator<StringT> generatorImpl;
    CErrorPrinter<StringT, CGenerator<StringT> > generator( generatorImpl);

    //bad comment chars
    BOOST_CHECK_THROW( generator.setCsvCommentChars( "\"");, CErrorPrinted);
    //bad delimiter
    BOOST_CHECK_THROW( generator.setCsvDelimiter( '\"'), CErrorPrinted);
    generator.setCsvDelimiter( ';');
    //require delimiting char
    BOOST_CHECK_THROW( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RequireDelimitingChar.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //unexpected quote
    BOOST_CHECK_THROW( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "UnexpectedQuote.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //cannot open table file
    BOOST_CHECK_THROW( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "NotThere.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //row overflow
    BOOST_CHECK_THROW( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RowOverflow.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);
    //row underflow
    BOOST_CHECK_THROW( generator.loadTable( CCK_TEST_INPUT_FILE_PREFIX "RowUnderflow.csv", "LabelA", true, true, 1, 1, false), CErrorPrinted);

    //unloaded file not found
    BOOST_CHECK_THROW( generator.unloadTable( "not there"), CErrorPrinted);

    //cannot open template file
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "NotThere.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open target file
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", "BadDir/TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open intermediate output file
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", true, false, "BadDir/TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //failed to move intermediate file
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", "BadDir/TargetFile.txt", true, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //bad parameter
    {
        ParameterListT parameterList;
        parameterList.push_back("a=4");
        parameterList.push_back("b-5");
        BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
    }

    //cyclix inclusion
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "CyclicInclusion.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //infinite loop
    {
        ParameterListT parameterList;
        parameterList.push_back("loop=[ENTRY][\"loop\"]");
        BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "InfiniteLoop.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
        generator.reset();
    }

    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "PrefixLeadingWS.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "PostfixTrailingWS.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "ErrorTag.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( CCK_TEST_INPUT_FILE_PREFIX "BadSetRecursionLevelLimit.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //bad chars passed for padding
    BOOST_CHECK_THROW(generator.generate( CCK_TEST_INPUT_FILE_PREFIX "BadCharsPassedForPadding.tpl.txt", CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt", false, false, CCK_TEST_INPUT_FILE_PREFIX "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
}

