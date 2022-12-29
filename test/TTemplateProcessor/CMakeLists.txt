add_executable(Test_TemplateProcessorRunner
    TTemplateProcessor.h
    TTemplateProcessor.cpp
    TTemplateProcessorW.cpp
)

target_include_directories(Test_TemplateProcessorRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_TemplateProcessorRunner cck_implementation)

custom_target_use_highest_warning_level(Test_TemplateProcessorRunner)

add_test(
    NAME Test_TemplateProcessor
    COMMAND Test_TemplateProcessorRunner
)
