add_executable(Test_FileSystemRunner TFileSystem.cpp)

target_include_directories(Test_FileSystemRunner
PRIVATE
    ${PROJECT_SOURCE_DIR}/test/include
    ${PROJECT_SOURCE_DIR}/include/code-creation-kit
)

custom_target_use_highest_warning_level(Test_FileSystemRunner)

add_test(
    NAME Test_FileSystem
    COMMAND Test_FileSystemRunner
)
