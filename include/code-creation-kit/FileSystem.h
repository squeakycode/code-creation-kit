// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifdef WIN32
#include <windows.h>
#include <Shellapi.h>
#endif

#include <iostream>
#include <filesystem>

#include "cppstringx.hpp"
#include "StringLiteral.h"

namespace filesystem_namespace = std;

namespace code_creation_kit
{
    namespace FileSystem
    {

        template <typename StringT>
        inline StringT getString(const filesystem_namespace::filesystem::path&)
        {
        }

        template <>
        inline std::string getString(const filesystem_namespace::filesystem::path& p)
        {
            std::string s = p.string();
            std::string result = cppstringx::replace_all_copy(s, "\\", "/");
            return result;
        }

        template <>
        inline std::wstring getString(const filesystem_namespace::filesystem::path& p)
        {
            std::wstring s = p.wstring();
            std::wstring result = cppstringx::replace_all_copy(s, "\\", "/");
            return result;
        }

        template <typename StringT>
        bool equals( const StringT& a, const StringT& b)
        {
#if defined WIN32
            return cppstringx::iequals( a, b);
#else
            return cppstringx::equals( a, b);
#endif
        }

        //restores deprecated boost behavior without treating symlinks correctly, TODO
        inline void unsafe_normalize(filesystem_namespace::filesystem::path& p)
        {
            filesystem_namespace::filesystem::path result;
            for (filesystem_namespace::filesystem::path::iterator it = p.begin(); it != p.end();
                ++it)
            {
                if (*it == "..")
                {
                    if (result.filename() == ".." || result.empty())
                    {
                        result /= *it;
                    }
                    else
                    {
                        result = result.parent_path();
                    }
                }
                else if (*it == ".")
                {
                    // ignore
                }
                else
                {
                    result /= *it;
                }
            }
            p = result;
        }

        ///returns the location (path+name) determined from a fixed location and a location that may be relative to the fixed location
        template <typename StringT>
        inline StringT determineRelativeLocation( const StringT& location, const StringT& dependentLocation, bool locationIsFile = true)
        {
            typedef typename StringT::value_type CharT;
            typedef filesystem_namespace::filesystem::path PathT;

            //create path objects
            PathT base( location);
            PathT dependent( dependentLocation);

            unsafe_normalize(base);
            unsafe_normalize(dependent);

            //remove filename if file
            if ( locationIsFile )
            {
                base = base.parent_path();
            }

            if (   base.has_root_path()
                && dependent.has_root_path()
                && equals(getString<StringT>(base.root_path()), getString<StringT>(dependent.root_path()))
                )
            {
                //relative location to determine
                PathT relativeLocation;
                //iterate over paths
                typename PathT::const_iterator baseIt = base.begin();
                typename PathT::const_iterator dependentIt = dependent.begin();
                //skip root path
                ++baseIt;
                ++dependentIt;
                while( baseIt != base.end() && dependentIt != dependent.end())
                {
                    //if root folders are different
                    if (!equals(getString<StringT>( *baseIt), getString<StringT>( *dependentIt)))
                    {
                        //add '..' for each base location folder to move to root
                        while ( baseIt != base.end())
                        {
                            ++baseIt;
                            relativeLocation /= STRING_LITERAL("..");
                        }
                    }
                    else
                    {
                        ++baseIt;
                        ++dependentIt;
                    }

                    //base folder is processed
                    if ( baseIt == base.end() && dependentIt != dependent.end() )
                    {
                        //move from base to target
                        for (; dependentIt != dependent.end(); ++dependentIt)
                        {
                            relativeLocation /= *dependentIt;
                        }
                        return getString<StringT>( relativeLocation);
                    }

                    //dependent is processed
                    if ( baseIt != base.end() && dependentIt == dependent.end() )
                    {
                        //add '..' for each base location folder to move to root
                        while ( baseIt != base.end())
                        {
                            ++baseIt;
                            relativeLocation /= STRING_LITERAL("..");
                        }
                        return getString<StringT>( relativeLocation);
                    }
                }
            }
            return getString<StringT>( dependent);
        }

        ///returns the location (path+name) determined from a fixed location and a location that may be relative to the fixed location
        template <typename StringT>
        inline StringT determineDependentLocation( const StringT& location, const StringT& relativeLocation, bool locationIsFile = true)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT base( location);
            PathT relative( relativeLocation);

            //if is absolute path return it
            if (relative.is_absolute())
            {
                //normalize it removing superfluous '..' and so on
                unsafe_normalize(relative);
                return getString<StringT>( relative);
            }
            else
            {
                //create new location relative to base
                PathT  dependentLocation( (locationIsFile ? base.parent_path() : base) / relativeLocation);
                //normalize it removing superfluous '..' and so on
                unsafe_normalize(dependentLocation);
                return getString<StringT>( dependentLocation);
            }
        }

        ///returns the filename
        template <typename StringT>
        inline StringT determineFilename( const StringT& location)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT path( location);
            return getString<StringT>(path.filename());
        }

        ///removes an extension if present
        template <typename StringT>
        inline StringT removeExtension(const StringT& location)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT path(location);
            path.replace_extension();
            return getString<StringT>(path);
        }

        ///returns the location (path+name) determined from a initial path and a location that may be relative to it
        template <typename StringT>
        inline StringT determineDependentLocation(const StringT& relativeLocation)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT initialPath;
            return determineDependentLocation(getString<StringT>(initialPath), relativeLocation, false);
        }

        /// returns true if the loaction specifies an existing file
        template <typename StringT>
        inline bool isRegularFile( const StringT& location)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT path( location);
            return filesystem_namespace::filesystem::is_regular_file( path);
        }

        /// removes a file
        template <typename StringT>
        inline bool removeFile( const StringT& location)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT path( location);
            return filesystem_namespace::filesystem::remove( path);
        }

        /// moves a file
        template <typename StringT>
        inline void moveFile( const StringT& from, const StringT& to)
        {
            typedef filesystem_namespace::filesystem::path PathT;
            PathT pathFrom( from);
            PathT pathTo( to);
            filesystem_namespace::filesystem::rename( from, to);
        }

        /// recycle a file
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

        /// recycle a file
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

        ///helper function for getting the right output stream
        template <typename CharT> 
        inline std::basic_ostream<CharT, std::char_traits<CharT> >& getCout()
        {
            return std::cout;
        }

        ///helper function for getting the right output stream
        template <>
        inline std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& getCout()
        {
            return std::wcout;
        }

        ///helper function for getting the right output stream
        template <typename CharT> 
        inline std::basic_ostream<CharT, std::char_traits<CharT> >& getCerr()
        {
            return std::cerr;
        }

        ///helper function for getting the right output stream
        template <>
        inline std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& getCerr()
        {
            return std::wcerr;
        }
    }
}
