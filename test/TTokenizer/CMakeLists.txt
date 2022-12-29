add_executable(Test_TokenizerRunner
    TTokenizer.cpp
    TokenizerTest.tpl.h
    TokenizerTest.gen.h
)

target_include_directories(Test_TokenizerRunner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
$<TARGET_PROPERTY:cck_implementation,INTERFACE_INCLUDE_DIRECTORIES>
)

add_dependencies(Test_TokenizerRunner cck_implementation)

custom_target_use_highest_warning_level(Test_TokenizerRunner)

add_test(
    NAME Test_Tokenizer
    COMMAND Test_TokenizerRunner
)