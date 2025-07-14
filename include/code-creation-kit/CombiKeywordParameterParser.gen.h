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
        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1CStyle1UIntRepeatUIntOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(2);

            //parse opening parentheses
            ParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type CStyle
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                //parse the seperator, e.g. the comma and surrounding space
                ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
                //parse parameter of type UInt
                UIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for (size_t i = 0; i < SIZE_MAX; ++i)
            {
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                IteratorT temp(start);
                if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                {
                    break;
                }
                start = temp;
                typename ContainerT::value_type parameterValue;
                UIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
            }

            //parse closing parentheses
            ParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1Plain2CStyleOptional1PlainOptional1CStyleOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(1);

            //parse opening parentheses
            ParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type Plain
                parameterValue.clear();
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            ParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi2Plain2CStyleOptional1PlainOptional1CStyleOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(2);

            //parse opening parentheses
            ParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                //parse the seperator, e.g. the comma and surrounding space
                ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
                //parse parameter of type Plain
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type Plain
                parameterValue.clear();
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            ParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1Plain1CStyleOptional1UIntOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(1);

            //parse opening parentheses
            ParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type Plain
                PlainParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type UInt
                parameterValue.clear();
                UIntParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            ParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }


        ///parse parameter range, extract values
        template <typename IteratorT, typename ContainerT>
        void getParametersCombi1CStyle1CStyleOptional(IteratorT& start, const IteratorT& end, ContainerT& parameters)
        {
            parameters.clear();
            parameters.resize(1);

            //parse opening parentheses
            ParameterPolicyBase::parseParameterStart<ExParameterStartExpected>(start, end);
            {
                typename ContainerT::iterator it = parameters.begin();
                //parse parameter of type CStyle
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, *it);
                ++it; //set to the next value to parse
                assert(it == parameters.end()); //all fixed part parameters must be read defined by parameters.resize() above
            }
            
            for(;;)
            {
                typename ContainerT::value_type parameterValue;
                //try to find a seperator, e.g. the comma and surrounding space, otherwise there are no more parameters to parse
                {
                    IteratorT temp(start);
                    if (!ParameterPolicyBase::parseParameterSeparator<ExParameterSeparatorExpected>(temp, end, true))
                    {
                        break;
                    }
                    start = temp;
                }
                //parse parameter of type CStyle
                parameterValue.clear();
                CStyleParameterPolicy::parseParameterValue<ExParameterValueExpected>(start, end, parameterValue);
                parameters.push_back(parameterValue);
                
                break;
            }

            //parse closing parentheses
            ParameterPolicyBase::parseParameterEnd<ExParameterEndExpected>(start, end);
        }
    }
}
