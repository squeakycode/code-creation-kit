#  Copyright (c) 2011-2019 Andreas Gau
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
    CBackEndTokenizer.gen.h
    CCommandFileLineParser.csv
    CCommandFileLineParser.gen.h
    CCommandFileLineParser.tccmd
    CCommandFileLineParser.xls
    CCommandLineParser.csv
    CCommandLineParser.gen.h
    CCommandLineParser.tccmd
    CCommandLineParser.xls
    CComparableSharedObject.h
    CCsvParser.h
    CErrorPrinter.gen.h
    CErrorPrinter.tccmd
    CErrorPrinter.tpl.h
    CFilesBinaryEqual.h
    CGenerator.h
    CGeneratorCommandProcessor.h
    CGeneratorStatistic.h
    CInlineTemplateParameters.h
    CLineCollector.h
    CMacro.h
    CMacroExpander.h
    CMacroExpression.h
    CMacroProcessor.h
    CMakeLists.txt
    CNul.h
    CombiKeywordParameterParser.gen.h
    CombiKeywordParameterParser.tccmd
    CombiKeywordParameterParser.tpl.h
    CommandProcessor.h
    ConstraintDirectives.gen.h
    ConstraintDirectives.tpl.h
    Constraints.h
    ConversionDirectives.gen.h
    ConversionDirectives.tpl.h
    Conversions.h
    CParser.h
    CPositionTracker.h
    CProcessingLevelControl.h
    CSourceFile.h
    CSpecialRegexCharacterPrefixer.h
    CSubstitution.h
    CTableIndex.h
    CTargetFile.h
    CTemplateLoader.h
    CTemplatePreprocessor.h
    CTemplateProcessor.h
    CToken.h
    CTokenizer.gen.h
    CTokenizer.tpl.h
    CVerticalTableBuilder.h
    CVerticalTableRotator.h
    DependencyPrinter.h
    ESubstitutions.gen.h
    ESubstitutions.tpl.h
    ETokens.gen.h
    ETokens.tpl.h
    Exceptions.csv
    Exceptions.xls
    FileSystem.cpp
    FileSystem.h
    IConstraint.h
    IConversion.h
    KeywordParameterCheckFunctions.h
    KeywordParameterParser.h
    ParameterParser.h
    Parser.tccmd
    ParserExtensions.csv
    ParserExtensions.gen.h
    ParserExtensions.tpl.h
    ParserExtensions.xls
    StringLiteral.h
    Substitutions.tccmd
    System.h
    CTemplateProvidedTableLoader.h
)

target_include_directories(cck_implementation
PRIVATE
${Boost_INCLUDE_DIR}
)

custom_target_use_highest_warning_level(cck_implementation)

if (CCK_TCCMD_EXECUTABLE)
    message(STATUS "Adding cck_implementation tccmd execution.")
    
    #CCommandFileLineParser.tccmd >> CCommandFileLineParser.gen.h << CCommandFileLineParser.csv ../templates/CommandLineParser.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CCommandFileLineParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/CCommandFileLineParser.tccmd"
        MAIN_DEPENDENCY CCommandFileLineParser.tccmd
        DEPENDS CCommandFileLineParser.csv ../templates/CommandLineParser.tpl.h
        COMMENT "Generating CCommandFileLineParser.tccmd"
    )

    #CCommandLineParser.tccmd >> CCommandLineParser.gen.h << CCommandLineParser.csv ../templates/CommandLineParser.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CCommandLineParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/CCommandLineParser.tccmd"
        MAIN_DEPENDENCY CCommandLineParser.tccmd
        DEPENDS CCommandLineParser.csv ../templates/CommandLineParser.tpl.h
        COMMENT "Generating CCommandLineParser.tccmd"
    )

    #CErrorPrinter.tccmd >> CErrorPrinter.gen.h << Exceptions.csv CErrorPrinter.tpl.h
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CErrorPrinter.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/CErrorPrinter.tccmd"
        MAIN_DEPENDENCY CErrorPrinter.tccmd
        DEPENDS Exceptions.csv CErrorPrinter.tpl.h
        COMMENT "Generating CErrorPrinter.tccmd"
    )

    #Parser.tccmd >> ConstraintDirectives.gen.h ConversionDirectives.gen.h ParserExtensions.gen.h << "../requirements/Tag Specification.csv" ParserExtensions.csv ConstraintDirectives.tpl.h ConversionDirectives.tpl.h ParserExtensions.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/ConstraintDirectives.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/ConversionDirectives.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/ParserExtensions.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Parser.tccmd"
        MAIN_DEPENDENCY Parser.tccmd
        DEPENDS "../requirements/Tag Specification.csv" ParserExtensions.csv ConstraintDirectives.tpl.h ConversionDirectives.tpl.h ParserExtensions.tpl.h 
        COMMENT "Generating Parser.tccmd"
    )

    #Substitutions.tccmd >> ESubstitutions.gen.h << "../requirements/Tag Specification.csv" ESubstitutions.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/ESubstitutions.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Substitutions.tccmd"
        MAIN_DEPENDENCY Substitutions.tccmd
        DEPENDS "../requirements/Tag Specification.csv" ESubstitutions.tpl.h 
        COMMENT "Generating Substitutions.tccmd"
    )

    #Tokens.tccmd >> CBackEndTokenizer.gen.h CTokenizer.gen.h ETokens.gen.h test/TTokenizer/TokenizerTest.gen.h << "../requirements/Tag Specification.csv" CTokenizer.tpl.h ETokens.tpl.h test/TTokenizer/TokenizerTest.tpl.h 
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CBackEndTokenizer.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/CTokenizer.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/ETokens.gen.h" "${CMAKE_CURRENT_SOURCE_DIR}/../test/TTokenizer/TokenizerTest.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/Tokens.tccmd"
        MAIN_DEPENDENCY Tokens.tccmd
        DEPENDS "../requirements/Tag Specification.csv" CTokenizer.tpl.h ETokens.tpl.h ../test/TTokenizer/TokenizerTest.tpl.h 
        COMMENT "Generating Tokens.tccmd"
    )

    #CombiKeywordParameterParser.tccmd >> CombiKeywordParameterParser.gen.h << "../requirements/Tag Specification.CombiParsers.csv" CombiKeywordParameterParser.tpl.h
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/CombiKeywordParameterParser.gen.h"
        COMMAND ${CCK_TCCMD_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/CombiKeywordParameterParser.tccmd"
        MAIN_DEPENDENCY CombiKeywordParameterParser.tccmd
        DEPENDS "../requirements/Tag Specification.CombiParsers.csv" CombiKeywordParameterParser.tpl.h
        COMMENT "Generating CombiKeywordParameterParser.tccmd"
    )

endif()

