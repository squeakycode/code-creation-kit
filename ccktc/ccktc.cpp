//   Copyright (C) 2011 Andreas Gau
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
    
#include <string>
#include <iostream>
#ifdef WIN32
#	include <conio.h>
#endif
#include "CGenerator.h"
#include "CGeneratorStatistic.h"
#include "CErrorPrinter.gen.h"
#include "CommandProcessor.h"


//run processing of command line
int process( int argc, char* argv[], bool& prompt)
{
    typedef char CharT;
    typedef std::basic_string<CharT, std::char_traits<CharT> > StringT;

    try
    {
        //create generator
        CGenerator<StringT> generatorImpl;
        CErrorPrinter<StringT, CGenerator<StringT> > generator( generatorImpl);

        //create generator statistic
        CGeneratorStatistic<StringT> generatorStatisticImpl;
        CErrorPrinter<StringT, CGeneratorStatistic<StringT> > generatorStatistic( generatorStatisticImpl);

        //execute command, which generator is used depends on the command
        CommandProcessor::processCommandLine( argc, argv, generator, generatorStatistic, &prompt);
    }
    catch( CErrorPrinted&)
    {
        return EXIT_FAILURE;
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}

///main
int main(int argc, char* argv[])
{
    bool prompt = false;

    //loop for prompt mode
    for(;;)
    {
        //run processing
        int exitCode = process( argc, argv, prompt);

        //if prompt mode
        if ( prompt )
        {
            char keyPressed = ' ';
            bool success = exitCode == 0;
            //output status
            std::cout << std::endl << ( success ? "Command file successfully processed..." : "An error occured while processing...") << std::endl;
#ifdef WIN32
			//output help
            std::cout << "Press 'r' to rerun the processing, any other key to exit." << std::endl;
            //wait for key press
			keyPressed = (char)_getch();
#else
			//output help
            std::cout << "Press 'r' and enter to rerun the processing, any other key and enter to exit." << std::endl;
            //wait for input
			keyPressed = (char)getchar();
#endif
            if ( keyPressed == 'r' || keyPressed == 'R' )
            {
                //try again, maybe after changing the input files
                continue;
            }
        }
        return exitCode;
    }
}

