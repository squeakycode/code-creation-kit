add_executable(Test_ErrorPrinterRunner
    TErrorPrinter.cpp
    TErrorPrinterTestFiles.h
)

target_include_directories(Test_ErrorPrinterRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_ErrorPrinterRunner)

add_test(
    NAME Test_ErrorPrinter
    COMMAND Test_ErrorPrinterRunner
)
