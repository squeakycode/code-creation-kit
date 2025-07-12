// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

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
