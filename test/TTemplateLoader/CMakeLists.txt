add_executable(Test_TemplateLoaderRunner
    TTemplateLoader.cpp
    TTemplateLoaderTestFiles.h
)

target_include_directories(Test_TemplateLoaderRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_TemplateLoaderRunner)

add_test(
    NAME Test_TemplateLoader
    COMMAND Test_TemplateLoaderRunner
)
