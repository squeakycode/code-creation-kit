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

#define BOOST_TEST_MAIN
#ifndef _MSC_VER
#   define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>
#include "KeywordParameterParser.h"
#include <string>
#include <sstream>
#include <vector>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

//perform the actual test
template <typename StringT, typename PolicyT>
void test( const char* testText, std::vector<std::string> expectedParameters, unsigned int count)
{
    //prepare
    StringT text = boost::lexical_cast<StringT>( testText);
    typename StringT::const_iterator start = text.begin();
    typename StringT::const_iterator end = text.end();
    //perform parsing
    std::vector<StringT> parameters;
    parameters.resize(count);
    KeywordParameterParser::getParameters<PolicyT>( start, end, parameters);
    //check
    BOOST_CHECK( *start == boost::lexical_cast<typename StringT::value_type>('-'));
    BOOST_REQUIRE( expectedParameters.size() == parameters.size());
    for( unsigned int i = 0; i < expectedParameters.size(); ++i)
    {
        bool ok = parameters[i] == boost::lexical_cast<StringT>( expectedParameters[i].c_str());
        if ( !ok )
        {
            BOOST_CHECK( ok );
        }
    }
}

//test wide and narrow
template <typename PolicyT>
void test( const char* testText, std::vector<std::string> expectedParameters, unsigned int count)
{
    test<std::string, PolicyT>( testText, expectedParameters, count);
    test<std::wstring, PolicyT>( testText, expectedParameters, count);
}

BOOST_AUTO_TEST_CASE( TKeywordParameterParser)
{
    { // one
        std::vector<std::string> expectedParameters;
        expectedParameters.push_back( "test");
        test<CPlainParameterPolicy>( "[ \"test\" ]-", expectedParameters, 1);
    }

    { // two
        std::vector<std::string> expectedParameters;
        expectedParameters.push_back( "testa");
        expectedParameters.push_back( "testb");
        test<CPlainParameterPolicy>( "[\"testa\" , \"testb\"]-", expectedParameters, 2);
    }

    { //exceptions
        std::vector<std::string> expectedParameters;
        BOOST_CHECK_THROW( test<CPlainParameterPolicy>( "x", expectedParameters, 1), KeywordParameterParser::ExParameterStartExpected);
        BOOST_CHECK_THROW( test<CPlainParameterPolicy>( "[]-", expectedParameters, 1), KeywordParameterParser::ExParameterValueExpected);
        BOOST_CHECK_THROW( test<CPlainParameterPolicy>( "[\"test\"]-", expectedParameters, 2), KeywordParameterParser::ExParameterSeparatorExpected);
        BOOST_CHECK_THROW( test<CPlainParameterPolicy>( "[\"test\",\"test\"]-", expectedParameters, 1), KeywordParameterParser::ExParameterEndExpected);
    }

    { // one + special chars
        std::vector<std::string> expectedParameters;
        expectedParameters.push_back( "\n\t\"\\test");
        test<CCStyleParameterPolicy>( "[\"\\n\\t\\\"\\\\test\"]-", expectedParameters, 1);
    }


    { //exceptions
        std::vector<std::string> expectedParameters;
        BOOST_CHECK_THROW( test<CCStyleParameterPolicy>( "[\"a\\s\"]-", expectedParameters, 1), KeywordParameterParser::ExParameterValueExpected);
    }

    { // one + special chars
        std::vector<std::string> expectedParameters;
        expectedParameters.push_back( "a'b");
        test<CRegexParameterPolicy>( "['a''b']-", expectedParameters, 1);
    }
}
