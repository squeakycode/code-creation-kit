add_executable(Test_FileSystemRunner TFileSystem.cpp)

target_include_directories(Test_FileSystemRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(Test_FileSystemRunner
    PRIVATE
    cck_implementation
)

add_dependencies(Test_FileSystemRunner cck_implementation)

custom_target_use_highest_warning_level(Test_FileSystemRunner)

add_test(
    NAME Test_FileSystem
    COMMAND Test_FileSystemRunner
)
