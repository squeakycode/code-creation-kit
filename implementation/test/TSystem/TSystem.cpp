//  Copyright (c) 2011-2015 Andreas Gau
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
#include <string>
#include "System.h"

using namespace code_creation_kit;

template <typename StringT>
void run_test()
{
   typedef typename StringT::value_type CharT;

#ifdef _MSC_VER //TODO
   _putenv("TEST_VARIABLE=123abc");
#else
    setenv("TEST_VARIABLE","123abc",1);
#endif

    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE)")) == STRING_LITERAL("123abc"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start$(TEST_VARIABLE)end")) == STRING_LITERAL("start123abcend"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("start1$(TEST_VARIABLE)end1start2$(TEST_VARIABLE)end2")) == STRING_LITERAL("start1123abcend1start2123abcend2"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE222222222222)")) == STRING_LITERAL("$(TEST_VARIABLE222222222222)"));
    BOOST_CHECK( System::expandEnvironmentVariables<StringT>( STRING_LITERAL("$(TEST_VARIABLE )")) == STRING_LITERAL("$(TEST_VARIABLE )"));
}

BOOST_AUTO_TEST_CASE( TSystem)
{
    run_test<std::string>();
#ifdef _MSC_VER //TODO
    run_test<std::wstring>(); //true wchar support is not implemented
#endif
}

