add_executable(Test_DependencyPrinterRunner
    TDependencyPrinter.cpp
)

target_include_directories(Test_DependencyPrinterRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_DependencyPrinterRunner)

add_test(
    NAME Test_DependencyPrinter
    COMMAND Test_DependencyPrinterRunner
)
