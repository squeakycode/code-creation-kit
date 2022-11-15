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

#include <map>
#include <list>
#include <stdexcept>

namespace code_creation_kit
{
    namespace ParameterParser
    {
        class ExMissingDelimiterInParameterDescription : public std::runtime_error 
        { public: ExMissingDelimiterInParameterDescription() : std::runtime_error( "Parameter syntax error, use format \"name=value\".") {}};

        ///parses the parameter list and creates a table from the provided parameters
        template <typename ParameterContainerT, typename TableT, typename CharT>
        void parse( const ParameterContainerT& parameterList, TableT& table, typename ParameterContainerT::size_type& indexOfLastProcessedParameter, CharT delimiter)
        {
            typedef typename ParameterContainerT::value_type StringT;
            typedef std::list<StringT> ValueListT;
            typedef typename ValueListT::size_type SizeT;
            typedef std::map<StringT, ValueListT> ParameterMapT;
            typedef typename ParameterMapT::value_type PairT;

            //create list of values from one parameter
            ParameterMapT map;
            indexOfLastProcessedParameter = 0;
            for (const StringT& parameter : parameterList)
            {
                typename StringT::const_iterator it = parameter.begin();
                for(; it != parameter.end(); ++it)
                {
                    if ( *it == delimiter)
                    {
                        break;
                    }
                }
                if ( it == parameter.end() )
                {
                    throw ExMissingDelimiterInParameterDescription();
                }
                map[ StringT( parameter.begin(), it)].push_back( StringT( it + 1, parameter.end()));
                ++indexOfLastProcessedParameter;
            }

            ///determine the maximum list size and count the parameters
            SizeT max = 0;
            SizeT count = 0;
            for (const PairT& pair : map)
            {
                if ( pair.second.size() > max)
                {
                    max = pair.second.size();
                }
                ++count;
            }

            //update the table
            table.clear();
            //resize the table
            table.resize( count);
            for ( SizeT i = 0; i < count; ++i)
            {
                table[i].resize( max + 1); //plus one for the name
            }

            //transfer the values
            SizeT indexColumn = 0;
            for (PairT& pair : map)
            {
                typename TableT::value_type& column = table[ indexColumn ];
                ValueListT& valueList = pair.second;
                column[ 0 ] = pair.first;
                SizeT indexRow = 1;
                for (StringT& value : valueList)
                {
                    column[ indexRow ].swap( value);
                    ++indexRow;
                }
                ++indexColumn;
            }
        }
    }
}
