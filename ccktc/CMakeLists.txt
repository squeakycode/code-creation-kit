add_executable(ccktc
    ../include/code-creation-kit/BackEndTokenizer.gen.h
    ../include/code-creation-kit/ComparableSharedObject.h
    ../include/code-creation-kit/CsvParser.h
    ../include/code-creation-kit/ErrorPrinter.gen.h
    ../include/code-creation-kit/FilesBinaryEqual.h
    ../include/code-creation-kit/Generator.h
    ../include/code-creation-kit/GeneratorStatistic.h
    ../include/code-creation-kit/InlineTemplateParameters.h
    ../include/code-creation-kit/LineCollector.h
    ../include/code-creation-kit/Macro.h
    ../include/code-creation-kit/MacroExpander.h
    ../include/code-creation-kit/MacroExpression.h
    ../include/code-creation-kit/MacroProcessor.h
    ../include/code-creation-kit/Nul.h
    ../include/code-creation-kit/CombiKeywordParameterParser.gen.h
    ../include/code-creation-kit/ConstraintDirectives.gen.h
    ../include/code-creation-kit/Constraints.h
    ../include/code-creation-kit/ConversionDirectives.gen.h
    ../include/code-creation-kit/Conversions.h
    ../include/code-creation-kit/Parser.h
    ../include/code-creation-kit/PositionTracker.h
    ../include/code-creation-kit/cppstringx.hpp
    ../include/code-creation-kit/cpptokenfinder.hpp
    ../include/code-creation-kit/ProcessingLevelControl.h
    ../include/code-creation-kit/SimpleFormat.h
    ../include/code-creation-kit/SourceFile.h
    ../include/code-creation-kit/Substitution.h
    ../include/code-creation-kit/TableIndex.h
    ../include/code-creation-kit/TargetFile.h
    ../include/code-creation-kit/TemplateLoader.h
    ../include/code-creation-kit/TemplatePreprocessor.h
    ../include/code-creation-kit/TemplateProcessor.h
    ../include/code-creation-kit/TemplateProvidedTableLoader.h
    ../include/code-creation-kit/Token.h
    ../include/code-creation-kit/Tokenizer.gen.h
    ../include/code-creation-kit/VerticalTableBuilder.h
    ../include/code-creation-kit/VerticalTableRotator.h
    ../include/code-creation-kit/DependencyPrinter.h
    ../include/code-creation-kit/ESubstitutions.gen.h
    ../include/code-creation-kit/ETokens.gen.h
    ../include/code-creation-kit/FileSystem.h
    ../include/code-creation-kit/IConstraint.h
    ../include/code-creation-kit/IConversion.h
    ../include/code-creation-kit/KeywordParameterCheckFunctions.h
    ../include/code-creation-kit/KeywordParameterParser.h
    ../include/code-creation-kit/ParserExtensions.gen.h
    ../include/code-creation-kit/StringConvert.h
    ../include/code-creation-kit/StringLiteral.h
    ../include/code-creation-kit/System.h
    version.gen.h
    ccktc.cpp
)

target_include_directories(ccktc
    PRIVATE
    ../include/code-creation-kit
)

custom_target_use_highest_warning_level(ccktc)

install(TARGETS ccktc
        RUNTIME DESTINATION bin
)

# ccktc executable is linked with static runtime for Windows and Release build
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(ccktc PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded"
    )
endif()
