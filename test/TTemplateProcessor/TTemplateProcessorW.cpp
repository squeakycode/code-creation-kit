// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#include <catch2/catch.hpp>
#include "TTemplateProcessor.h"

//this is separated to avoid fatal error C1128: number of sections exceeded object file format limit: compile with /bigobj
TEST_CASE("TTemplateProcessorW", "[TTemplateProcessor]")
{
#ifdef _MSC_VER
    testTemplateProcessor<std::wstring>();
#endif 
}
