// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

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
TemplateProvidedTableLoaderExceptions::ExUnexpectedTableProperty

#<3:>TABLE_BEGIN<:3>["labelxyz",";",";"];Numbers;1;2;5<3:>TABLE_END<:3><<3:>ENTRY<:3>["Numbers"]>
CsvParser::ExBadCommentChars

#<4:>TABLE_BEGIN<:4>["labelxyz","\n",";"];Numbers;1;2;5<4:>TABLE_END<:4><<4:>ENTRY<:4>["Numbers"]>
CsvParser::ExBadDelimiter

#<5:>TABLE_BEGIN<:5>["labelxyz"];Numbers;1;2;5
;Numbers;1;2;5<5:>
;Numbers;1;2;5<5:>
;Numbers;1;2";5<5:>
<5:>TABLE_END<:5><<5:>ENTRY<:5>["Numbers"]>
CsvParser::ExUnexpectedQuote

#<6:>TABLE_BEGIN<:6>["labelxyz"];Numbers;1;2a;5
;Numbers;1;2a;5
;Numbers;1;"2"a;5
<6:>TABLE_END<:6><<6:>ENTRY<:6>["Numbers"]>
CsvParser::ExRequireDelimitingChar

a<8:>TABLE_BEGIN<:8>["l"]b
ParserExceptions::ExMissingTableEnd

a<10:>TABLE_END<:10>b
ParserExceptions::ExMissingTableBegin

#<11:>TABLE_BEGIN<:11>["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2
row3;3,1;3,2;3,3
<11:>TABLE_END<:11><<11:>ENTRY<:11>["m"]>
VerticalTableBuilderExceptions::ExUnderflow

#<12:>TABLE_BEGIN<:12>["labelxyz"]m;col1;col2;col3
row1;1,1;1,2;1,3
row2;2,1;2,2;over;flow
row3;3,1;3,2;3,3
<12:>TABLE_END<:12><<12:>ENTRY<:12>["m"]>
VerticalTableBuilderExceptions::ExOverflow

#<13:>TABLE_LOAD<:13>["NotThere.csv","labelxyz"]
SourceFileExceptions<CsvFileLoadedViaTemplateT>::ExCannotOpenFile

#<14:>TABLE_LOAD<:14>["TestInputFile_RequireDelimitingChar.csv","labelxyz"]
CsvParser::ExRequireDelimitingChar

#<15:>TABLE_LOAD<:15>["TestInputFile_UnexpectedQuote.csv","labelxyz"]
CsvParser::ExUnexpectedQuote

#<16:>TABLE_LOAD<:16>["TestInputFile_RowOverflow.csv","labelxyz"]
VerticalTableBuilderExceptions::ExOverflow

#<17:>TABLE_LOAD<:17>["TestInputFile_RowUnderflow.csv","labelxyz"]
VerticalTableBuilderExceptions::ExUnderflow

#<18:>TABLE_LOAD<:18>["","labelxyz"]
TemplateProvidedTableLoaderExceptions::ExTableLoadFileNameMustNotBeEmpty

#   <19:>PART_PADDING<:19>
ParserExceptions::ExUnexpectedPartPadding
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TableBlockErrors.tpl.txt", fileData);
    }
}
