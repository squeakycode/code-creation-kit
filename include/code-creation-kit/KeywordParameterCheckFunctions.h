// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <stdexcept>

namespace code_creation_kit
{
    namespace KeywordParameterParser
    {
        class ExCharsUsedForPaddingNotSupported : public std::runtime_error
        {
        public: ExCharsUsedForPaddingNotSupported() : std::runtime_error("The text passed for padding contains unsupported characters.") {}
        };
    }

    template <typename ContainerT>
    void checkPadParameters(const ContainerT& container)
    {
        const typename ContainerT::value_type& first = container.front();
        for (typename ContainerT::value_type::const_iterator it = first.begin(); it != first.end(); ++it)
        {
            if (*it < ' ')
            {
                throw KeywordParameterParser::ExCharsUsedForPaddingNotSupported();
            }
        }
    }
}
