add_executable(Test_ConversionsRunner TConversions.cpp)

target_include_directories(Test_ConversionsRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_ConversionsRunner)

add_test(
    NAME Test_Conversions
    COMMAND Test_ConversionsRunner
)
