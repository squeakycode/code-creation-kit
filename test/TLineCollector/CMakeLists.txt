add_executable(Test_LineCollectorRunner TLineCollector.cpp)

target_include_directories(Test_LineCollectorRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_LineCollectorRunner cck_implementation)

custom_target_use_highest_warning_level(Test_LineCollectorRunner)

add_test(
    NAME Test_LineCollector
    COMMAND Test_LineCollectorRunner
)
