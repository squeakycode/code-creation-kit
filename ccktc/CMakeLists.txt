set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/bin)

add_executable(ccktc ccktc.cpp)
target_link_libraries(ccktc
    PRIVATE
    cck_implementation 
)

custom_target_use_highest_warning_level(ccktc)
