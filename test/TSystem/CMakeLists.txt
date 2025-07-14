add_executable(Test_SystemRunner TSystem.cpp)

target_include_directories(Test_SystemRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_SystemRunner)

add_test(
    NAME Test_System
    COMMAND Test_SystemRunner
)
