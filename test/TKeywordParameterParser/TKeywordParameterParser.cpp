// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "CombiKeywordParameterParser.gen.h"
#include <string>
#include <sstream>
#include <vector>
#include "StringConvert.h"

using namespace code_creation_kit;

//perform the actual test
template <typename StringT, typename PolicyT>
void test( const char* testText, std::vector<std::string> expectedParameters, unsigned int count)
{
    //prepare
    StringT text = StringConvert<StringT>( testText);
    typename StringT::const_iterator start = text.begin();
    typename StringT::const_iterator end = text.end();
    //perform parsing
    std::vector<StringT> parameters;
    parameters.resize(count);
    KeywordParameterParser::getParameters<PolicyT>( start, end, parameters);
    //check
    CHECK( *start == static_cast<typename StringT::value_type>('-'));
    REQUIRE( expectedParameters.size() == parameters.size());
    for( unsigned int i = 0; i < expectedParameters.size(); ++i)
    {
        bool ok = parameters[i] == StringConvert<StringT>( expectedParameters[i].c_str());
        if ( !ok )
        {
            CHECK( ok );
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

TEST_CASE( "TKeywordParameterParser", "[TKeywordParameterParser]")
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
        CHECK_THROWS_AS( test<CPlainParameterPolicy>( "x", expectedParameters, 1), KeywordParameterParser::ExParameterStartExpected);
        CHECK_THROWS_AS( test<CPlainParameterPolicy>( "[]-", expectedParameters, 1), KeywordParameterParser::ExParameterValueExpected);
        CHECK_THROWS_AS( test<CPlainParameterPolicy>( "[\"test\"]-", expectedParameters, 2), KeywordParameterParser::ExParameterSeparatorExpected);
        CHECK_THROWS_AS( test<CPlainParameterPolicy>( "[\"test\",\"test\"]-", expectedParameters, 1), KeywordParameterParser::ExParameterEndExpected);
    }

    { // one + special chars
        std::vector<std::string> expectedParameters;
        expectedParameters.push_back( "\n\t\"\\test");
        test<CCStyleParameterPolicy>( "[\"\\n\\t\\\"\\\\test\"]-", expectedParameters, 1);
    }


    { //exceptions
        std::vector<std::string> expectedParameters;
        CHECK_THROWS_AS( test<CCStyleParameterPolicy>( "[\"a\\s\"]-", expectedParameters, 1), KeywordParameterParser::ExParameterValueExpected);
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
            KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional(it, text.end(), parsedParameters);
            CHECK(it == text.end());
            CHECK(expectedParameters == parsedParameters);
        }
        expectedParameters.push_back("22");
        {
            std::string text("[ \" \" , 99, 22 ]");
            std::string::iterator it = text.begin();
            KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional(it, text.end(), parsedParameters);
            CHECK(it == text.end());
            CHECK(expectedParameters == parsedParameters);
        }
        expectedParameters.push_back("+42");
        {
            std::string text("[\" \",99,22,+42]");
            std::string::iterator it = text.begin();
            KeywordParameterParser::getParametersCombi1CStyle1UIntRepeatUIntOptional(it, text.end(), parsedParameters);
            CHECK(it == text.end());
            CHECK(expectedParameters == parsedParameters);
        }
    }

}
