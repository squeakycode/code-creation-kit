add_executable(Test_CommandProcessorRunner
    TCommandProcessor.cpp
    TCommandProcessorTestFiles.h
)

target_include_directories(Test_CommandProcessorRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
    ${PROJECT_SOURCE_DIR}/ccktc
)

custom_target_use_highest_warning_level(Test_CommandProcessorRunner)

add_test(
    NAME Test_CommandProcessor
    COMMAND Test_CommandProcessorRunner
)
