//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_CFILESBINARYEQUAL_H_7098963
#define INCLUDED_CFILESBINARYEQUAL_H_7098963

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <string>
#include <fstream>

///holds exceptions thrown by comparison functions
namespace FilesBinaryEqualExceptions
{
    class ExStreamBad : public std::runtime_error 
    { public: ExStreamBad() : std::runtime_error( "Failed to compare files. Cannot read from input stream.") {}};
}

///checks two files for binary equality
template <typename StringT>
bool FilesBinaryEqual( const StringT& fileNameA, const StringT& fileNameB)
{
    typedef typename StringT::value_type CharT;
    typedef std::basic_ifstream< CharT, std::char_traits<CharT> > InputStreamT;

    //open file a
    InputStreamT fileA( fileNameA.c_str(), std::ios::binary);
    if ( !fileA.is_open() )
    {
        return false; //not equal if either one does not exist
    }

    //open file b
    InputStreamT fileB( fileNameB.c_str(), std::ios::binary);
    if ( !fileB.is_open() )
    {
        return false; //not equal if either one does not exist
    }

    //read from both streams and compare
    CharT a, b;
    do
    {
        fileA.get(a);
        fileB.get(b);
    }
    while ( a == b && fileA && fileB);

    //check that the streams have not failed reading
    if ( !fileA && !fileA.eof() || !fileB && !fileB.eof())
    {
        throw FilesBinaryEqualExceptions::ExStreamBad();
    }

    //equal when both streams are at end of file and the content is the same
    bool equal = a == b && fileA.eof() && fileB.eof(); 
    return equal;
}

#endif /* INCLUDED_CFILESBINARYEQUAL_H_7098963 */
