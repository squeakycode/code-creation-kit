add_executable(Test_GeneratorRunner
    TGenerator.cpp
    TGeneratorTestFiles.h
)

target_include_directories(Test_GeneratorRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_GeneratorRunner)

add_test(
    NAME Test_Generator
    COMMAND Test_GeneratorRunner
)
