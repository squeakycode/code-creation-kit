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

#include <string>
#include <iostream>
#ifdef WIN32
#   include <conio.h>
#endif
#include "CGenerator.h"
#include "CGeneratorStatistic.h"
#include "CErrorPrinter.gen.h"
#include "CommandProcessor.h"
#include "CTargetFile.h"

using namespace code_creation_kit;

class LogFileT;

//run processing of command line
int process( int argc, char* argv[], bool& prompt, bool& logging)
{
    typedef char CharT;
    typedef std::basic_string<CharT, std::char_traits<CharT> > StringT;
    typedef std::basic_ostream< CharT, std::char_traits<CharT> > LogOutputStreamT;

    try
    {
        //create log file
        CTargetFile<StringT, LogFileT> logFile;

        //create generator
        CGenerator<StringT, LogOutputStreamT> generatorImpl;
        CErrorPrinter<StringT, CGenerator<StringT, LogOutputStreamT> > generator( generatorImpl);

        //create generator statistic
        CGeneratorStatistic<StringT> generatorStatisticImpl;
        CErrorPrinter<StringT, CGeneratorStatistic<StringT> > generatorStatistic( generatorStatisticImpl);

        //execute command, which generator is used depends on the command
        CommandProcessor::processCommandLine( argc, argv, generator, generatorStatistic, logFile, &prompt, &logging);
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
    bool logging = false;

    //loop for prompt mode
    for(;;)
    {
        //run processing
        int exitCode = process( argc, argv, prompt, logging);

        //if prompt mode
        if ( prompt )
        {
            char keyPressed = ' ';
            bool success = exitCode == 0;
            //output status
            std::cerr << std::endl << ( success ? "Command file successfully processed..." : "An error occured while processing...") << std::endl;
#ifdef WIN32
            //output help
            std::cerr << "Press 'r' to rerun the processing" << (logging ? " or 'l' to rerun with logging" : "") << ", any other key to exit." << std::endl;
            //wait for key press
            keyPressed = (char)_getch();
#else
            //output help
            std::cerr << "Press 'r' and enter to rerun the processing" << (logging ? " or 'l' to rerun with logging" : "") << ", any other key and enter to exit." << std::endl;
            //wait for input
            keyPressed = (char)getchar();
#endif
            logging = logging && (keyPressed == 'l' || keyPressed == 'L');
            if ( keyPressed == 'r' || keyPressed == 'R' || logging)
            {
                //try again, maybe after changing the input files
                continue;
            }
        }
        return exitCode;
    }
}

