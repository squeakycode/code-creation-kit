add_executable(ccktc
    ../include/code-creation-kit/CBackEndTokenizer.gen.h
    ../include/code-creation-kit/CComparableSharedObject.h
    ../include/code-creation-kit/CCsvParser.h
    ../include/code-creation-kit/CErrorPrinter.gen.h
    ../include/code-creation-kit/CFilesBinaryEqual.h
    ../include/code-creation-kit/CGenerator.h
    ../include/code-creation-kit/CGeneratorStatistic.h
    ../include/code-creation-kit/CInlineTemplateParameters.h
    ../include/code-creation-kit/CLineCollector.h
    ../include/code-creation-kit/CMacro.h
    ../include/code-creation-kit/CMacroExpander.h
    ../include/code-creation-kit/CMacroExpression.h
    ../include/code-creation-kit/CMacroProcessor.h
    ../include/code-creation-kit/CNul.h
    ../include/code-creation-kit/CombiKeywordParameterParser.gen.h
    ../include/code-creation-kit/ConstraintDirectives.gen.h
    ../include/code-creation-kit/Constraints.h
    ../include/code-creation-kit/ConversionDirectives.gen.h
    ../include/code-creation-kit/Conversions.h
    ../include/code-creation-kit/CParser.h
    ../include/code-creation-kit/CPositionTracker.h
    ../include/code-creation-kit/cppstringx.hpp
    ../include/code-creation-kit/cpptokenfinder.hpp
    ../include/code-creation-kit/CProcessingLevelControl.h
    ../include/code-creation-kit/CSimpleFormat.h
    ../include/code-creation-kit/CSourceFile.h
    ../include/code-creation-kit/CSubstitution.h
    ../include/code-creation-kit/CTableIndex.h
    ../include/code-creation-kit/CTargetFile.h
    ../include/code-creation-kit/CTemplateLoader.h
    ../include/code-creation-kit/CTemplatePreprocessor.h
    ../include/code-creation-kit/CTemplateProcessor.h
    ../include/code-creation-kit/CTemplateProvidedTableLoader.h
    ../include/code-creation-kit/CToken.h
    ../include/code-creation-kit/CTokenizer.gen.h
    ../include/code-creation-kit/CVerticalTableBuilder.h
    ../include/code-creation-kit/CVerticalTableRotator.h
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
