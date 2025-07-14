// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include "../include/TestInputFileWriter.h"

inline void CreateTGeneratorStatisticFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TGeneratorStatistic/test2.tpl.txt
    {
        const char* fileData =
R"()";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "test2.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGeneratorStatistic/IncludeDirectory/test1.tpl.txt
    {
        const char* fileData =
R"(a
b
c
[SET_MARKUP]["$", "?"]
d
e
$INCLUDE?["../TestInputFile_test2.tpl.txt"][TRIM]
f
)";

        WriteTestInputFile("IncludeDirectory/" CCK_TEST_INPUT_FILE_PREFIX "test1.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGeneratorStatistic/IncludeDirectory/test1Inline.tpl.txt
    {
        const char* fileData =
R"($$$[COMMENT] This is a comment.
$$$b
$$$c
$$$[SET_MARKUP]["$", "?"]
$$$d
$$$e
$$$$INCLUDE?["../TestInputFile_test2.tpl.txt"][TRIM]
$$$f
)";

        WriteTestInputFile("IncludeDirectory/" CCK_TEST_INPUT_FILE_PREFIX "test1Inline.tpl.txt", fileData);
    }


}
