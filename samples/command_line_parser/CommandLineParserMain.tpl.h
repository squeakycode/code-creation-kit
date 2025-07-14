#include "CommandLineParser.gen.h"

///main
int main(int argc, char* argv[])
{
    try
    {
        CommandLineParser parser;
        parser.parse( argc, argv);

        parser.printHelp();

        [MACRO_BEGIN][TRIM]
        if ( parser.[ENTRY]["Option Name Identifier"]Passed())
        {
            std::cout << "Option [BEGIN][ENTRY]["Option Name"][TO_CSTRING][OR][ENTRY]["Short Option Name"][TO_CSTRING][END] passed."
            [BEGIN][IF][ENTRY]["Data Type"][TRIM]
                      << " Value = " << parser.[ENTRY]["Option Name Identifier"]Value()
            [OR][END][TRIM]
                      << std::endl;
        }

        [MACRO_END][TRIM]
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