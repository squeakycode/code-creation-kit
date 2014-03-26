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

#define BOOST_TEST_MAIN
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include "CLineCollector.h"

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