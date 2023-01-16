//  Copyright (c) 2011-2023 Andreas Gau
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

#include "../include/TestInputFileWriter.h"

inline void CreateTErrorPrinterFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/BadCharsPassedForPadding.tpl.txt
    {
        const char* fileData =
R"([PAD_LEFT]["\t",2])";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "BadCharsPassedForPadding.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/BadSetRecursionLevelLimit.tpl.txt
    {
        const char* fileData =
R"(a[SET_RECURSION_LEVEL_LIMIT]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "BadSetRecursionLevelLimit.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/CyclicInclusion.tpl.txt
    {
        const char* fileData =
R"(




[INCLUDE]["CyclicInclusion.tpl.txt"]




)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "CyclicInclusion.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/ErrorTag.tpl.txt
    {
        const char* fileData =
R"([ERROR]["Error Message 1234."]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "ErrorTag.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/InfiniteLoop.tpl.txt
    {
        const char* fileData =
R"(







[ENTRY]["loop"]

)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "InfiniteLoop.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/PostfixTrailingWS.tpl.txt
    {
        const char* fileData =
R"(a
b
c
[SET_MARKUP]["%","-   "]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "PostfixTrailingWS.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/PrefixLeadingWS.tpl.txt
    {
        const char* fileData =
R"(a
b
c
[SET_MARKUP]["   -","%"]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "PrefixLeadingWS.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/RequireDelimitingChar.csv
    {
        const char* fileData =
R"(;;;
;;"badly quoted" ;
;;;)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "RequireDelimitingChar.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/RowOverflow.csv
    {
        const char* fileData =
R"(;;;
;;;;
;;;)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "RowOverflow.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/RowUnderflow.csv
    {
        const char* fileData =
R"(;;;
;;
;;;)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "RowUnderflow.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/Template.tpl.txt
    {
        const char* fileData =
R"()";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "Template.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/UnexpectedQuote.csv
    {
        const char* fileData =
R"(;;;
;;badly quoted" ;
;;;)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "UnexpectedQuote.csv", fileData);
    }


    ///////////////////////////////////////////////////////////////////////////
    //TErrorPrinter/TableBlockErrors.tpl.txt
    {
        const char* fileData =
R"(#<1:>TABLE_BEGIN<:1>["labelxyz",";","#","unknown-property"]Numbers;1;2;5<1:>TABLE_END<:1><<1:>ENTRY<:1>["Numbers"]>
CTemplateProvidedTableLoaderExceptions::ExUnexpectedTableProperty

#<3:>TABLE_BEGIN<:3>["labelxyz",";",";"];Numbers;1;2;5<3:>TABLE_END<:3><<3:>ENTRY<:3>["Numbers"]>
CCsvParser::ExBadCommentChars

#<4:>TABLE_BEGIN<:4>["labelxyz","\n",";"];Numbers;1;2;5<4:>TABLE_END<:4><<4:>ENTRY<:4>["Numbers"]>
CCsvParser::ExBadDelimiter

#<5:>TABLE_BEGIN<:5>["labelxyz"];Numbers;1;2;5
;Numbers;1;2;5<5:>
;Numbers;1;2;5<5:>
;Numbers;1;2";5<5:>
<5:>TABLE_END<:5><<5:>ENTRY<:5>["Numbers"]>
CCsvParser::ExUnexpectedQuote

#<6:>TABLE_BEGIN<:6>["labelxyz"];Numbers;1;2a;5
;Numbers;1;2a;5
;Numbers;1;"2"a;5
<6:>TABLE_END<:6><<6:>ENTRY<:6>["Numbers"]>
CCsvParser::ExRequireDelimitingChar

a<8:>TABLE_BEGIN<:8>["l"]b
CParserExceptions::ExMissingTableEnd

a<10:>TABLE_END<:10>b
CParserExceptions::ExMissingTableBegin

#<11:>TABLE_BEGIN<:11>["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2
row3;3,1;3,2;3,3
<11:>TABLE_END<:11><<11:>ENTRY<:11>["m"]>
CVerticalTableBuilderExceptions::ExUnderflow

#<12:>TABLE_BEGIN<:12>["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;over;flow
row3;3,1;3,2;3,3
<12:>TABLE_END<:12><<12:>ENTRY<:12>["m"]>
CVerticalTableBuilderExceptions::ExOverflow

#<13:>TABLE_LOAD<:13>["NotThere.csv","labelxyz"]
CSourceFileExceptions<CsvFileLoadedViaTemplateT>::ExCannotOpenFile

#<14:>TABLE_LOAD<:14>["TestInputFile_RequireDelimitingChar.csv","labelxyz"]
CCsvParser::ExRequireDelimitingChar

#<15:>TABLE_LOAD<:15>["TestInputFile_UnexpectedQuote.csv","labelxyz"]
CCsvParser::ExUnexpectedQuote

#<16:>TABLE_LOAD<:16>["TestInputFile_RowOverflow.csv","labelxyz"]
CVerticalTableBuilderExceptions::ExOverflow

#<17:>TABLE_LOAD<:17>["TestInputFile_RowUnderflow.csv","labelxyz"]
CVerticalTableBuilderExceptions::ExUnderflow

#<18:>TABLE_LOAD<:18>["","labelxyz"]
CTemplateProvidedTableLoaderExceptions::ExTableLoadFileNameMustNotBeEmpty

#   <19:>PART_PADDING<:19>
CParserExceptions::ExUnexpectedPartPadding
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TableBlockErrors.tpl.txt", fileData);
    }
}
