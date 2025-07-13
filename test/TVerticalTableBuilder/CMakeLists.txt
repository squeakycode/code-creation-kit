add_executable(Test_VerticalTableBuilderRunner TVerticalTableBuilder.cpp)

target_include_directories(Test_VerticalTableBuilderRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_VerticalTableBuilderRunner)

add_test(
    NAME Test_VerticalTableBuilder
    COMMAND Test_VerticalTableBuilderRunner
)
