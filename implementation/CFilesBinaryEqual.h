//  Copyright (c) 2011-2023 Andreas Gau
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
