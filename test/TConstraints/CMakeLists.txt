add_executable(Test_ConstraintsRunner TConstraints.cpp)

target_include_directories(Test_ConstraintsRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_ConstraintsRunner cck_implementation)

custom_target_use_highest_warning_level(Test_ConstraintsRunner)

add_test(
    NAME Test_Constraints
    COMMAND Test_ConstraintsRunner
)
