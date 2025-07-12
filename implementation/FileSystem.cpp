// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include "FileSystem.h"
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif

namespace code_creation_kit
{
    namespace FileSystem
    {
        bool recycleFile( const std::string& location)
        {
#ifdef WIN32
            std::vector<std::string::value_type> temp( location.size() + 2, 0);
            memcpy( &temp[0], location.c_str(), location.size());
            SHFILEOPSTRUCTA fileOps;
            ::ZeroMemory(&fileOps, sizeof(fileOps));
            fileOps.wFunc = FO_DELETE;
            fileOps.pFrom = &temp[0];
            fileOps.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;

            HRESULT res = SHFileOperationA( &fileOps);
            return SUCCEEDED( res);
#else
            (void) location;
            return false;
#endif
        }

        bool recycleFile( const std::wstring& location)
        {
#ifdef WIN32
            std::vector<std::wstring::value_type> temp( location.size() + 2, 0);
            memcpy( &temp[0], location.c_str(), location.size());
            SHFILEOPSTRUCTW fileOps;
            ::ZeroMemory(&fileOps, sizeof(fileOps));
            fileOps.wFunc = FO_DELETE;
            fileOps.pFrom = &temp[0];
            fileOps.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;

            HRESULT res = SHFileOperationW( &fileOps);
            return SUCCEEDED( res);
#else
            (void) location;
            return false;
#endif
        }
    }
}
