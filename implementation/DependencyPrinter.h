// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <string.h>
#include "FileSystem.h"

namespace code_creation_kit
{
    namespace DependencyPrinter
    {
        template <typename ContainerT, typename StringT>
        void concatenate( const ContainerT& container, typename StringT::value_type quote, typename StringT::value_type delimiter, StringT& result)
        {
            for (const typename ContainerT::value_type& item : container)
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
            for (const typename ContainerT::value_type& item : container)
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
}
