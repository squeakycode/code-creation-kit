add_executable(Test_DependencyPrinterRunner
    TDependencyPrinter.cpp
)

target_include_directories(Test_DependencyPrinterRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_DependencyPrinterRunner
    PRIVATE
    cck_implementation
    Boost::filesystem
)

add_dependencies(Test_DependencyPrinterRunner cck_implementation)

custom_target_use_highest_warning_level(Test_DependencyPrinterRunner)

add_test(
    NAME Test_DependencyPrinter
    COMMAND Test_DependencyPrinterRunner
)
