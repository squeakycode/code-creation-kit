//   Copyright (C) 2011-2015 Andreas Gau
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

#pragma once

#include "KeywordParameterParser.h"

namespace KeywordParameterParser
{
    [MACRO_BEGIN][TRIM]
    ///parse parameter range, extract values
    template <typename IteratorT, typename ContainerT>
    void getParameters[ENTRY]["Identifier"](IteratorT& start, const IteratorT& end, ContainerT& parameters)
    {
        parameters.clear();
        [BEGIN][TRIM]
        parameters.resize([ENTRY]["Mandatory Parameter Count"][NOT][EQUALS]["0"]);
        [OR][END][TRIM]

        //parse opening parentheses
        CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
        [BEGIN][TRIM]
        {
            typename ContainerT::iterator it = parameters.begin();
            C[ENTRY]["Fixed Part"][REPLACE][";", "ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it); CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end); ++it;\n            C"]ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it); ++it;
            assert(it == parameters.end());
        }
        [OR][END][TRIM]

        [BEGIN][TRIM]
        for (size_t i = 1; i < [ENTRY]["Variable Part Maximum Count"]; ++i)
        {
            IteratorT temp(start);
            if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
            {
                break;
            }
            start = temp;
            typename ContainerT::value_type parameterValue;
            C[ENTRY]["Variable Part"]ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
            parameters.push_back(parameterValue);
        }

        [OR][END][TRIM]
        //parse closing parentheses
        CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
    }


    [MACRO_END][TRIM]
}