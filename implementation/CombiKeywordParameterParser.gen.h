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

#pragma once

#include "KeywordParameterParser.h"
#ifndef _MSC_VER
#include <stdint.h>
#endif

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
