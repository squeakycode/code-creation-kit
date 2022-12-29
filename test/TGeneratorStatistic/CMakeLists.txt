add_executable(Test_GeneratorStatisticRunner
    TGeneratorStatistic.cpp
    TGeneratorStatisticTestFiles.h
)

target_include_directories(Test_GeneratorStatisticRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_GeneratorStatisticRunner
    PRIVATE
    cck_implementation
    Boost::filesystem
)

add_dependencies(Test_GeneratorStatisticRunner cck_implementation)

custom_target_use_highest_warning_level(Test_GeneratorStatisticRunner)

add_test(
    NAME Test_GeneratorStatistic
    COMMAND Test_GeneratorStatisticRunner
)
