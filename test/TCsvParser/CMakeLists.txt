add_executable(Test_CsvParserRunner
    TCsvParser.cpp
)

target_include_directories(Test_CsvParserRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_CsvParserRunner)

add_test(
   NAME Test_CsvParser
   COMMAND Test_CsvParserRunner
)
