#pragma once

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
        printBlock( 39, 40, "[ENTRY]["Description"][TO_CSTRING]");
        
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
            [BEGIN][IF][FIRST_TIME][OR]else [END]if ( isEqual( arg, "--[ENTRY]["Option Name"]")[BEGIN] || isEqual( arg, "-[ENTRY]["Short Option Name"]")[OR][END])
            {
                m_[ENTRY]["Option Name Identifier"]Passed = true;
                [BEGIN][IF][ENTRY]["Data Type"][TRIM]
                ++i;
                if ( i >= argc )
                {
                    throw std::runtime_error( "Option '[ENTRY]["Option Name"]' requires a value.");
                }
                convertTo( m_[ENTRY]["Option Name Identifier"]Value, argv[ i ]);
                [OR][END][TRIM]
            }
            [MACRO_END][TRIM]
            else
            {
                throw std::runtime_error( "Error parsing program options.");
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
    void convertTo( std::string& value, const char* arg)
    {
        value = arg;
    }

    ///helper function for converting parameters
    template <typename T>
    void convertTo( T& value, const char* arg)
    {
        std::stringstream s;
        s << arg;
        s >> value;

        if ( !s.str().empty())
        {
            throw std::runtime_error( "Error parsing program option value.");
        }
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

    ///helper function for printing a formatted block of text
    void printBlock( unsigned int offset, unsigned int width, const char* text)
    {
        for( unsigned int i = 0, start = 0, lastSpace = 0; text[ i ]; ++i)
        {
            if ( text[ i ] == ' ' )
            {
                lastSpace = i;
            }
            else if ( text[ i ] == '\n')
            {
                std::cerr << std::string(offset, ' ');
                std::cerr << std::string(&text[ start], &text[ i ]) << std::endl;
                start = i + 1;
                lastSpace = 0;
            }
            else if ( text[ i + 1 ] == '\0')
            {
                std::cerr << std::string(offset, ' ');
                std::cerr << &text[ start] << std::endl;
            }
            else if ( (i - start + 1) >= width )
            {
                std::cerr << std::string(offset, ' ');
                std::cerr << std::string(&text[ start], &text[ lastSpace ? lastSpace : i + 1]) << std::endl;
                start = (lastSpace ? lastSpace : i) + 1;
                lastSpace = 0;
            }
        }
    }

    bool m_[ENTRY]["Option Name Identifier"]Passed;

    [ENTRY]["Data Type"] m_[ENTRY]["Option Name Identifier"]Value;
};
