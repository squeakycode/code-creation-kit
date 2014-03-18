//   Copyright (C) 2011-2014 Andreas Gau
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

#include "FileSystem.h"
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif

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