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

#ifndef INCLUDED_COMMANDPROCESSOR_H_7221186
#define INCLUDED_COMMANDPROCESSOR_H_7221186

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

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

#endif /* INCLUDED_COMMANDPROCESSOR_H_7221186 */
