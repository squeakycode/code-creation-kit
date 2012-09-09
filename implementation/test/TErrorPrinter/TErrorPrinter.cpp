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
#include "CGenerator.h"
#include "CErrorPrinter.gen.h"

BOOST_AUTO_TEST_CASE( TErrorPrinter)
{
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
    BOOST_CHECK_THROW( generator.loadTable( "RequireDelimitingChar.csv", "LabelA", true, true, 1, 1), CErrorPrinted);
    //unexpected quote
    BOOST_CHECK_THROW( generator.loadTable( "UnexpectedQuote.csv", "LabelA", true, true, 1, 1), CErrorPrinted);
    //cannot open table file
    BOOST_CHECK_THROW( generator.loadTable( "NotThere.csv", "LabelA", true, true, 1, 1), CErrorPrinted);
    //row overflow
    BOOST_CHECK_THROW( generator.loadTable( "RowOverflow.csv", "LabelA", true, true, 1, 1), CErrorPrinted);
    //row underflow
    BOOST_CHECK_THROW( generator.loadTable( "RowUnderflow.csv", "LabelA", true, true, 1, 1), CErrorPrinted);

    //unloaded file not found
    BOOST_CHECK_THROW( generator.unloadTable( "not there"), CErrorPrinted);

    //cannot open template file
    BOOST_CHECK_THROW( generator.generate( "NotThere.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open target file
    BOOST_CHECK_THROW( generator.generate( "Template.tpl.txt", "BadDir/TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //cannot open intermediate output file
    BOOST_CHECK_THROW( generator.generate( "Template.tpl.txt", "TargetFile.txt", true, "BadDir/TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    //failed to move intermediate file
    BOOST_CHECK_THROW( generator.generate( "Template.tpl.txt", "BadDir/TargetFile.txt", true, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //bad parameter
    {
        ParameterListT parameterList;
        parameterList.push_back("a=4");
        parameterList.push_back("b-5");
        BOOST_CHECK_THROW( generator.generate( "Template.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
    }

    BOOST_CHECK_THROW( generator.generate( "CyclicInclusion.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);

    //infinite loop
    {
        ParameterListT parameterList;
        parameterList.push_back("loop=[ENTRY][\"loop\"]");
        BOOST_CHECK_THROW( generator.generate( "InfiniteLoop.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, parameterList), CErrorPrinted);
        generator.reset();
    }

    BOOST_CHECK_THROW( generator.generate( "PrefixLeadingWS.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( "PostfixTrailingWS.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( "ErrorTag.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
    BOOST_CHECK_THROW( generator.generate( "BadSetRecursionLevelLimit.tpl.txt", "TargetFile.txt", false, "TargetFile.txt.intermediate", false, ParameterListT()), CErrorPrinted);
}

