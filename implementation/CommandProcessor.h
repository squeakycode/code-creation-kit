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

#include <string>
#include "CCommandLineParser.gen.h"
#include <boost/foreach.hpp>
#include <stdexcept>
#include "CSourceFile.h"
#include "FileSystem.h"
#include "DependencyPrinter.h"
#include "../version/version.gen.h"
#include "CGeneratorCommandProcessor.h"
#include "StringLiteral.h"

class CommandFileT;

namespace code_creation_kit
{
    ///processes user commands
    namespace CommandProcessor
    {
        class ExInvalidCommandLineOptions : public std::runtime_error 
        { public: ExInvalidCommandLineOptions() : std::runtime_error( "Invalid command line options. Please use --help to get the option description.") {}};

        ///process the command line from the console
        template <typename CharT, typename GeneratorT, typename GeneratorStatisticT, typename LogFileT>
        void processCommandLine( int argc, CharT* argv[], GeneratorT& generator, GeneratorStatisticT& generatorStatistic, LogFileT& logFile, bool* prompt = 0, bool *logging = 0)
        {
            typedef std::basic_string<CharT, std::char_traits<CharT> > StringT;

            CCommandLineParser<StringT> parser;
            CGeneratorCommandProcessor<StringT> generatorCommandProcessor;
            StringT commandFileAbsolutePath;

            parser.parse( argc, argv);

            try
            {
                const typename CCommandLineParser<StringT>::ECommand command = parser.getCommand();
                if ( CCommandLineParser<StringT>::eHelp == command )
                {
                    printVersionInfo();
                    parser.printDescription();
                    generatorCommandProcessor.printDescription();
                }
                else if ( CCommandLineParser<StringT>::eExecuteCommand == command )
                {
                    std::vector<StringT> commands = parser.getCommands();
                    BOOST_FOREACH( const StringT& generatorCommand, commands)
                    {
                        generatorCommandProcessor.processCommand(generatorCommand, generator, StringT(), logFile);
                    }
                }
                else if ( CCommandLineParser<StringT>::eExecuteCommandFile == command )
                {
                    //check if wait and retry is switched on
                    if ( prompt )
                    {
                        *prompt = parser.hasPrompt();
                    }
                    if ( logging && parser.hasPrompt())
                    {
                        // logging turned on?
                        if ( *logging)
                        {
                            //activate logging via cerr
                            generator.connectLogOutputStream( &FileSystem::getCerr<CharT>());
                        }
                        *logging = true; //return true, logging option can be shown
                    }

                    std::vector<StringT> commandFiles = parser.getCommandFiles();
                    BOOST_FOREACH( const StringT& commandFileName, commandFiles)
                    {
                        //output names of the files processed to show what is processed
                        if ( prompt && parser.hasPrompt())
                        {
                            FileSystem::getCerr<CharT>() << "Processing command file:" << std::endl;
                            FileSystem::getCerr<CharT>() << commandFileName << std::endl;
                        }

                        //reset the generator for every command file
                        generator.reset();

                        //process the command file
                        bool useCin = commandFileName == STRING_LITERAL("-");
                        commandFileAbsolutePath = useCin ? commandFileName : FileSystem::determineDependentLocation( commandFileName);
                        CSourceFile<StringT,CommandFileT> commandFile( commandFileAbsolutePath, useCin);
                        generatorCommandProcessor.processCommandStream( commandFile.get(), generator, commandFileAbsolutePath, logFile);
                        commandFile.checkEofReached();
                    }
                }
                else if ( CCommandLineParser<StringT>::eCommandFileDependencies == command )
                {
                    //check if wait and retry is switched on
                    if ( prompt )
                    {
                        *prompt = parser.hasPrompt();
                    }
                    if ( logging && parser.hasPrompt())
                    {
                        *logging = false; //return false, logging option can not be shown
                    }

                    std::vector<StringT> commandFiles = parser.getCommandFiles();
                    BOOST_FOREACH( const StringT& commandFileName, commandFiles)
                    {
                        //reset the generator for every command file
                        generatorStatistic.reset();

                        //process the command file
                        bool useCin = commandFileName == STRING_LITERAL("-");
                        commandFileAbsolutePath = useCin ? commandFileName : FileSystem::determineDependentLocation( commandFileName);
                        CSourceFile<StringT,CommandFileT> commandFile( commandFileAbsolutePath, useCin);
                        generatorCommandProcessor.processCommandStream( commandFile.get(), generatorStatistic, commandFileAbsolutePath, logFile, true);
                        commandFile.checkEofReached();

                        if ( parser.getOutputDependenciesStyle() == STRING_LITERAL("mpc"))
                        {
                            DependencyPrinter::printMpc( generatorStatistic.getTableFiles(), generatorStatistic.getGeneratedFiles(), generatorStatistic.getTemplateFiles(), commandFileAbsolutePath, FileSystem::getCout<CharT>());
                        }
                        else if ( parser.getOutputDependenciesStyle() == STRING_LITERAL("vs"))
                        {
                            DependencyPrinter::printVs( generatorStatistic.getTableFiles(), generatorStatistic.getGeneratedFiles(), generatorStatistic.getTemplateFiles(), commandFileAbsolutePath, FileSystem::getCout<CharT>());
                        }
                        else
                        {
                            throw ExInvalidCommandLineOptions();
                        }
                    }
                }
                else
                {
                    throw ExInvalidCommandLineOptions();
                }
            }
            catch( CSourceFileExceptions<CommandFileT>::ExCannotOpenFile&)
            {
                FileSystem::getCerr<CharT>() << "Failed to open command file: " << commandFileAbsolutePath << std::endl;
                throw CErrorPrinted(); //empty class provided externally
            }
            catch( CSourceFileExceptions<CommandFileT>::ExCannotReadFile&)
            {
                FileSystem::getCerr<CharT>() << "Failed to open command file: " << commandFileAbsolutePath << std::endl;
                throw CErrorPrinted(); //empty class provided externally
            }
        }
    }
}
