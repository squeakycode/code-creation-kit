//   Copyright (C) 2011-2012 Andreas Gau
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

#ifndef INCLUDED_PARAMETERPARSER_H_3718531
#define INCLUDED_PARAMETERPARSER_H_3718531

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <map>
#include <list>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
#endif
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#include <boost/foreach.hpp>
#include <stdexcept>

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
        BOOST_FOREACH( const StringT& parameter, parameterList)
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
        BOOST_FOREACH( const PairT& pair, map)
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
        BOOST_FOREACH( PairT& pair, map)
        {
            typename TableT::value_type& column = table[ indexColumn ];
            ValueListT& valueList = pair.second;
            column[ 0 ] = pair.first;
            SizeT indexRow = 1;
            BOOST_FOREACH( StringT& value, valueList)
            {
                column[ indexRow ].swap( value);
                ++indexRow;
            }
            ++indexColumn;
        }
    }
}

#endif /* INCLUDED_PARAMETERPARSER_H_3718531 */
