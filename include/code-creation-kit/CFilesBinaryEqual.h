// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <string>
#include <fstream>

namespace code_creation_kit
{
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
        if ( (!fileA && !fileA.eof()) || (!fileB && !fileB.eof()))
        {
            throw FilesBinaryEqualExceptions::ExStreamBad();
        }

        //equal when both streams are at end of file and the content is the same
        bool equal = a == b && fileA.eof() && fileB.eof(); 
        return equal;
    }
}
