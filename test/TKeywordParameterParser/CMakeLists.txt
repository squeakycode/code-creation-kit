add_executable(Test_KeywordParameterParserRunner TKeywordParameterParser.cpp)

target_include_directories(Test_KeywordParameterParserRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_KeywordParameterParserRunner)

add_test(
    NAME Test_KeywordParameterParser
    COMMAND Test_KeywordParameterParserRunner
)
