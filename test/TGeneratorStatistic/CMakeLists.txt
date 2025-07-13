add_executable(Test_GeneratorStatisticRunner
    TGeneratorStatistic.cpp
    TGeneratorStatisticTestFiles.h
)

target_include_directories(Test_GeneratorStatisticRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_GeneratorStatisticRunner)

add_test(
    NAME Test_GeneratorStatistic
    COMMAND Test_GeneratorStatisticRunner
)
