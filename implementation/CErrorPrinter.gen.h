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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <boost/format.hpp>

#include "FileSystem.h"
#include <set>
#include "StringLiteral.h"

class CErrorPrinted{};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

template <typename StringT, typename GeneratorT>
class CErrorPrinter
{
public:
    typedef boost::basic_format< typename StringT::value_type> FormatT;
    typedef typename StringT::value_type CharT; 

    CErrorPrinter( GeneratorT& generator) : m_generator( generator) {}

    template <typename ParameterListT>
    void generate( const StringT& templateFileName, const StringT& targetFileName, bool useIntermediateFile, const StringT& intermediateFileName, bool append, const ParameterListT& parameters)
    {
        try
        {
            m_generator.generate( templateFileName, targetFileName, useIntermediateFile, intermediateFileName, append, parameters);
        }
        catch( CSourceFileExceptions<TemplateFileT>::ExCannotOpenFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1060: Cannot open template file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1060: Cannot open template file: %1%\n"));
#endif
            formatter % addPath( getCurrentFileName());
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CSourceFileExceptions<TemplateFileT>::ExCannotReadFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1061: An error occured reading template file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1061: An error occured reading template file: %1%\n"));
#endif
            formatter % addPath( getCurrentFileName());
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTargetFileExceptions<GeneratedFileT>::ExCannotOpenFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1070: Cannot open target file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1070: Cannot open target file: %1%\n"));
#endif
            formatter % targetFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTargetFileExceptions<GeneratedFileT>::ExCannotWriteToFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1071: An error occured writing to target file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1071: An error occured writing to target file: %1%\n"));
#endif
            formatter % targetFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTargetFileExceptions<IntermediateFileT>::ExCannotOpenFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1080: Cannot open intermediate file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1080: Cannot open intermediate file: %1%\n"));
#endif
            formatter % intermediateFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTargetFileExceptions<IntermediateFileT>::ExCannotWriteToFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1081: An error occured writing to intermediate file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1081: An error occured writing to intermediate file: %1%\n"));
#endif
            formatter % intermediateFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( FilesBinaryEqualExceptions::ExStreamBad& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1090: Failed to compare intermediate file '%1%' with target file '%2%'\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1090: Failed to compare intermediate file '%1%' with target file '%2%'\n"));
#endif
            formatter % intermediateFileName % targetFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CGeneratorExceptions::ExFailedToDeleteTempFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1100: Failed to delete intermediate output file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1100: Failed to delete intermediate output file: %1%\n"));
#endif
            formatter % intermediateFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CGeneratorExceptions::ExFailedToDeleteOldTargetFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1101: Failed to delete old version of target file '%1%' for replacement with intermediate file.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1101: Failed to delete old version of target file '%1%' for replacement with intermediate file.\n"));
#endif
            formatter % targetFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CGeneratorExceptions::ExCannotMoveIntermediateFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1102: Failed to move intermediate file '%1%' to target file '%2%'\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1102: Failed to move intermediate file '%1%' to target file '%2%'\n"));
#endif
            formatter % intermediateFileName % targetFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( KeywordParameterParser::ExParameterStartExpected& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1206: Parameter is missing or syntax is incorrect,  '[' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1206: Parameter is missing or syntax is incorrect,  '[' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( KeywordParameterParser::ExParameterSeparatorExpected& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1207: Less parameters then expected or syntax is incorrect, ',' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1207: Less parameters then expected or syntax is incorrect, ',' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( KeywordParameterParser::ExParameterEndExpected& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1208: More parameters then expected or syntax is incorrect, ']' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1208: More parameters then expected or syntax is incorrect, ']' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( KeywordParameterParser::ExParameterValueExpected& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1209: Parameter value is missing or syntax is incorrect. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1209: Parameter value is missing or syntax is incorrect. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CProcessingLevelControlExceptions::ExPossibleInfiniteLoop& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1250: Recursion exceeded the maximum of %3% levels while expanding macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1250: Recursion exceeded the maximum of %3% levels while expanding macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getMaxNumberOfRecursionLevels();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1251: The set recursion level limit directive can only be used at the beginning of a line and outside of a macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1251: The set recursion level limit directive can only be used at the beginning of a line and outside of a macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTemplateLoaderExceptions::ExCyclicInclusion& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1300: Cyclic inclusion detected.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1300: Cyclic inclusion detected.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( ParameterParser::ExMissingDelimiterInParameterDescription& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1400: Syntax bad for parameter option number %1%, expecting name=value but is: %2%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1400: Syntax bad for parameter option number %1%, expecting name=value but is: %2%\n"));
#endif
            formatter % (m_generator.getIndexOfLastProcessedParameter() + 1) % parameters.at( m_generator.getIndexOfLastProcessedParameter());
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CRegexMatchesConstraintExceptions::ExRegexSyntaxError& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CRegexReplaceConversionExceptions::ExRegexSyntaxError& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTemplatePreprocessorExceptions::ExPrefixLeadingWhiteSpace& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1602: Leading white space for markup prefix is not allowed.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1602: Leading white space for markup prefix is not allowed.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTemplatePreprocessorExceptions::ExPostfixTrailingWhiteSpace& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1603: Trailing white space for markup postfix is not allowed.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1603: Trailing white space for markup postfix is not allowed.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTemplatePreprocessorExceptions::ExBadlyPlacedTrim& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1604: Trim directives are expected at the end of a line. Trailing whitespace is allowed.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1604: Trim directives are expected at the end of a line. Trailing whitespace is allowed.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CTemplatePreprocessorExceptions::ExBadlyPlacedComment& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1605: COMMENT is expected at the beginning of a line. Leading whitespace is allowed.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1605: COMMENT is expected at the beginning of a line. Leading whitespace is allowed.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExMissingBlockBegin& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1801: Missing begin block marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1801: Missing begin block marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExMissingMacroBegin& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1802: Missing begin macro marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1802: Missing begin macro marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExMissingBlockEnd& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1803: Missing block end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1803: Missing block end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExMissingMacroEnd& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1804: Missing macro end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1804: Missing macro end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExUnexpectedKeyword& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1810: Syntax error. Unexpected keyword found. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1810: Syntax error. Unexpected keyword found. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExUnexpectedEndOfMacro& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1805: Unexpected end of macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1805: Unexpected end of macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExDirectiveAlreadyApplied& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1820: Directive has been applied already. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1820: Directive has been applied already. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExConstraintExpectedAfterNot& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1821: Constraint following not expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1821: Constraint following not expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExSubstitutionExpectedAfterIf& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1822: Substitution following if expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1822: Substitution following if expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExCannotApplyDirectiveToSubstitution& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1823: Directive cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1823: Directive cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExCannotApplyDirectiveToConstraint& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1824: Directive cannot be applied to this constraint. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1824: Directive cannot be applied to this constraint. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExCannotApplyDirectiveToConversion& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1825: Directive cannot be applied to this conversion. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1825: Directive cannot be applied to this conversion. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExCannotApplyConstraintToSubstitution& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1826: Constraint cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1826: Constraint cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExCannotApplyConversionToSubstitution& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1827: Conversion cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1827: Conversion cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExSubstitutionParsingBadOrder& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1828: Extensions fo a substitution are expected in the order directives, constraints, conversions. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1828: Extensions fo a substitution are expected in the order directives, constraints, conversions. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExSubstitutionRequiresIf& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1829: Substitution requires if. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1829: Substitution requires if. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CParserExceptions::ExMacroTooLarge& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1830: The macro exceeds the maximum allowed size of %3% kb. This error can be caused by a too large text line or by a missing macro end marker. The text compiler is not designed to handle very large text lines efficiently.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1830: The macro exceeds the maximum allowed size of %3% kb. This error can be caused by a too large text line or by a missing macro end marker. The text compiler is not designed to handle very large text lines efficiently.\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % (m_generator.getMaxMacroTextSizeBytes() / 1024);
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CMacroExpanderExceptions::ExErrorTagExpanded<StringT>& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%) : just_testing TC1900: Error tag triggered: %3%\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%) : error TC1900: Error tag triggered: %3%\n"));
#endif
            formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % e.getMessage();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
    }

    void reset()
    {
        m_generator.reset();
    }

    ///set delimiter for next csv table to load
    void setCsvDelimiter( CharT delimiter)
    {
        try
        {
            m_generator.setCsvDelimiter( delimiter);
        }
        catch( CCsvParser::ExBadDelimiter& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1001: '%1%' cannot be used as delimiting character.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1001: '%1%' cannot be used as delimiting character.\n"));
#endif
            formatter % delimiter;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
    }

    ///set list of characters as string that mark commented lines for next csv table to load
    void setCsvCommentChars( const StringT& commentChars)
    {
        try
        {
            m_generator.setCsvCommentChars( commentChars);
        }
        catch( CCsvParser::ExBadCommentChars& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines. e.g. double quote and the delimiter.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines. e.g. double quote and the delimiter.\n"));
#endif
            formatter % commentChars;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
    }

    void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
    {
        try
        {
            m_generator.loadTable( tableFileName, label, topDown, leftToRight, rowHeaderIndex, columnHeaderIndex);
        }
        catch( CCsvParser::ExBadDelimiter& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1001: '%1%' cannot be used as delimiting character.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1001: '%1%' cannot be used as delimiting character.\n"));
#endif
            formatter % m_generator.getCsvDelimiter();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CCsvParser::ExRequireDelimitingChar& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : just_testing TC1002: New line or delimiter at the end of item in quotes is expected.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : error TC1002: New line or delimiter at the end of item in quotes is expected.\n"));
#endif
            formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CCsvParser::ExUnexpectedQuote& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : just_testing TC1003: Unexpected quote.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : error TC1003: Unexpected quote.\n"));
#endif
            formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CCsvParser::ExStreamBad& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1004: An error occured reading table file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1004: An error occured reading table file: %1%\n"));
#endif
            formatter % tableFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CSourceFileExceptions<CsvFileT>::ExCannotOpenFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1010: Cannot open table file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1010: Cannot open table file: %1%\n"));
#endif
            formatter % tableFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CSourceFileExceptions<CsvFileT>::ExCannotReadFile& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1011: An error occured reading table file: %1%\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1011: An error occured reading table file: %1%\n"));
#endif
            formatter % tableFileName;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CVerticalTableBuilderExceptions::ExUnderflow& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : just_testing TC1020: Table row %4% contains less items than first row.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : error TC1020: Table row %4% contains less items than first row.\n"));
#endif
            formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CVerticalTableBuilderExceptions::ExOverflow& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : just_testing TC1021: Table row %4% contains more items than first row.\n"));
#else
            FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : error TC1021: Table row %4% contains more items than first row.\n"));
#endif
            formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CCsvParser::ExBadCommentChars& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines. e.g. double quote and the delimiter.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines. e.g. double quote and the delimiter.\n"));
#endif
            formatter % m_generator.getCsvCommentChars();
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CMacroProcessorExceptions::ExRowHeaderIndexOutOfBounds& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1051: Row header index %1% exceeds the bounds of the table.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1051: Row header index %1% exceeds the bounds of the table.\n"));
#endif
            formatter % rowHeaderIndex;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CMacroProcessorExceptions::ExColumnHeaderIndexOutOfBounds& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1052: Column header index  %1% exceeds the bounds of the table.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1052: Column header index  %1% exceeds the bounds of the table.\n"));
#endif
            formatter % columnHeaderIndex;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
    }

    void unloadTable( const StringT& label)
    {
        try
        {
            m_generator.unloadTable( label);
        }
        catch( CGeneratorExceptions::ExFailedToUnloadTable& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1700: A table entry with the specified label '%1%' does not exist.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1700: A table entry with the specified label '%1%' does not exist.\n"));
#endif
            formatter % label;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
        catch( CMacroProcessorExceptions::ExTableNotFound& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL(" Just_Testing TC1701: A table entry with the specified label '%1%' does not exist.\n"));
#else
            FormatT formatter(STRING_LITERAL(" Error TC1701: A table entry with the specified label '%1%' does not exist.\n"));
#endif
            formatter % label;
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
    }

    ///sets new tag markup
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        m_generator.setMarkup( prefix, postfix);
    }

    ///adds an include directory to the list
    void addIncludeDirectory( const StringT& directory)
    {
        m_generator.addIncludeDirectory( directory);
    }

    typedef std::set<StringT> FileSetT;

    ///get list of loaded tables, statistic only
    const FileSetT& getTableFiles() const
    {
        return m_generator.getTableFiles();
    }

    ///get list of generated files, statistic only
    const FileSetT& getGeneratedFiles() const
    {
        return m_generator.getGeneratedFiles();
    }

    ///get list of template files, statistic only
    const FileSetT& getTemplateFiles() const
    {
        return m_generator.getTemplateFiles();
    }

private:
    void toErrorStream( std::string text)
    {
        std::cerr << text;
    }

    void toErrorStream( std::wstring text)
    {
        std::wcerr << text;
    }

    unsigned int getCurrentLineNumber()
    {
        const typename GeneratorT::FileDataListT& list = m_generator.getInclusionHierarchy();
        if ( list.empty())
        {
            return 1;
        }
        return list.back().line;
    }

    StringT getCurrentFileName()
    {
        const typename GeneratorT::FileDataListT& list = m_generator.getInclusionHierarchy();
        if ( list.empty())
        {
            return STRING_LITERAL("???");
        }
        if ( list.back().usingCin)
        {
            return STRING_LITERAL("stdin");
        }
        return list.back().name;
    }

    StringT addPath( const StringT& location)
    {
        return FileSystem::determineDependentLocation( location);
    }

    GeneratorT& m_generator;
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
