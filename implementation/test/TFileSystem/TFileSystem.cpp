//   Copyright (C) 2011 Andreas Gau
//
//   This file is part of the code-creation-kit.
//
//   The code-creation-kit is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   The code-creation-kit is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "FileSystem.h"
#include <iostream>
#include <fstream>

template <typename StringT>
bool run_test( const StringT& base, const StringT& relative, const StringT& expected)
{
    StringT result = FileSystem::determineDependentLocation<StringT>( base, relative);

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

    if ( result != expected)
    {
        return false;
    }
    return true;
}


BOOST_AUTO_TEST_CASE( TFileSystem)
{
    BOOST_CHECK(  run_test<std::string>( "C:\\dir\\c.txt", "..\\a.txt", "C:/a.txt"));
    BOOST_CHECK(  run_test<std::string>( "zip\\dir\\c.txt", "..\\a.txt", "zip/a.txt"));
    BOOST_CHECK(  run_test<std::wstring>( L"C:\\dir\\c.txt", L"D:\\ddir\\c.txt", L"D:/ddir/c.txt"));
    BOOST_CHECK(  run_test<std::string>( "C:\\c.txt", "a.txt", "C:/a.txt"));

    BOOST_CHECK(  run_test_relative<std::wstring>( L"C:\\dir\\c.txt", L"D:\\dir\\c.txt", L"D:\\dir\\c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "D:\\dir\\c.txt", "D:\\dir\\c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "c:\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\..\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "C:\\dir\\a\\c.txt", "a/c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a\\x.txt", "../../x.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a", "../.."));
    //BOOST_CHECK(  run_test_relative<std::string>( "C:\\a.txt\\a.txt", "C:\\a.txt", "a.txt")); //todo

    BOOST_CHECK( FileSystem::determineFilename<std::string>( "dir\\a.txt") == "a.txt");

    std::cout << FileSystem::determineDependentLocation<std::string>( "dir\\a.txt");

    BOOST_CHECK( FileSystem::isRegularFile( std::string("TFileSystem.cpp")));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("NotExistant.cpp")));

    //create test file
    {
        std::ofstream("_temp_test_file_.txt");
    }

    BOOST_CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    FileSystem::moveFile( std::string("_temp_test_file_.txt"), std::string("_temp_test_file_moved_.txt"));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    BOOST_CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));
    BOOST_CHECK_NO_THROW( FileSystem::removeFile( std::string("_does_not_exist_.txt")));
    BOOST_CHECK( FileSystem::removeFile( std::string("_temp_test_file_moved_.txt")));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));
}

