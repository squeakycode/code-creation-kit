[COMMENT]  Copyright (c) 2011-2023 Andreas Gau
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

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cwchar>
#include <climits>
#include "StringLiteral.h"
#include "3rdparty/boost/split_winmain.h"

//parses the command line, provides the parameters from the command line, and checks for valid option combinations
template <typename StringT = std::string>
class [ENTRY]["Parser Name"]
{
    typedef typename StringT::value_type CharT;
    static const size_t cLeftColumnSize = 40;

public:
    ///lists valid option combinations
    enum ECommand
    {
        e[ENTRY]["Program Options"][IF][ENTRY]["Combination"],
        eNoOptionsGiven,
        eOptionsInvalid
    };

    [ENTRY]["Parser Name"]()
    {
        reset();
    }

    ///reset the parsed data
    void reset()
    {
        m_[ENTRY]["C++ Name"]Passed = false;

        [MACRO_BEGIN][TRIM]
        m_[ENTRY]["C++ Name"]Value = [ENTRY]["Default"][IF][ENTRY]["C++ Type"];
        [OR][TRIM]
        m_[ENTRY]["C++ Name"]Value.clear();[IF][ENTRY]["C++ Type"]
        [MACRO_END][TRIM]
    }

    void printDescription(std::ostream& stream)
    {
        stream << "[ENTRY]["Heading"][TO_CSTRING]" << std::endl;
        [MACRO_BEGIN][TRIM]
        [BEGIN][TRIM]
        stream << "[ENTRY]["Group Description"]" << ":" << std::endl;
        [OR][END][TRIM]
        [BEGIN][IF][ENTRY]["Shortcut"][TRIM]
        printHelpCommandText("  -[ENTRY]["Shortcut"] [ --[ENTRY]["Name"] ][BEGIN][IF][ENTRY]["Zero-Token"][OR] arg[END]", stream);
        [OR][TRIM]
        printHelpCommandText("  --[ENTRY]["Name"][BEGIN][IF][ENTRY]["Zero-Token"][OR] arg[END]", stream);
        [END][TRIM]
        stream << "[ENTRY]["Description"][BLOCK_FORMAT][39][PAD_LEFT][" ",0,+40][TO_CSTRING][REPLACE]["\\n","\" << std::endl;\n        stream << \""]" << std::endl;
        [MACRO_END][TRIM]
    }

    ///parses standard command line parameters
    void parse(int argc, CharT* args[])
    {
        std::vector<const CharT*> argv;
        argv.reserve(argc + 1);
        for (int i = 0; i < argc; ++i)
        {
            argv.push_back(args[i]);
        }
        argv.push_back(NULL);
        parse(argc, argv.data());
    }
    
    ///parses command line parameters
    void parse(int argc, const CharT* argv[])
    {
        reset();

        for (int i = 1; i < argc; ++i)
        {
            //get current argument
            bool argumentConsumed = false;
            const CharT* arg = argv[ i ];
            if (arg && arg[0] == '-')
            {
                [MACRO_BEGIN][TRIM]
                [BEGIN][IF][FIRST_TIME][OR]else [END]if ([BEGIN](arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("[ENTRY]["Name"]"))) || isEqual(arg + 1, STRING_LITERAL("[ENTRY]["Shortcut"]"))[OR](arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("[ENTRY]["Name"]")))[OR]isEqual(arg[1], STRING_LITERAL("[ENTRY]["Shortcut"]")))[END])
                {
                    argumentConsumed = true;
                    m_[ENTRY]["C++ Name"]Passed = true;
                    [BEGIN][IF][ENTRY]["C++ Type"][IF][ENTRY]["Multi-Token"][TRIM]
                    parseArgs(arg, argc, argv, ++i, m_[ENTRY]["C++ Name"]Value, 1, SIZE_MAX);
                    [OR][IF][ENTRY]["C++ Type"][IF][NOT][ENTRY]["Zero-Token"][TRIM]
                    parseArg(arg, argc, argv, ++i, m_[ENTRY]["C++ Name"]Value, false);
                    [OR][END][TRIM]
                }
                [MACRO_END][TRIM]
            }
            
            [MACRO_BEGIN][IF][ENTRY]["Positional"][TRIM]
            if (!argumentConsumed && !m_[ENTRY]["C++ Name"]Passed)
            {
                argumentConsumed = true;
                m_[ENTRY]["C++ Name"]Passed = true;
                [BEGIN][IF][ENTRY]["C++ Type"][IF][ENTRY]["Multi-Token"][TRIM]
                parseArgs("[BEGIN]--[ENTRY]["Name"][OR]-[ENTRY]["Shortcut"][END]", argc, argv, i, m_[ENTRY]["C++ Name"]Value, 1, static_cast<size_t>([ENTRY]["Positional Count"]));
                [OR][IF][ENTRY]["C++ Type"][IF][NOT][ENTRY]["Zero-Token"][TRIM]
                parseArg(STRING_LITERAL("[BEGIN]--[ENTRY]["Name"][OR]-[ENTRY]["Shortcut"][END]"), argc, argv, i, m_[ENTRY]["C++ Name"]Value, false);
                [OR][END][TRIM]                
            }
            [MACRO_END][TRIM]
            if (!argumentConsumed)
            {
                throw std::runtime_error( std::string("Error unknown program option '") + arg + "'." );
            }
        }
    }

    void parse(const StringT& commandLine)
    {
        std::vector<StringT> args = boost::program_options::split_winmain(commandLine);
        std::vector<const CharT*> argv;
        argv.reserve(args.size() + 2);
        argv.push_back("");
        for (const StringT& arg : args)
        {
            argv.push_back(arg.c_str());
        }
        argv.push_back(NULL);
        parse(static_cast<int>(args.size() + 1), argv.data());
    }

    ///determines the command by checking the combination of parameters provided
    ECommand getCommand() const
    {
        [MACRO_BEGIN][TRIM]
        if (
            [BEGIN.][IF.][FIRST_TIME.]   [OR.]&& [END.]m_[ENTRY.]["C++ Name"]Passed[BEGIN.] == true[IF.][ENTRY.]["[ENTRY]["Program Options"]"][EQUALS.]["yes"][OR.] == false[IF.][ENTRY.]["[ENTRY]["Program Options"]"][EQUALS.]["no"][END.]
        )
        {
            return e[ENTRY]["Program Options"][IF][ENTRY]["Combination"];
        }
        
        [MACRO_END][TRIM]
        
        if (
            [BEGIN][IF][FIRST_TIME]   [OR]&& [END]!m_[ENTRY]["C++ Name"]Passed
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }
    
    [MACRO_BEGIN][TRIM]
    [ENTRY]["C++ Type"] get[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Value;
    }

    [OR][TRIM]
    bool get[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Passed;
    }
    
    [MACRO_END][TRIM]
    
    [MACRO_BEGIN][TRIM]
    bool has[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Passed;
    }
    
    [MACRO_END][TRIM]

    
private:    
    bool isOption(const CharT* arg)
    {
        bool result = false;
        if (arg && arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                if(
                    [BEGIN][IF][FIRST_TIME]  [OR]||[END] isEqual(arg + 2, STRING_LITERAL("[ENTRY]["Name"]"))
                )
                {
                    result = true;
                }
                
            }
            else if(
                [BEGIN][IF][FIRST_TIME]  [OR]||[END] isEqual(arg + 1, STRING_LITERAL("[ENTRY]["Shortcut"]"))
            )
            {
                result = true;
            }
        }
        return result;
    }
    
    bool isEqual(const char* a, const char* b)
    {
        bool result = (strcmp(a,b) == 0);
        return result;
    }

    bool isEqual(const wchar_t* a, const wchar_t* b)
    {
        bool result = (wcscmp(a,b) == 0);
        return result;
    }

    void printHelpCommandText(const char* text, std::ostream& stream)
    {
        size_t textSize = strlen(text);
        if (textSize > (cLeftColumnSize - 1) )
        {
            stream << text << std::endl;
            stream << std::string(cLeftColumnSize, ' ');
        }
        else
        {
            stream << text << std::string(cLeftColumnSize - textSize, ' ');
        }
    }

    template <typename T>
    bool parseArg(const CharT* option, int argc, const CharT** argv, int& index, T& parsedValue, bool valueOptional)
    {
        const char* argValue = argv[ index ];
        
        if ( index >= argc || !argValue || isOption(argValue))
        {
            if (valueOptional)
            {
                return false;
            }
            else
            {
                throw std::runtime_error( std::string("Option '") + option + "' requires a value.");
            }
        }
        if ( !convertTo( parsedValue, argValue))
        {
            throw std::runtime_error( std::string("Error parsing value '") + argValue + "' of option '" + option + "'.");
        }
        return true;
    }

    template <typename T>
    void parseArgs(const CharT* option, int argc, const CharT** argv, int& index, T& container, size_t minCount, size_t maxCount)
    {
        size_t argsParsed = 0;
        for (; argsParsed <= maxCount && index < argc; ++argsParsed, ++index)
        {
            typename T::value_type parsedValue;
            if (!parseArg(option, argc, argv, index, parsedValue, true))
            {
                --index;
                break;
            }
            container.push_back(parsedValue);
        }
        if (argsParsed < minCount)
        {
            throw std::runtime_error( std::string("Option '") + option + "' requires more values.");
        }
    }

    bool convertTo( std::string& value, const char* arg)
    {
        value = arg;
        return true;
    }
    
    bool convertTo( std::wstring& value, const wchar_t* arg)
    {
        value = arg;
        return true;
    }

    //helper function for converting parameters
    template <typename T>
    bool convertTo( T& value, const CharT* arg)
    {
        std::stringstream s;
        s << arg;
        s >> value;
        return s.eof();
    }

private:    
    bool m_[ENTRY]["C++ Name"]Passed;

    [ENTRY]["C++ Type"] m_[ENTRY]["C++ Name"]Value;
};
