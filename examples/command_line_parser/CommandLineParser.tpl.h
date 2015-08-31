#ifndef INCLUDED_COMMANDLINEPARSER_TPL_H_3787958
#define INCLUDED_COMMANDLINEPARSER_TPL_H_3787958

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>

///provides simple command line parsing support
class CommandLineParser
{
public:
    ///create the parser
    CommandLineParser()
    {
        reset();
    }
    
    ///reset the parsed data
    void reset()
    {
        m_[ENTRY]["Option Name Identifier"]Passed = false;

        m_[ENTRY]["Option Name Identifier"]Value = [ENTRY]["Default Value"][IF][ENTRY]["Data Type"];
    }
    
    ///prints help
    void printHelp()
    {
        std::cerr.fill(' ');
        [MACRO_BEGIN][TRIM]
        std::cerr.width(4);
        [BEGIN][TRIM]
        std::cerr << "-[ENTRY]["Short Option Name"]" << " [ --[ENTRY]["Option Name"] ]" << std::endl;
        [OR][TRIM]
        std::cerr << "-[ENTRY]["Short Option Name"]" << std::endl;
        [OR][TRIM]
        std::cerr << "--" << "[ENTRY]["Option Name"]" << std::endl;
        [END][TRIM]
        [COMMENT]//                         [tabs to spaces      ][block format->   |buffer end?   |string ends \n |string with ' '| text no ' '| line start spaces for formatting       insert match][remove last new line     ][to cstring][replace \n for formatting generated code                  ]
        std::cerr << "[ENTRY]["Description"][REPLACE]["\t","    "][REGEX_REPLACE]['([^\n]{1,39})\''|([^\n]{0,39})\n|([^\n]{0,39}) +|([^\n]{39})','                                        $1$2$3$4\n'][REGEX_REPLACE]['\n\''',''][TO_CSTRING][REPLACE]["\\n","\" << std::endl;\n        std::cerr << \""]" << std::endl;
        
        [MACRO_END][TRIM]
    }

    ///parse the program options, use getter methods to get the values
    void parse( int argc, char* argv[])
    {
        reset();

        for ( int i = 1; i < argc; ++i)
        {
            //get current argument
            const char* arg = argv[ i ];
            [MACRO_BEGIN][TRIM]
            [BEGIN][IF][FIRST_TIME][OR]else [END]if ( [BEGIN]isEqual( arg, "--[ENTRY]["Option Name"]") || isEqual( arg, "-[ENTRY]["Short Option Name"]")[OR]isEqual( arg, "--[ENTRY]["Option Name"]")[OR]isEqual( arg, "-[ENTRY]["Short Option Name"]")[END])
            {
                m_[ENTRY]["Option Name Identifier"]Passed = true;
                [BEGIN][IF][ENTRY]["Data Type"][TRIM]
                const char* value = argv[ ++i ];
                if ( i >= argc || isOption( value))
                {
                    throw std::runtime_error( std::string("Option '") + arg + "' requires a value.");
                }
                if ( !convertTo( m_[ENTRY]["Option Name Identifier"]Value, value))
                {
                    throw std::runtime_error( std::string("Error parsing value '") + value + "' of option '" + arg + "'.");
                }
                [OR][END][TRIM]
            }
            [MACRO_END][TRIM]
            else
            {
                throw std::runtime_error( std::string("Error unknown program option '") + arg + "'." );
            }
        }
    }

    [MACRO_BEGIN][TRIM]
    ///indicates that the option '[ENTRY]["Option Name Identifier"]' has been provided
    bool [ENTRY]["Option Name Identifier"]Passed() const
    {
        return m_[ENTRY]["Option Name Identifier"]Passed;
    }

    [MACRO_END][TRIM]

    [MACRO_BEGIN][TRIM]
    ///returns the value passed for option '[ENTRY]["Option Name Identifier"]'
    const [ENTRY]["Data Type"]& [ENTRY]["Option Name Identifier"]Value() const
    {
        return m_[ENTRY]["Option Name Identifier"]Value;
    }

    [MACRO_END][TRIM]
private:
    ///helper function for converting parameters
    bool convertTo( std::string& value, const char* arg)
    {
        value = arg;
        return true;
    }

    ///helper function for converting parameters
    template <typename T>
    bool convertTo( T& value, const char* arg)
    {
        std::stringstream s;
        s << arg;
        s >> value;
        return s.eof();
    }

    ///helper function for comparing strings
    bool isEqual( const char* a, const char* b)
    {
#ifdef WIN32
        return _stricmp( a, b) == 0;
#else
        return stricmp( a, b) == 0;
#endif
    }

    ///can be used to check whether a string is a valid option
    bool isOption( const char* arg)
    {
        if( isEqual( arg, "-[ENTRY]["Short Option Name"][READ_TOP_DOWN]")) return true;
        if( isEqual( arg, "--[ENTRY]["Option Name"][READ_TOP_DOWN]")) return true;
        return false;
    }

    bool m_[ENTRY]["Option Name Identifier"]Passed;

    [ENTRY]["Data Type"] m_[ENTRY]["Option Name Identifier"]Value;
};

#endif /* INCLUDED_COMMANDLINEPARSER_TPL_H_3787958 */
