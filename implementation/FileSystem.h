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

#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "StringLiteral.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
#endif
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#include <iostream>

namespace code_creation_kit
{
    namespace FileSystem
    {

        template <typename StringT>
        inline StringT getString(const boost::filesystem::path&)
        {
        }

        template <>
        inline std::string getString(const boost::filesystem::path& p)
        {
            std::string s = p.string();
            boost::replace_all(s, "\\", "/");
            return s;
        }

        template <>
        inline std::wstring getString(const boost::filesystem::path& p)
        {
            std::wstring s = p.wstring();
            boost::replace_all(s, "\\", "/");
            return s;
        }

        template <typename StringT>
        bool equals( const StringT& a, const StringT& b)
        {
#if defined WIN32
            return boost::iequals( a, b);
#else
            return boost::equals( a, b);
#endif
        }

        ///returns the location (path+name) determined from a fixed location and a location that may be relative to the fixed location
        template <typename StringT>
        inline StringT determineRelativeLocation( const StringT& location, const StringT& dependentLocation, bool locationIsFile = true)
        {
            typedef typename StringT::value_type CharT;
            typedef boost::filesystem::path PathT;

            //create path objects
            PathT base( location);
            PathT dependent( dependentLocation);

            base.normalize();
            dependent.normalize();

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
            typedef boost::filesystem::path PathT;
            PathT base( location);
            PathT relative( relativeLocation);

            //if is absolute path return it
            if ( relative.has_root_name())
            {
                return getString<StringT>( relative);
            }
            else
            {
                //create new location relative to base
                PathT  dependentLocation( (locationIsFile ? base.parent_path() : base) / relativeLocation);
                //normalize it removing superfluous '..' and so on
                dependentLocation.normalize();
                return getString<StringT>( dependentLocation);
            }
        }

        ///returns the filename
        template <typename StringT>
        inline StringT determineFilename( const StringT& location)
        {
            typedef boost::filesystem::path PathT;
            PathT path( location);
            return getString<StringT>(path.filename());
        }

        ///returns the location (path+name) determined from a initial path and a location that may be relative to it
        template <typename StringT>
        inline StringT determineDependentLocation(const StringT& relativeLocation)
        {
            typedef boost::filesystem::path PathT;
            PathT initialPath( boost::filesystem::initial_path<PathT>());
            return determineDependentLocation(getString<StringT>(initialPath), relativeLocation, false);
        }

        /// returns true if the loaction specifies an existing file
        template <typename StringT>
        inline bool isRegularFile( const StringT& location)
        {
            typedef boost::filesystem::path PathT;
            PathT path( location);
            return boost::filesystem::is_regular_file( path);
        }

        /// removes a file
        template <typename StringT>
        inline bool removeFile( const StringT& location)
        {
            typedef boost::filesystem::path PathT;
            PathT path( location);
            return boost::filesystem::remove( path);
        }

        /// moves a file
        template <typename StringT>
        inline void moveFile( const StringT& from, const StringT& to)
        {
            typedef boost::filesystem::path PathT;
            PathT pathFrom( from);
            PathT pathTo( to);
            boost::filesystem::rename( from, to);
        }

        /// recycle a file
        bool recycleFile( const std::string& location);
        bool recycleFile( const std::wstring& location);

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
