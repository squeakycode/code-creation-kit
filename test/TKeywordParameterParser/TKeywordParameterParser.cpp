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
#include "CombiKeywordParameterParser.gen.h"
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

using namespace code_creation_kit;

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

    { // getParametersCombiCStyleUIntUIntRepeat
        std::vector<std::string> expectedParameters;
        std::vector<std::string> parsedParameters;
        expectedParameters.push_back(" ");
        expectedParameters.push_back("99");
        {
            std::string text("[\" \" , 99]");
            std::string::iterator it = text.begin();
            KeywordParameterParser::getParametersCombiCStyleUIntUIntRepeat(it, text.end(), parsedParameters);
            BOOST_CHECK(it == text.end());
            BOOST_CHECK(expectedParameters == parsedParameters);
        }
        expectedParameters.push_back("22");
        {
            std::string text("[ \" \" , 99, 22 ]");
            std::string::iterator it = text.begin();
            KeywordParameterParser::getParametersCombiCStyleUIntUIntRepeat(it, text.end(), parsedParameters);
            BOOST_CHECK(it == text.end());
            BOOST_CHECK(expectedParameters == parsedParameters);
        }
        expectedParameters.push_back("+42");
        {
            std::string text("[\" \",99,22,+42]");
            std::string::iterator it = text.begin();
            KeywordParameterParser::getParametersCombiCStyleUIntUIntRepeat(it, text.end(), parsedParameters);
            BOOST_CHECK(it == text.end());
            BOOST_CHECK(expectedParameters == parsedParameters);
        }
    }

}
