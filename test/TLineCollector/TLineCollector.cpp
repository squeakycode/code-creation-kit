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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "CLineCollector.h"

using namespace code_creation_kit;

///represents output stream in test
template <typename StringT>
class THelper
{
public:
    THelper<StringT>& operator <<(const StringT& text)
    {
        result.push_back(text);
        return *this;
    }
    std::vector<StringT> result; 
};

BOOST_AUTO_TEST_CASE( TLineCollector)
{
    //types in test
    typedef std::string StringT;
    typedef THelper<StringT> OutputStreamT;
    typedef CLineCollector<StringT,OutputStreamT> CollectorT;

    //create objects
    OutputStreamT lines;
    OutputStreamT expectedLines;
    CollectorT collector;

    //connect object
    collector.connectOutputStream( &lines);

    //feed text snippets
    collector
        << StringT("ab")
        << StringT("c\ndef")
        << StringT("\ngh")
        << StringT("ijk\nlmn")
        ;

    collector.close();

    //fill expected output
    expectedLines
        << "abc\n" 
        << "def\n"
        << "ghijk\n"
        << "lmn"
        ;

    //compare equal
    BOOST_CHECK( expectedLines.result == lines.result);
}
