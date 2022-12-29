add_executable(Test_ConversionsRunner TConversions.cpp)

target_include_directories(Test_ConversionsRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_ConversionsRunner cck_implementation)

custom_target_use_highest_warning_level(Test_ConversionsRunner)

add_test(
    NAME Test_Conversions
    COMMAND Test_ConversionsRunner
)
