#  Copyright (c) 2011-2023 Andreas Gau
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of the copyright holder nor the
#        names of contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

add_library(cck_implementation STATIC
        ../include/code-creation-kit/CBackEndTokenizer.gen.h
        ../codegen/datamodels/CCommandFileLineParser.csv
    CCommandFileLineParser.gen.h
        ../codegen/CCommandFileLineParser.tccmd
        ../codegen/datamodels/CCommandFileLineParser.xls
        ../codegen/datamodels/CCommandLineParser.csv
    CCommandLineParser.gen.h
        ../codegen/CCommandLineParser.tccmd
        ../codegen/datamodels/CCommandLineParser.xls
        ../include/code-creation-kit/CComparableSharedObject.h
    CErrorPrinter.gen.h
        ../codegen/CErrorPrinter.tccmd
        ../codegen/templates/CErrorPrinter.tpl.h
    CFilesBinaryEqual.h
    CGenerator.h
    CGeneratorCommandProcessor.h
    CGeneratorStatistic.h
    CInlineTemplateParameters.h
        ../include/code-creation-kit/CLineCollector.h
        ../include/code-creation-kit/CMacro.h
        ../include/code-creation-kit/CMacroExpander.h
        ../include/code-creation-kit/CMacroExpression.h
        ../include/code-creation-kit/CMacroProcessor.h
    CMakeLists.txt
        ../include/code-creation-kit/CNul.h
        ../include/code-creation-kit/CombiKeywordParameterParser.gen.h
        ../codegen/CombiKeywordParameterParser.tccmd
        ../codegen/templates/CombiKeywordParameterParser.tpl.h
    CommandProcessor.h
        ../include/code-creation-kit/ConstraintDirectives.gen.h
        ../codegen/templates/ConstraintDirectives.tpl.h
        ../include/code-creation-kit/Constraints.h
        ../include/code-creation-kit/ConversionDirectives.gen.h
        ../codegen/templates/ConversionDirectives.tpl.h
        ../include/code-creation-kit/Conversions.h
        ../include/code-creation-kit/CParser.h
        ../include/code-creation-kit/CPositionTracker.h
        ../include/code-creation-kit/cppstringx.hpp
        ../include/code-creation-kit/cpptokenfinder.hpp
        ../include/code-creation-kit/CProcessingLevelControl.h
    CSimpleFormat.h
        ../include/code-creation-kit/CSourceFile.h
        ../include/code-creation-kit/CSubstitution.h
        ../include/code-creation-kit/CTableIndex.h
        ../include/code-creation-kit/CTargetFile.h
        ../include/code-creation-kit/CTemplateLoader.h
        ../include/code-creation-kit/CTemplatePreprocessor.h
        ../include/code-creation-kit/CTemplateProcessor.h
        ../include/code-creation-kit/CToken.h
        ../include/code-creation-kit/CTokenizer.gen.h
        ../codegen/templates/CTokenizer.tpl.h
        ../include/code-creation-kit/CVerticalTableBuilder.h
        ../include/code-creation-kit/CVerticalTableRotator.h
    DependencyPrinter.h
        ../include/code-creation-kit/ESubstitutions.gen.h
        ../codegen/templates/ESubstitutions.tpl.h
        ../include/code-creation-kit/ETokens.gen.h
        ../codegen/templates/ETokens.tpl.h
        ../codegen/datamodels/Exceptions.csv
        ../codegen/datamodels/Exceptions.xls
    FileSystem.cpp
        ../include/code-creation-kit/FileSystem.h
        ../include/code-creation-kit/IConstraint.h
        ../include/code-creation-kit/IConversion.h
        ../include/code-creation-kit/KeywordParameterCheckFunctions.h
        ../include/code-creation-kit/KeywordParameterParser.h
    ParameterParser.h
        ../codegen/Parser.tccmd
        ../codegen/datamodels/ParserExtensions.csv
        ../include/code-creation-kit/ParserExtensions.gen.h
        ../codegen/templates/ParserExtensions.tpl.h
        ../codegen/datamodels/ParserExtensions.xls
        ../include/code-creation-kit/StringConvert.h
        ../include/code-creation-kit/StringLiteral.h
        ../codegen/Substitutions.tccmd
    System.h
        ../include/code-creation-kit/CTemplateProvidedTableLoader.h
)

target_include_directories(cck_implementation
INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
${PROJECT_SOURCE_DIR}/include/code-creation-kit
PRIVATE
${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(cck_implementation)

if (CCK_TCCMD_EXECUTABLE_NEVER)
    message(STATUS "Adding cck_implementation tccmd execution.")
    
    #CCommandFileLineParser.tccmd >> CCommandFileLineParser.gen.h << CCommandFileLineParser.csv ../templates/CommandLineParser.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CCommandFileLineParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/CCommandFileLineParser.tccmd"
        MAIN_DEPENDENCY ../codegen/CCommandFileLineParser.tccmd
        DEPENDS ../codegen/datamodels/CCommandFileLineParser.csv ../codegen/templates/CommandLineParser.tpl.h
        COMMENT "Generating CCommandFileLineParser.tccmd"
    )

    #CCommandLineParser.tccmd >> CCommandLineParser.gen.h << CCommandLineParser.csv ../templates/CommandLineParser.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CCommandLineParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/CCommandLineParser.tccmd"
        MAIN_DEPENDENCY ../codegen/CCommandLineParser.tccmd
        DEPENDS ../codegen/datamodels/CCommandLineParser.csv ../codegen/templates/CommandLineParser.tpl.h
        COMMENT "Generating CCommandLineParser.tccmd"
    )

    #CErrorPrinter.tccmd >> CErrorPrinter.gen.h << Exceptions.csv CErrorPrinter.tpl.h
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CErrorPrinter.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/CErrorPrinter.tccmd"
        MAIN_DEPENDENCY ../codegen/CErrorPrinter.tccmd
        DEPENDS ../codegen/datamodels/Exceptions.csv ../codegen/templates/CErrorPrinter.tpl.h
        COMMENT "Generating CErrorPrinter.tccmd"
    )

    #Parser.tccmd >> ConstraintDirectives.gen.h ConversionDirectives.gen.h ParserExtensions.gen.h << "../requirements/Tag Specification.csv" ParserExtensions.csv ConstraintDirectives.tpl.h ConversionDirectives.tpl.h ParserExtensions.tpl.h 
    add_custom_command(
        OUTPUT "../include/code-creation-kit/ConstraintDirectives.gen.h" "../include/code-creation-kit/ConversionDirectives.gen.h" "../include/code-creation-kit/ParserExtensions.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/Parser.tccmd"
        MAIN_DEPENDENCY ../codegen/Parser.tccmd
        DEPENDS "../requirements/Tag Specification.csv" ../codegen/datamodels/ParserExtensions.csv ../codegen/templates/ConstraintDirectives.tpl.h ../codegen/templates/ConversionDirectives.tpl.h ../codegen/templates/ParserExtensions.tpl.h
        COMMENT "Generating Parser.tccmd"
    )

    #Substitutions.tccmd >> ESubstitutions.gen.h << "../requirements/Tag Specification.csv" ESubstitutions.tpl.h 
    add_custom_command(
        OUTPUT "../include/code-creation-kit/ESubstitutions.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/Substitutions.tccmd"
        MAIN_DEPENDENCY ../codegen/Substitutions.tccmd
        DEPENDS "../requirements/Tag Specification.csv" ../codegen/templates/ESubstitutions.tpl.h
        COMMENT "Generating Substitutions.tccmd"
    )

    #Tokens.tccmd >> CBackEndTokenizer.gen.h CTokenizer.gen.h ETokens.gen.h test/TTokenizer/TokenizerTest.gen.h << "../requirements/Tag Specification.csv" CTokenizer.tpl.h ETokens.tpl.h test/TTokenizer/TokenizerTest.tpl.h 
    add_custom_command(
        OUTPUT "../include/code-creation-kit/CBackEndTokenizer.gen.h" "../include/code-creation-kit/CTokenizer.gen.h" "../include/code-creation-kit/ETokens.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/../test/TTokenizer/TokenizerTest.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/Tokens.tccmd"
        MAIN_DEPENDENCY ../codegen/Tokens.tccmd
        DEPENDS "../requirements/Tag Specification.csv" ../codegen/templates/CTokenizer.tpl.h ../codegen/templates/ETokens.tpl.h ../test/TTokenizer/TokenizerTest.tpl.h
        COMMENT "Generating Tokens.tccmd"
    )

    #CombiKeywordParameterParser.tccmd >> CombiKeywordParameterParser.gen.h << "../requirements/Tag Specification.CombiParsers.csv" CombiKeywordParameterParser.tpl.h
    add_custom_command(
        OUTPUT "../include/code-creation-kit/CombiKeywordParameterParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "../codegen/CombiKeywordParameterParser.tccmd"
        MAIN_DEPENDENCY ../codegen/CombiKeywordParameterParser.tccmd
        DEPENDS "../requirements/Tag Specification.CombiParsers.csv" ../codegen/templates/CombiKeywordParameterParser.tpl.h
        COMMENT "Generating CombiKeywordParameterParser.tccmd"
    )

endif()

