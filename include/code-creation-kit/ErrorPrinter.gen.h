// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <set>
#include "SimpleFormat.h"
#include "FileSystem.h"
#include "StringLiteral.h"

#if defined CODE_CREATION_KIT_JUST_TESTING
//prevent test output to be listed as error
#   define CODE_CREATION_KIT_ERROR_TAG1 "just_testing"
#   define CODE_CREATION_KIT_ERROR_TAG2 "Just_Testing"
#else
#   define CODE_CREATION_KIT_ERROR_TAG1 "error"
#   define CODE_CREATION_KIT_ERROR_TAG2 "Error"
#endif

namespace code_creation_kit
{
    class CErrorPrinted{};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

    template <typename StringT, typename GeneratorT>
    class ErrorPrinter
    {
    public:
        typedef SimpleFormat<StringT> FormatT;
        typedef typename StringT::value_type CharT; 

        ErrorPrinter( GeneratorT& generator) : m_generator( generator) {}

        template <typename ParameterListT>
        void generate(
            const StringT& templateFileName,
            const StringT& targetFileName,
            bool useIntermediateFile,
            bool recycle,
            const StringT& intermediateFileName,
            bool append,
            const ParameterListT& parameters,
            bool canChangeTableList = false,
            const InlineTemplateParameters<StringT>& inlineTemplateParameters = InlineTemplateParameters<StringT>()
        )
        {
            try
            {
                m_generator.generate( 
                    templateFileName,
                    targetFileName,
                    useIntermediateFile,
                    recycle,
                    intermediateFileName,
                    append,
                    parameters,
                    canChangeTableList,
                    inlineTemplateParameters);
            }
            catch( SourceFileExceptions<TemplateFileT>::ExCannotOpenFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1060: Cannot open template file: %1%\n"));
                formatter % addPath( getCurrentFileName());
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( SourceFileExceptions<TemplateFileT>::ExCannotReadFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1061: An error occured reading template file: %1%\n"));
                formatter % addPath( getCurrentFileName());
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TargetFileExceptions<GeneratedFileT>::ExCannotOpenFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1070: Cannot open target file: %1%\n"));
                formatter % targetFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TargetFileExceptions<GeneratedFileT>::ExCannotWriteToFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1071: An error occured writing to target file: %1%\n"));
                formatter % targetFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TargetFileExceptions<IntermediateFileT>::ExCannotOpenFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1080: Cannot open intermediate file: %1%\n"));
                formatter % intermediateFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TargetFileExceptions<IntermediateFileT>::ExCannotWriteToFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1081: An error occured writing to intermediate file: %1%\n"));
                formatter % intermediateFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( FilesBinaryEqualExceptions::ExStreamBad& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1090: Failed to compare intermediate file '%1%' with target file '%2%'\n"));
                formatter % intermediateFileName % targetFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( GeneratorExceptions::ExFailedToDeleteTempFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1100: Failed to delete intermediate output file: %1%\n"));
                formatter % intermediateFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( GeneratorExceptions::ExFailedToDeleteOldTargetFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1101: Failed to delete old version of target file '%1%' for replacement with intermediate file.\n"));
                formatter % targetFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( GeneratorExceptions::ExCannotMoveIntermediateFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1102: Failed to move intermediate file '%1%' to target file '%2%'\n"));
                formatter % intermediateFileName % targetFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( KeywordParameterParser::ExParameterStartExpected& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1206: Parameter is missing or syntax is incorrect,  '[' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( KeywordParameterParser::ExParameterSeparatorExpected& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1207: Less parameters then expected or syntax is incorrect, ',' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( KeywordParameterParser::ExParameterEndExpected& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1208: More parameters then expected or syntax is incorrect, ']' is expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( KeywordParameterParser::ExParameterValueExpected& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1209: Parameter value is missing or syntax is incorrect. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( KeywordParameterParser::ExCharsUsedForPaddingNotSupported& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1215: The text passed for padding contains unsupported characters. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ProcessingLevelControlExceptions::ExPossibleInfiniteLoop& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1250: Recursion exceeded the maximum of %3% levels while expanding macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getMaxNumberOfRecursionLevels();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ProcessingLevelControlExceptions::ExCannotSetRecursionLevelLimit& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1251: The set recursion level limit directive can only be used at the beginning of a line and outside of a macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplateLoaderExceptions::ExCyclicInclusion& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1300: Cyclic inclusion detected.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParameterParser::ExMissingDelimiterInParameterDescription& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1400: Syntax bad for parameter option number %1%, expecting name=value but is: %2%\n"));
                formatter % (m_generator.getIndexOfLastProcessedParameter() + 1) % parameters.at( m_generator.getIndexOfLastProcessedParameter());
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( RegexMatchesConstraintExceptions::ExRegexSyntaxError& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( RegexReplaceConversionExceptions::ExRegexSyntaxError& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1500: Syntax error in regular expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CalcConversionExceptions::ExDivisionByZero& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1510: Division by zero. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CalcConversionExceptions::ExArithmeticExpressionSyntaxError& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1511: Syntax error in arithmetic expression. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplatePreprocessorExceptions::ExPrefixLeadingWhiteSpace& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1602: Leading white space for markup prefix is not allowed.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplatePreprocessorExceptions::ExPostfixTrailingWhiteSpace& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1603: Trailing white space for markup postfix is not allowed.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplatePreprocessorExceptions::ExBadlyPlacedTrim& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1604: Trim directives are expected at the end of a line. Trailing whitespace is allowed.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplatePreprocessorExceptions::ExBadlyPlacedComment& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1605: COMMENT is expected at the beginning of a line. Leading whitespace is allowed.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingBlockBegin& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1801: Missing begin block marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingMacroBegin& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1802: Missing begin macro marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingBlockEnd& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1803: Missing block end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingMacroEnd& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1804: Missing macro end marker. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExUnexpectedKeyword& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1810: Syntax error. Unexpected keyword found. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExUnexpectedEndOfMacro& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1805: Unexpected end of macro. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExDirectiveAlreadyApplied& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1820: Directive has been applied already. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExConstraintExpectedAfterNot& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1821: Constraint following not expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExSubstitutionExpectedAfterIf& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1822: Substitution following if expected. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExCannotApplyDirectiveToSubstitution& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1823: Directive cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExCannotApplyDirectiveToConstraint& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1824: Directive cannot be applied to this constraint. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExCannotApplyDirectiveToConversion& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1825: Directive cannot be applied to this conversion. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExCannotApplyConstraintToSubstitution& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1826: Constraint cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExCannotApplyConversionToSubstitution& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1827: Conversion cannot be applied to this substitution. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExSubstitutionParsingBadOrder& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1828: Extensions fo a substitution are expected in the order directives, constraints, conversions. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExSubstitutionRequiresIf& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1829: Substitution requires if. The line number shown corresponds to the last read line. This may not be the line causing the error for multi line macros.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMacroTooLarge& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1830: The macro exceeds the maximum allowed size of %3% kb. This error can be caused by a too large text line or by a missing macro end marker. The text compiler is not designed to handle very large text lines efficiently.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % (m_generator.getMaxMacroTextSizeBytes() / 1024);
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExPartBlocksCannotBeNested& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1840: Part blocks cannot be nested.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingPartBegin& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1841: Missing begin part marker.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingPartEnd& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1842: Missing part end marker.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExPartAlreadyDefined& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1843: Part already defined. Cannot add a second part with the same label.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExPartNotDefined& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1844: Part not defined. Cannot expand part.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExPossibleInfiniteLoop& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1845: Recursion exceeded the maximum of allowed levels while expanding part.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplateProvidedTableLoaderExceptions::ExUnexpectedTableProperty& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1860: Unexpected table property found for template-provided table.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingTableBegin& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1861: Missing begin table marker.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExMissingTableEnd& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1862: Missing table end marker.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroProcessorExceptions::ExCannotChangeTableList& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1863: Changing the table list permanently is disabled. Only temporary tables can be added or removed.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( TemplateProvidedTableLoaderExceptions::ExTableLoadFileNameMustNotBeEmpty& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1864: Table filename must not be empty.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ParserExceptions::ExUnexpectedPartPadding& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1865: The part padding directives can only be used inside a part block.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( ToSizeConversionExceptions::ExUnexpectedToSizeProperty& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1880: Unexpected property found for to size conversion.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExBadDelimiter& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1001: Provided character cannot be used as delimiting character. The line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExRequireDelimitingChar& e)
            {
                (void) e; //unused
                if (!getTableLoadFileNameWithFailure().empty())
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1002: New line or delimiter at the end of item in quotes is expected.\n"));
                    formatter % addPath( getTableLoadFileNameWithFailure()) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
                    toErrorStream( formatter.str());
                }
                else
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1002: New line or delimiter at the end of item in quotes is expected. CSV table source line %4% column %3%. The source file line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                    formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getLastColumnWithFailure() % m_generator.getLastLineWithFailure();
                    toErrorStream( formatter.str());
                }
                throw CErrorPrinted();
            }
            catch( CsvParser::ExUnexpectedQuote& e)
            {
                (void) e; //unused
                if (!getTableLoadFileNameWithFailure().empty())
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1003: Unexpected quote.\n"));
                    formatter % addPath( getTableLoadFileNameWithFailure()) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
                    toErrorStream( formatter.str());
                }
                else
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1003: Unexpected quote. CSV table source line %4%  column %3%. The source file line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                    formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getLastColumnWithFailure() % m_generator.getLastLineWithFailure();
                    toErrorStream( formatter.str());
                }
                throw CErrorPrinted();
            }
            catch( CsvParser::ExStreamBad& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1004: An error occured reading table.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( SourceFileExceptions<CsvFileLoadedViaTemplateT>::ExCannotOpenFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1010: Cannot open table file: %1%\n"));
                formatter % addPath(getTableLoadFileNameWithFailure());
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( SourceFileExceptions<CsvFileLoadedViaTemplateT>::ExCannotReadFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1011: An error occured reading table file: %1%\n"));
                formatter % addPath(getTableLoadFileNameWithFailure());
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( VerticalTableBuilderExceptions::ExUnderflow& e)
            {
                (void) e; //unused
                if (!getTableLoadFileNameWithFailure().empty())
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1020: Table row %4% contains less items than first row.\n"));
                    formatter % addPath( getTableLoadFileNameWithFailure()) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
                    toErrorStream( formatter.str());
                }
                else
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1020: Table row %4% contains less items than first row. CSV table source line %5% column %3%. The source file line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                    formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure() % m_generator.getLastLineWithFailure();
                    toErrorStream( formatter.str());
                }
                throw CErrorPrinted();
            }
            catch( VerticalTableBuilderExceptions::ExOverflow& e)
            {
                (void) e; //unused
                if (!getTableLoadFileNameWithFailure().empty())
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1021: Table row %4% contains more items than first row.\n"));
                    formatter % addPath( getTableLoadFileNameWithFailure()) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
                    toErrorStream( formatter.str());
                }
                else
                {
                    FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1021: Table row %4% contains more items than first row. CSV table source line %5% column %3%. The source file line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                    formatter % addPath( getCurrentFileName()) % getCurrentLineNumber() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure() % m_generator.getLastLineWithFailure();
                    toErrorStream( formatter.str());
                }
                throw CErrorPrinted();
            }
            catch( CsvParser::ExBadCommentChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1030: The CSV comment char list contains characters that cannot be used for commenting lines, e.g. quote or the delimiter characters. The line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExBadQuoteChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1031: The CSV quote char list contains characters that cannot be used for putting text items in quotes, e.g. comment or the delimiter characters. The line number shown corresponds to the last read line. This may not be the line causing the error.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroProcessorExceptions::ExTableLabelAlreadyDefined& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1053: Table label already defined. Cannot add a second table with the same label.\n"));
                formatter % addPath( getCurrentFileName()) % getCurrentLineNumber();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroExpanderExceptions::ExErrorTagExpanded<StringT>& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1900: Error tag triggered: %3%\n"));
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
        void setCsvDelimiterChars(const StringT& csvDelimiterChars)
        {
            try
            {
                m_generator.setCsvDelimiterChars(csvDelimiterChars);
            }
            catch( CsvParser::ExBadDelimiter& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1001: '%1%' cannot be used as delimiting characters.\n"));
                formatter % csvDelimiterChars;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
        }

        ///set list of characters as string that mark commented lines for next csv table to load
        void setCsvCommentChars( const StringT& csvCommentChars)
        {
            try
            {
                m_generator.setCsvCommentChars( csvCommentChars);
            }
            catch( CsvParser::ExBadCommentChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines, e.g. quote or the delimiter characters.\n"));
                formatter % csvCommentChars;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
        }

        void setCsvQuoteChars(const StringT& csvQuoteChars)
        {
            try
            {
                m_generator.setCsvQuoteChars(csvQuoteChars);
            }
                 catch( CsvParser::ExBadQuoteChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1031: The CSV quote char list '%1%' contains characters that cannot be used for putting text items in quotes, e.g. comment or the delimiter characters.\n"));
                formatter % csvQuoteChars;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
        }

        void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex, bool padRows)
        {
            try
            {
                m_generator.loadTable( tableFileName, label, topDown, leftToRight, rowHeaderIndex, columnHeaderIndex, padRows);
            }
            catch( CsvParser::ExBadDelimiter& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1001: '%1%' cannot be used as delimiting characters.\n"));
                formatter % m_generator.getCsvDelimiterChars();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExRequireDelimitingChar& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1002: New line or delimiter at the end of item in quotes is expected.\n"));
                formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExUnexpectedQuote& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1003: Unexpected quote.\n"));
                formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExStreamBad& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1004: An error occured reading table file: %1%\n"));
                formatter % tableFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( SourceFileExceptions<CsvFileT>::ExCannotOpenFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1010: Cannot open table file: %1%\n"));
                formatter % tableFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( SourceFileExceptions<CsvFileT>::ExCannotReadFile& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1011: An error occured reading table file: %1%\n"));
                formatter % tableFileName;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( VerticalTableBuilderExceptions::ExUnderflow& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1020: Table row %4% contains less items than first row.\n"));
                formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( VerticalTableBuilderExceptions::ExOverflow& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL("%1%(%2%,%3%) : " CODE_CREATION_KIT_ERROR_TAG1 " TC1021: Table row %4% contains more items than first row.\n"));
                formatter % addPath( tableFileName) % m_generator.getLastLineWithFailure() % m_generator.getLastColumnWithFailure() % m_generator.getLastRowNumberWithFailure();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExBadCommentChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1030: The CSV comment char list '%1%' contains characters that cannot be used for commenting lines, e.g. quote or the delimiter characters.\n"));
                formatter % m_generator.getCsvCommentChars();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( CsvParser::ExBadQuoteChars& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1031: The CSV quote char list '%1%' contains characters that cannot be used for putting text items in quotes, e.g. comment or the delimiter characters.\n"));
                formatter % m_generator.getCsvQuoteChars();
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroProcessorExceptions::ExRowHeaderIndexOutOfBounds& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1051: Row header index %1% exceeds the bounds of the table.\n"));
                formatter % rowHeaderIndex;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroProcessorExceptions::ExColumnHeaderIndexOutOfBounds& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1052: Column header index  %1% exceeds the bounds of the table.\n"));
                formatter % columnHeaderIndex;
                toErrorStream( formatter.str());
                throw CErrorPrinted();
            }
            catch( MacroProcessorExceptions::ExTableLabelAlreadyDefined& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1053: Table label '%1%' already defined. Cannot add a second table with the same label.\n"));
                formatter % label;
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
            catch( MacroProcessorExceptions::ExTableNotFound& e)
            {
                (void) e; //unused
                FormatT formatter(STRING_LITERAL(" " CODE_CREATION_KIT_ERROR_TAG2 " TC1701: A table entry with the provided label '%1%' does not exist.\n"));
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

        ///connect log output stream
        template <typename LogOutputStreamT>
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_generator.connectLogOutputStream( stream);
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
            const typename GeneratorT::FileDataT& fileData = m_generator.getLastTemplateFileProcessed();
            if (fileData.line == 0) //uninitialized?
            {
                return 1;
            }
            return fileData.line;
        }

        StringT getCurrentFileName()
        {
            const typename GeneratorT::FileDataT& fileData = m_generator.getLastTemplateFileProcessed();
            if ( fileData.name.empty()) //uninitialized?
            {
                return STRING_LITERAL("???");
            }

            return fileData.name;
        }

        StringT getTableLoadFileNameWithFailure()
        {
            return m_generator.getTableLoadFileNameWithFailure();
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

#undef CODE_CREATION_KIT_ERROR_TAG1
#undef CODE_CREATION_KIT_ERROR_TAG2
}
