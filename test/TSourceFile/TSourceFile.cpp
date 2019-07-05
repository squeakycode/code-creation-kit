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
#include "CSourceFile.h"
#include "TSourceFileTestFiles.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

using namespace code_creation_kit;

///represents streams in test
template <typename StringT>
class TStreamHelper
{
public:
    template <typename InputT>
    TStreamHelper<StringT>& operator <<(const InputT& text)
    {
        result.push_back( boost::lexical_cast<StringT>(text));
        return *this;
    }
    std::vector<StringT> result; 
};

class TestFile;

template <typename StringT>
void run_test()
{
    BOOST_CHECK_NO_THROW(CreateTSourceFileFiles());

    typedef TStreamHelper<StringT> SinkT;
    typedef CSourceFile<StringT,TestFile> FileT;

    {
        SinkT result;
        SinkT expected;
        int counter = 0;
        FileT( boost::lexical_cast<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "a.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 0);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "b.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 1);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>(CCK_TEST_INPUT_FILE_PREFIX  "c.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 1);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n" << "b";

        int counter = 0;
        FileT( boost::lexical_cast<StringT>(CCK_TEST_INPUT_FILE_PREFIX "d.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 2);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a" << "b";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>(CCK_TEST_INPUT_FILE_PREFIX "d.txt")).feedLineSink( result, false, counter);
        BOOST_CHECK( counter == 2);
        BOOST_CHECK( result.result == expected.result);
    }
}

BOOST_AUTO_TEST_CASE( TSourceFile)
{
    run_test<std::string>();
#ifdef _MSC_VER //TODO
    run_test<std::wstring>();
#endif
}

