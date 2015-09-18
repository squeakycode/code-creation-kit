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
    ///parse parameter range, extract values
    template <typename IteratorT, typename ContainerT>
    void getParametersCombiCStyleUIntUIntRepeat(IteratorT& start, const IteratorT& end, ContainerT& parameters)
    {
        parameters.clear();
        parameters.resize(2);

        //parse opening parentheses
        CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
        {
            typename ContainerT::iterator it = parameters.begin();
            CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it); CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end); ++it;
            CUIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it); ++it;
            assert(it == parameters.end());
        }

        for (size_t i = 1; i < SIZE_MAX; ++i)
        {
            IteratorT temp(start);
            if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
            {
                break;
            }
            start = temp;
            typename ContainerT::value_type parameterValue;
            CUIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
            parameters.push_back(parameterValue);
        }

        //parse closing parentheses
        CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
    }


}