//  Copyright (c) 2011-2023 Andreas Gau
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

#pragma once
#include "cppstringx.hpp"
#include <string>

namespace code_creation_kit
{
    //use only for ASCII character strings
    template <typename StringOutT, typename StringInT>
    inline StringOutT StringConvert(const StringInT& string)
    {
        StringOutT result = cppstringx::copy<StringOutT>(string);
        return result;
    }

    template <typename StringOutT, typename IntT>
    inline StringOutT ToString(IntT number)
    {
        //extend the ToString variants below if you get compile errors here
    }

    template <>
    inline std::string ToString<std::string, int>(int number)
    {
        std::string result = std::to_string(number);
        return result;
    }

    template <>
    inline std::wstring ToString<std::wstring, int>(int number)
    {
        std::wstring result = std::to_wstring(number);
        return result;
    }

    template <>
    inline std::string ToString<std::string, size_t>(size_t number)
    {
        std::string result = std::to_string(number);
        return result;
    }

    template <>
    inline std::wstring ToString<std::wstring, size_t>(size_t number)
    {
        std::wstring result = std::to_wstring(number);
        return result;
    }

    template <>
    inline std::string ToString<std::string, int64_t>(int64_t number)
    {
        std::string result = std::to_string(number);
        return result;
    }

    template <>
    inline std::wstring ToString<std::wstring, int64_t>(int64_t number)
    {
        std::wstring result = std::to_wstring(number);
        return result;
    }
}
