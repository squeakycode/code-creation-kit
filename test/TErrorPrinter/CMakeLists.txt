add_executable(Test_ErrorPrinterRunner
    TErrorPrinter.cpp
    TErrorPrinterTestFiles.h
)

target_include_directories(Test_ErrorPrinterRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_ErrorPrinterRunner
    PRIVATE
    cck_implementation
    Boost::filesystem
)

add_dependencies(Test_ErrorPrinterRunner cck_implementation)

custom_target_use_highest_warning_level(Test_ErrorPrinterRunner)

add_test(
    NAME Test_ErrorPrinter
    COMMAND Test_ErrorPrinterRunner
)
