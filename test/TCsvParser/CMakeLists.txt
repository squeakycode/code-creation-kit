add_executable(Test_CsvParserRunner
    TCsvParser.cpp
    TCsvParserTestFiles.h
)

target_include_directories(Test_CsvParserRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

add_dependencies(Test_CsvParserRunner cck_implementation)

custom_target_use_highest_warning_level(Test_CsvParserRunner)

add_test(
   NAME Test_CsvParser
   COMMAND Test_CsvParserRunner
)
