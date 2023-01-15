add_executable(Test_GeneratorRunner
    TGenerator.cpp
    TGeneratorTestFiles.h
)

target_include_directories(Test_GeneratorRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_GeneratorRunner
    PRIVATE
    cck_implementation
)

add_dependencies(Test_GeneratorRunner cck_implementation)

custom_target_use_highest_warning_level(Test_GeneratorRunner)

add_test(
    NAME Test_Generator
    COMMAND Test_GeneratorRunner
)
