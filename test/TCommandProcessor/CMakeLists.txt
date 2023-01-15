add_executable(Test_CommandProcessorRunner
    TCommandProcessor.cpp
    TCommandProcessorTestFiles.h
)

target_include_directories(Test_CommandProcessorRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_CommandProcessorRunner
    PRIVATE
    cck_implementation
)

add_dependencies(Test_CommandProcessorRunner cck_implementation)

custom_target_use_highest_warning_level(Test_CommandProcessorRunner)

add_test(
    NAME Test_CommandProcessor
    COMMAND Test_CommandProcessorRunner
)
