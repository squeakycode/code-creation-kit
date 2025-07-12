// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include <fstream>

#include <sys/stat.h>
#ifdef _MSC_VER
#include <direct.h>
#endif

#define CCK_TEST_INPUT_FILE_PREFIX "TestInputFile_"
#define CCK_TEST_INPUT_FILE_PREFIX_WCHAR L"TestInputFile_"

inline void WriteTestInputFile(const char* fileName, const char* fileData)
{
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(fileName);
    file << fileData;
    file.close();
}


inline bool CreateDirForTesting(const char* dirName)
{
    bool result = false;
    struct stat st;
    int resultStat = stat(dirName, &st);
    if (resultStat != -1 && (st.st_mode & S_IFDIR))
    {
        result = true;
    }
#ifdef _MSC_VER
    else if (mkdir(dirName) != -1)
#else
    else if (mkdir(dirName, S_IRWXU | S_IRWXG) != -1)
#endif
    {
        result = true;
    }
    return result;
}