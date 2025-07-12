// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

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
    inline StringOutT ToString(IntT /*number*/)
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
