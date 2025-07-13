add_executable(Test_FilesBinaryEqualRunner
    TFilesBinaryEqual.cpp
    TFilesBinaryEqualTestFiles.h
)

target_include_directories(Test_FilesBinaryEqualRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_FilesBinaryEqualRunner)

add_test(
   NAME Test_FilesBinaryEqual
   COMMAND Test_FilesBinaryEqualRunner
)
