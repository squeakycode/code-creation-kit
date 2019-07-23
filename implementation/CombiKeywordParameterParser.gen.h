//  Copyright (c) 2011-2019 Andreas Gau
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

#include <cassert>
#include "KeywordParameterParser.h"
#ifndef _MSC_VER
#include <stdint.h>
#endif

namespace code_creation_kit
{
    namespace KeywordParameterParser
    {
        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1CStyle1UIntRepeatUIntOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(2);

            //parse opening parentheses
            CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type CStyle
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                //parse the seperator, e.g. the comma and surrounding space
                CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
                //parse parameter of type UInt
                CUIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for (size_t i = 0; i < SIZE_MAX; ++i)
            {
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
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


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1Plain2CStyleOptional1PlainOptional1CStyleOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(1);

            //parse opening parentheses
            CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type Plain
                parameterValue.clear();
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi2Plain2CStyleOptional1PlainOptional1CStyleOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(2);

            //parse opening parentheses
            CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                //parse the seperator, e.g. the comma and surrounding space
                CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
                //parse parameter of type Plain
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type Plain
                parameterValue.clear();
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1Plain1CStyleOptional1UIntOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(1);

            //parse opening parentheses
            CParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                CPlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CCStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!CParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type UInt
                parameterValue.clear();
                CUIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            CParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }
    }
}
