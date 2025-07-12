// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "FileSystem.h"
#include <iostream>
#include <fstream>

using namespace code_creation_kit;

template <typename StringT>
bool run_test( const StringT& base, const StringT& relative, const StringT& expected)
{
    StringT result = FileSystem::determineDependentLocation<StringT>( base, relative);

    CHECK(result == expected);
    if ( result != expected)
    {
        return false;
    }
    return true;
}

template <typename StringT>
bool run_test_relative( const StringT& base, const StringT& relative, const StringT& expected)
{
    StringT result = FileSystem::determineRelativeLocation<StringT>( base, relative);

    CHECK(result == expected);
    if ( result != expected)
    {
        return false;
    }
    return true;
}


TEST_CASE( "TFileSystem", "[TFileSystem]")
{
#ifdef _MSC_VER //TODO
    CHECK(  run_test<std::string>( "C:\\dir\\c.txt", "..\\a.txt", "C:/a.txt"));
    CHECK(  run_test<std::string>( "zip\\dir\\c.txt", "..\\a.txt", "zip/a.txt"));
    CHECK(  run_test<std::wstring>( L"C:\\dir\\c.txt", L"D:\\ddir\\c.txt", L"D:/ddir/c.txt"));
    CHECK(  run_test<std::string>( "C:\\c.txt", "a.txt", "C:/a.txt"));

    CHECK(  run_test_relative<std::wstring>( L"C:\\dir\\c.txt", L"D:\\dir\\c.txt", L"D:/dir/c.txt"));
    CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "D:\\dir\\c.txt", "D:/dir/c.txt"));
    CHECK(  run_test_relative<std::string>( "c:\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    CHECK(  run_test_relative<std::string>( "C:\\dir\\..\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "C:\\dir\\a\\c.txt", "a/c.txt"));
    CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a\\x.txt", "../../x.txt"));
    CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a", "../.."));
    //CHECK(  run_test_relative<std::string>( "C:\\a.txt\\a.txt", "C:\\a.txt", "a.txt")); //todo

    CHECK( FileSystem::determineFilename<std::string>( "dir\\a.txt") == "a.txt");
#endif

    std::cout << FileSystem::determineDependentLocation<std::string>( "dir\\a.txt");

    //create test file
    {
        std::ofstream("_temp_test_file_.txt");
    }

    CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    CHECK( !FileSystem::isRegularFile( std::string("NotExistant.cpp")));

    CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    FileSystem::moveFile( std::string("_temp_test_file_.txt"), std::string("_temp_test_file_moved_.txt"));
    CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));
    CHECK_NOTHROW( FileSystem::removeFile( std::string("_does_not_exist_.txt")));
    CHECK( FileSystem::removeFile( std::string("_temp_test_file_moved_.txt")));
    CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));

    ////create test file
    //{
    //    std::ofstream("_temp_test_file_.txt");
    //}
    //FileSystem::recycleFile( std::string("_temp_test_file_.txt"));
}
