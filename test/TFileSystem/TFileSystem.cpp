//  Copyright (c) 2011-2015 Andreas Gau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//      * Neither the name of the copyright holder nor the
//        names of contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "FileSystem.h"
#include <iostream>
#include <fstream>

using namespace code_creation_kit;

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
#ifdef _MSC_VER //TODO
    BOOST_CHECK(  run_test<std::string>( "C:\\dir\\c.txt", "..\\a.txt", "C:/a.txt"));
    BOOST_CHECK(  run_test<std::string>( "zip\\dir\\c.txt", "..\\a.txt", "zip/a.txt"));
    BOOST_CHECK(  run_test<std::wstring>( L"C:\\dir\\c.txt", L"D:\\ddir\\c.txt", L"D:/ddir/c.txt"));
    BOOST_CHECK(  run_test<std::string>( "C:\\c.txt", "a.txt", "C:/a.txt"));

    BOOST_CHECK(  run_test_relative<std::wstring>( L"C:\\dir\\c.txt", L"D:\\dir\\c.txt", L"D:/dir/c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "D:\\dir\\c.txt", "D:/dir/c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "c:\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\..\\dir\\c.txt", "C:\\dir\\c.txt", "c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\dir\\c.txt", "C:\\dir\\a\\c.txt", "a/c.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a\\x.txt", "../../x.txt"));
    BOOST_CHECK(  run_test_relative<std::string>( "C:\\a\\b\\c\\c.txt", "C:\\a", "../.."));
    //BOOST_CHECK(  run_test_relative<std::string>( "C:\\a.txt\\a.txt", "C:\\a.txt", "a.txt")); //todo

    BOOST_CHECK( FileSystem::determineFilename<std::string>( "dir\\a.txt") == "a.txt");
#endif

    std::cout << FileSystem::determineDependentLocation<std::string>( "dir\\a.txt");

    //create test file
    {
        std::ofstream("_temp_test_file_.txt");
    }

    BOOST_CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("NotExistant.cpp")));

    BOOST_CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    FileSystem::moveFile( std::string("_temp_test_file_.txt"), std::string("_temp_test_file_moved_.txt"));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_.txt")));
    BOOST_CHECK( FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));
    BOOST_CHECK_NO_THROW( FileSystem::removeFile( std::string("_does_not_exist_.txt")));
    BOOST_CHECK( FileSystem::removeFile( std::string("_temp_test_file_moved_.txt")));
    BOOST_CHECK( !FileSystem::isRegularFile( std::string("_temp_test_file_moved_.txt")));

    ////create test file
    //{
    //    std::ofstream("_temp_test_file_.txt");
    //}
    //FileSystem::recycleFile( std::string("_temp_test_file_.txt"));


}

