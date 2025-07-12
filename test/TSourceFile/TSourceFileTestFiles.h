// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include "../include/TestInputFileWriter.h"

inline void CreateTSourceFileFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TSourceFile/a.txt
    {
        const char* fileData =
R"()";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "a.txt", fileData);
    }


    ///////////////////////////////////////////////////////////////////////////
    //TSourceFile/b.txt
    {
        const char* fileData =
R"(a)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "b.txt", fileData);
    }


    ///////////////////////////////////////////////////////////////////////////
    //TSourceFile/c.txt
    {
        const char* fileData =
R"(a
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "c.txt", fileData);
    }


    ///////////////////////////////////////////////////////////////////////////
    //TSourceFile/d.txt
    {
        const char* fileData =
R"(a
b)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "d.txt", fileData);
    }
}
