[COMMENT]  Copyright (c) 2011-2015 Andreas Gau
[COMMENT]  All rights reserved.
[COMMENT]
[COMMENT]  Redistribution and use in source and binary forms, with or without
[COMMENT]  modification, are permitted provided that the following conditions are met:
[COMMENT]      * Redistributions of source code must retain the above copyright
[COMMENT]        notice, this list of conditions and the following disclaimer.
[COMMENT]      * Redistributions in binary form must reproduce the above copyright
[COMMENT]        notice, this list of conditions and the following disclaimer in the
[COMMENT]        documentation and/or other materials provided with the distribution.
[COMMENT]      * Neither the name of the copyright holder nor the
[COMMENT]        names of contributors may be used to endorse or promote products
[COMMENT]        derived from this software without specific prior written permission.
[COMMENT]
[COMMENT]  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
[COMMENT]  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
[COMMENT]  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
[COMMENT]  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
[COMMENT]  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
[COMMENT]  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
[COMMENT]  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
[COMMENT]  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
[COMMENT]  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
[COMMENT]  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
[COMMENT]
[COMMENT]
[COMMENT] Required Parameters:
[COMMENT] "Parser Name" - the name of the parser class, e.g. CCommandLineParser
[COMMENT] "Heading" - heading of option description, e.g. Allowed Options
//------------------------------------------------------------------------------
/**
\file
\brief  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
*/
//------------------------------------------------------------------------------

#pragma once

#include <iostream>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 4702 )
#endif
#include "boost/program_options.hpp"
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 )
#endif

///parses the command line, provides the parameters from the command line, and checks for valid option combinations
template <typename StringT = std::string>
class [ENTRY]["Parser Name"]
{
    typedef typename StringT::value_type CharT; 
    typedef boost::program_options::basic_command_line_parser<CharT> CommandLineParserT;

public:
    ///lists valid option combinations
    enum ECommand
    {
        e[ENTRY]["Program Options"][IF][ENTRY]["Combination"],
        eNoOptionsGiven,
        eOptionsInvalid
    };
    
    ///sets up the option description used by boost program options library
    [ENTRY]["Parser Name"]()
      : m_description("[ENTRY]["Heading"]")
      , m_description[ENTRY]["Group Name"]("[ENTRY]["Group Description"]")
    {
        // Declare the supported options.
        [MACRO_BEGIN][IF][ENTRY]["Group Description"][OR][IF][LAST_TIME]m_description.add_options()[MACRO_END]
        [MACRO_BEGIN][BEGIN][TRIM]
        ;
        m_description[ENTRY]["Group Name"].add_options() //("[ENTRY]["Group Description"]")
        [OR][END][TRIM]
            ("[ENTRY]["Name"][BEGIN],[ENTRY]["Shortcut"][OR][END]"[BEGIN], value<[ENTRY]["C++ Type"] >()[BEGIN]->zero_tokens()[IF][ENTRY]["Zero-Token"][OR]->multitoken()[IF][ENTRY]["Multi-Token"][OR][END][OR][END], "[ENTRY]["Description"]")
        [MACRO_END][TRIM]
            ;
        [MACRO_BEGIN][TRIM]
        [BEGIN][IF][FIRST_TIME][TRIM]
        // Add the positional descriptions
        [OR][END][TRIM]
        m_positionalDescription.add( "[ENTRY]["Name"]", [ENTRY]["Positional Count"]);[IF][ENTRY]["Positional"]
        [MACRO_END][TRIM]

        [MACRO_BEGIN][TRIM]
        [BEGIN][IF][FIRST_TIME][TRIM]
        // Connect descriptions
        [OR][END][TRIM]
        m_description.add( m_description[ENTRY]["Group Name"]);
        [MACRO_END][TRIM]
    }

    ///parses standard command line parameters
    void parse( int ac, CharT* av[])
    {
        m_vmap = boost::program_options::variables_map();
        boost::program_options::store( 
            CommandLineParserT(ac, av).options(m_description).positional(m_positionalDescription).run()
            , m_vmap
        );
    }
    
    ///parses command line parameters provided as single text string
    void parse( const StringT& commandLine)
    {
        m_vmap = boost::program_options::variables_map();
#ifdef WIN32
        std::vector<StringT> args = boost::program_options::split_winmain( commandLine);
#else
        std::vector<StringT> args = boost::program_options::split_unix( commandLine);
#endif
        boost::program_options::store( 
            CommandLineParserT(args).options(m_description).positional(m_positionalDescription).run()
            , m_vmap
        );
    }
    
    ///prints the option description to cout
    void printDescription() const
    {
        std::cout << m_description << std::endl;
    }
    
    ///determines the command by checking the combination of parameters provided
    ECommand getCommand() const
    {
        bool provided[ENTRY]["C++ Name"] = has[ENTRY]["C++ Name"]();
    
        [MACRO_BEGIN][TRIM]
        if (
            [BEGIN.][IF.][FIRST_TIME.]   [OR.]&& [END.]provided[ENTRY.]["C++ Name"][BEGIN.] == true[IF.][ENTRY.]["[ENTRY]["Program Options"]"][EQUALS.]["yes"][OR.] == false[IF.][ENTRY.]["[ENTRY]["Program Options"]"][EQUALS.]["no"][END.]
        )
        {
            return e[ENTRY]["Program Options"][IF][ENTRY]["Combination"];
        }
        
        [MACRO_END][TRIM]
        
        if (
            [BEGIN][IF][FIRST_TIME]   [OR]&& [END]!provided[ENTRY]["C++ Name"]
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }

    [MACRO_BEGIN][TRIM]
    ///returns the provided value[BEGIN] or [ENTRY]["Default"] as default[OR][END]
    [ENTRY]["C++ Type"] get[ENTRY]["C++ Name"]() const
    {
        [BEGIN][TRIM]
        if ( has[ENTRY]["C++ Name"]())
        {
            return m_vmap["[ENTRY]["Name"]"].as<[ENTRY]["C++ Type"] >();
        }
        return [ENTRY]["Default"];
        [OR][TRIM]
        return m_vmap["[ENTRY]["Name"]"].as<[ENTRY]["C++ Type"] >();
        [END][TRIM]
    }
    
    [MACRO_END][TRIM]

    [MACRO_BEGIN][TRIM]
    ///indicates that the option [ENTRY]["Name"] has been provided
    bool has[ENTRY]["C++ Name"]() const
    {
        return m_vmap.count( "[ENTRY]["Name"]") != 0;
    }
    
    [MACRO_END][TRIM]
    
private:
    ///assignment not supported
    void operator=( const CCommandLineParser<StringT>&);
    
    ///creates the right value object depending on the character type
    template<class T>
    boost::program_options::typed_value<T, CharT>*
    value()
    {
        boost::program_options::typed_value<T, CharT>* r = new boost::program_options::typed_value<T, CharT>(0);
        return r;        
    }

    boost::program_options::variables_map m_vmap; ///<map of the provided values
    boost::program_options::options_description m_description; ///<the option description
    boost::program_options::options_description m_description[ENTRY]["Group Name"]; ///<the option description of group: [ENTRY]["Group Description"]
    boost::program_options::positional_options_description m_positionalDescription; ///<description of positional options 
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

