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

#ifndef INCLUDED_DEPENDENCYPRINTER_H_8050457
#define INCLUDED_DEPENDENCYPRINTER_H_8050457

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <string.h>
#include <boost/foreach.hpp>
#include "FileSystem.h"

namespace DependencyPrinter
{
    template <typename ContainerT, typename StringT>
    void concatenate( const ContainerT& container, typename StringT::value_type quote, typename StringT::value_type delimiter, StringT& result)
    {
        BOOST_FOREACH( const typename ContainerT::value_type& item, container)
        {
            bool needsQuotes = std::find( item.begin(), item.end(), delimiter) != item.end();
            if ( !result.empty())
            {
                result += delimiter;
            }
            if ( needsQuotes )
            {
                result += quote;
            }
            result += item;
            if ( needsQuotes )
            {
                result += quote;
            }
        }
    }

    template <typename ContainerT, typename StringT>
    ContainerT toRelativePosition( const ContainerT& container, const StringT& baseLocation)
    {
        ContainerT result;
        BOOST_FOREACH( const typename ContainerT::value_type& item, container)
        {
            result.insert( FileSystem::determineRelativeLocation( baseLocation, item));
        }
        return result;
    }

    template <typename ContainerT, typename StringT, typename StreamT>
    void printMpc( const ContainerT& tables, const ContainerT& generatedFiles, const ContainerT& templateFiles, const StringT& baseLocation, StreamT& stream)
    {
        typedef typename StringT::value_type CharT;

        CharT quote = stream.widen( '"');
        CharT delimiter = stream.widen( ' ');
        StringT inputs;
        StringT outputs;
        StringT commandFile = FileSystem::determineFilename( baseLocation);

        concatenate( toRelativePosition( tables, baseLocation), quote, delimiter, inputs);
        concatenate( toRelativePosition( templateFiles, baseLocation), quote, delimiter, inputs);
        concatenate( toRelativePosition( generatedFiles, baseLocation), quote, delimiter, outputs);

        stream << commandFile << " >> " << outputs << " << " << inputs;
    }

    template <typename ContainerT, typename StringT, typename StreamT>
    void printVs( const ContainerT& tables, const ContainerT& generatedFiles, const ContainerT& templateFiles, const StringT& baseLocation, StreamT& stream)
    {
        typedef typename StringT::value_type CharT;

        CharT quote = stream.widen( '"');
        CharT delimiter = stream.widen( ';');
        StringT inputs;
        StringT outputs;
        StringT commandFile = FileSystem::determineFilename( baseLocation);

        concatenate( toRelativePosition( tables, baseLocation), quote, delimiter, inputs);
        concatenate( toRelativePosition( templateFiles, baseLocation), quote, delimiter, inputs);
        concatenate( toRelativePosition( generatedFiles, baseLocation), quote, delimiter, outputs);

        stream << "Parameters for creating a custom build step for file:" << std::endl;
        stream << commandFile << std::endl << std::endl;
        stream << "Command Line:" << std::endl;
        stream << "$(CCK_ROOT)\\bin\\ccktc.exe \"$(InputPath)\"" << std::endl << std::endl;
        stream << "Description:" << std::endl;
        stream << "Invoking code-creation-kit text compiler on " << commandFile << std::endl << std::endl;
        stream << "Outputs:" << std::endl;
        stream << outputs << std::endl << std::endl;
        stream << "Additional Dependencies:" << std::endl;
        stream << inputs << std::endl << std::endl;
    }

}

#endif /* INCLUDED_DEPENDENCYPRINTER_H_8050457 */
