//  Copyright (c) 2011-2023 Andreas Gau
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
