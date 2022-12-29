add_executable(Test_TableIndexRunner TTableIndex.cpp)

target_include_directories(Test_TableIndexRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_TableIndexRunner cck_implementation)

custom_target_use_highest_warning_level(Test_TableIndexRunner)

add_test(
    NAME Test_TableIndex
    COMMAND Test_TableIndexRunner
)
