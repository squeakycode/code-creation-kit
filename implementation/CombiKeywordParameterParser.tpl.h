// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <cassert>
#include "KeywordParameterParser.h"
#ifndef _MSC_VER
#include <stdint.h>
#endif

namespace code_creation_kit
{
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
                [TABLE_BEGIN.]["Fixed Part Table"]Fixed Part Row;[ENTRY]["Fixed Part"][TABLE_END.][TRIM.]
                [MACRO_BEGIN.][TRIM.]
                //parse parameter of type [ENTRY.]["Fixed Part Row"]
                C[ENTRY.]["Fixed Part Row"]ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                [BEGIN.][IF.][NOT.][LAST_TIME.][TRIM.]
                //parse the seperator, e.g. the comma and surrounding space
                CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
                [OR.][END.][TRIM.]
                [MACRO_END.][TRIM.]
                [TABLE_REMOVE.]["Fixed Part Table"][TRIM.]
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            [OR][END][TRIM]
            [BEGIN][TRIM]
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                [TABLE_BEGIN.]["Optional Part Table"]Optional Part Row;[ENTRY]["Optional Part"][TABLE_END.][TRIM.]
                [MACRO_BEGIN.][TRIM.]
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type [ENTRY.]["Optional Part Row"]
                parameterValue.clear();
                C[ENTRY.]["Optional Part Row"]ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                [MACRO_END.][TRIM.]
                [TABLE_REMOVE.]["Optional Part Table"][TRIM.]
                break;
            }

            [OR][END][TRIM]            
            [BEGIN][TRIM]
            for (size_t i = 0; i < [ENTRY]["Repeat Part Maximum Count"]; ++i)
            {
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                IteratorT temp(start);
                if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                {
                    break;
                }
                start = temp;
                typename ContainerT::value_type parameterValue;
                C[ENTRY]["Repeat Part"]ParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
            }

            [OR][END][TRIM]
            //parse closing parentheses
            CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }
        [BEGIN][IF][NOT][LAST_TIME][TRIM]


        [OR][END][TRIM]
        [MACRO_END][TRIM]
    }
}
