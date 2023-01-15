add_executable(Test_TemplateLoaderRunner
    TTemplateLoader.cpp
    TTemplateLoaderTestFiles.h
)

target_include_directories(Test_TemplateLoaderRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_TemplateLoaderRunner
    PRIVATE
    cck_implementation
)

add_dependencies(Test_TemplateLoaderRunner cck_implementation)

custom_target_use_highest_warning_level(Test_TemplateLoaderRunner)

add_test(
    NAME Test_TemplateLoader
    COMMAND Test_TemplateLoaderRunner
)
