#  Copyright (c) 2011-2023 Andreas Gau
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of the copyright holder nor the
#        names of contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.8)

#we only need debug and release configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE )
set(CCK_WITH_TESTS ON CACHE BOOL "Generate code-creation-kit test projects.")
set(CCK_BUILD_WITH_INSTALLED_CCKTC ON CACHE BOOL "Use installed ccktc executable to generate code while building otherwise pregenerated code is used.")
set(CCK_WITH_WIN32_STATIC_RUNTIME ON CACHE BOOL "Generate code-creation-kit with static runtime under Windows.")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#we are building code creation kit
project(cck CXX)

if(MSVC)
    cmake_policy(SET CMP0074 NEW) #The NEW behavior for this policy is to use <PackageName>_ROOT variables.
endif()

#if the code creation kit is installed use it for building, otherwise use pre generated files, that are checked in
if (DEFINED ENV{CCK_ROOT} AND CCK_BUILD_WITH_INSTALLED_CCKTC)
    if(WIN32) # Windows
        if(EXISTS "$ENV{CCK_ROOT}/bin/ccktc.exe")
            message(STATUS "Found installed ccktc executable. It will be used for building.")
            set (CCK_TCCMD_EXECUTABLE "$ENV{CCK_ROOT}/bin/ccktc.exe")
        endif()
    else()
        if(EXISTS "$ENV{CCK_ROOT}/bin/ccktc")
            message(STATUS "Found installed ccktc executable. It will be used for building.")
            set (CCK_TCCMD_EXECUTABLE "$ENV{CCK_ROOT}/bin/ccktc")
        endif()
    endif()
endif()

#use highest warning levels
macro(custom_target_use_highest_warning_level TargetName)
if(MSVC)
  target_compile_options(${TargetName} PRIVATE /W4 /WX /wd4996) #4996 produces too many pseudo warnings
else()
  target_compile_options(${TargetName} PRIVATE -Wall -Wextra -pedantic) #-Werror
endif()
endmacro(custom_target_use_highest_warning_level)


#static linking of c runtime for windows
if(MSVC)
    if (CCK_WITH_WIN32_STATIC_RUNTIME)
        #compile with std runtime linked statically
        set(CompilerFlags
                CMAKE_CXX_FLAGS
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                )
        foreach(CompilerFlag ${CompilerFlags})
          string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()  
    endif()
endif()

#add the sub directories to build
add_subdirectory(implementation)
add_subdirectory(ccktc)

option(CCK_BUILD_TESTS "Determines whether to build tests." ON)
if(CCK_BUILD_TESTS)
    message("Building tests.")
    include(CTest)
    add_subdirectory(test/TCommandProcessor)
    add_subdirectory(test/TConstraints)
    add_subdirectory(test/TConversions)
    add_subdirectory(test/TCsvParser)
    add_subdirectory(test/TDependencyPrinter)
    add_subdirectory(test/TErrorPrinter)
    add_subdirectory(test/TFilesBinaryEqual)
    add_subdirectory(test/TFileSystem)
    add_subdirectory(test/TGenerator)
    add_subdirectory(test/TGeneratorStatistic)
    add_subdirectory(test/TKeywordParameterParser)
    add_subdirectory(test/TLineCollector)
    add_subdirectory(test/TParameterParser)
    add_subdirectory(test/TSourceFile)
    add_subdirectory(test/TSpecialRegexCharacterPrefixer)
    add_subdirectory(test/TSystem)
    add_subdirectory(test/TTableIndex)
    add_subdirectory(test/TTemplateLoader)
    add_subdirectory(test/TTemplateProcessor)
    add_subdirectory(test/TTokenizer)
    add_subdirectory(test/TVerticalTableBuilder)
endif()
