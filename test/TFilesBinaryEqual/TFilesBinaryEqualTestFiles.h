// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include "../include/TestInputFileWriter.h"

inline void CreateTFilesBinaryEqualFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TFilesBinaryEqual/a.txt
    {
        const char* fileData =
R"(1
2
3
4)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "a.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TFilesBinaryEqual/a_.txt
    {
        const char* fileData =
R"(1
2
3
4)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "a_.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TFilesBinaryEqual/b.txt
    {
        const char* fileData =
R"(1
2
3
4
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "b.txt", fileData);
    }


}
