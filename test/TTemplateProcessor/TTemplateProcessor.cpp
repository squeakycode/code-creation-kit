// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "TTemplateProcessor.h"

TEST_CASE("TTemplateProcessor", "[TTemplateProcessor]")
{
    testTemplateProcessor<std::string>();
    testMacroProcessing<std::string>();
}

TEST_CASE("TTemplateProcessor MacroProcessing", "[TTemplateProcessor]")
{
    testMacroProcessing<std::string>();
}
