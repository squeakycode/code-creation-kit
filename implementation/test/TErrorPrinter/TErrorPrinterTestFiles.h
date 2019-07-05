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


}
