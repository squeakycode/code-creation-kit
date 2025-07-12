// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include "../include/TestInputFileWriter.h"

inline void CreateTCommandProcessorFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TCommandProcessor/CommandFile.tccmd
    {
        const char* fileData =
R"(-l table.csv

# -s y.txt -o z.txt -u -e .intermediatefile
-s a.txt -o b.txt -u -e .temp -p "a=\"b\""

-r)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "CommandFile.tccmd", fileData);
    }

    ///////////////////////////////////////////////////////////////////////////
    //TCommandProcessor/myfile.h.itpl
    {
        const char* fileData =
            R"()";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "myfile.h.itpl", fileData);
    }

    ///////////////////////////////////////////////////////////////////////////
    //TCommandProcessor/myfile
    {
        const char* fileData =
            R"()";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "myfile", fileData);
    }
}
