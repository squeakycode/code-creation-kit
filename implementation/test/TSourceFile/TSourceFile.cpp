//   Copyright (C) 2011 Andreas Gau
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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "CSourceFile.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

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
    typedef TStreamHelper<StringT> SinkT;
    typedef CSourceFile<StringT,TestFile> FileT;

    {
        SinkT result;
        SinkT expected;
        int counter = 0;
        FileT( boost::lexical_cast<StringT>("a.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 0);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>("b.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 1);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>("c.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 1);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a\n" << "b";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>("d.txt")).feedLineSink( result, true, counter);
        BOOST_CHECK( counter == 2);
        BOOST_CHECK( result.result == expected.result);
    }
    {
        SinkT result;
        SinkT expected;
        expected << "a" << "b";
        int counter = 0;
        FileT( boost::lexical_cast<StringT>("d.txt")).feedLineSink( result, false, counter);
        BOOST_CHECK( counter == 2);
        BOOST_CHECK( result.result == expected.result);
    }
}

BOOST_AUTO_TEST_CASE( TSourceFile)
{
    run_test<std::string>();
    run_test<std::wstring>();
}

