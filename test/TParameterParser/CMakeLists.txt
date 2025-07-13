add_executable(Test_ParameterParserRunner TParameterParser.cpp)

target_include_directories(Test_ParameterParserRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_ParameterParserRunner)

add_test(
    NAME Test_ParameterParser
    COMMAND Test_ParameterParserRunner
)
