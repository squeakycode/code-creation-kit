[COMMENT]  Copyright (c) 2011-2025 Andreas Gau
[COMMENT]  SPDX-License-Identifier: BSD-3-Clause
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

//parses the command line, provides the parameters from the command line, and checks for valid option combinations
template <typename StringT = std::string>
class [ENTRY]["Parser Name"]
{
    typedef typename StringT::value_type CharT;
    constexpr static size_t cLeftColumnSize = 40;

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
            const CharT* pArgument = argv[ i ];
            if (pArgument && pArgument[0] == '-')
            {
                [MACRO_BEGIN][TRIM]
                [BEGIN][IF][FIRST_TIME][OR]else [END]if ([BEGIN](pArgument[1] == '-' && isEqual(pArgument + 2, "[ENTRY]["Name"]")) || isEqual(pArgument + 1, "[ENTRY]["Shortcut"]")[OR](pArgument[1] == '-' && isEqual(pArgument + 2, "[ENTRY]["Name"]"))[OR]isEqual(pArgument[1], "[ENTRY]["Shortcut"]"))[END])
                {
                    argumentConsumed = true;
                    m_[ENTRY]["C++ Name"]Passed = true;
                    [BEGIN][IF][ENTRY]["C++ Type"][IF][ENTRY]["Multi-Token"][TRIM]
                    parseArgs(pArgument, argc, argv, ++i, m_[ENTRY]["C++ Name"]Value, 1, SIZE_MAX);
                    [OR][IF][ENTRY]["C++ Type"][IF][NOT][ENTRY]["Zero-Token"][TRIM]
                    parseArg(pArgument, argc, argv, ++i, m_[ENTRY]["C++ Name"]Value, false);
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
                parseArg("[BEGIN]--[ENTRY]["Name"][OR]-[ENTRY]["Shortcut"][END]", argc, argv, i, m_[ENTRY]["C++ Name"]Value, false);
                [OR][END][TRIM]                
            }
            [MACRO_END][TRIM]
            if (!argumentConsumed)
            {
                throw std::runtime_error( std::string("Error unknown program option '") + toStdString(pArgument) + "'." );
            }
        }
    }

    std::vector<StringT> splitCommandLine(const StringT& input)
    {
        typedef typename StringT::value_type CharT;
        std::vector<StringT> arguments;
        StringT currentArgument;
        bool inQuotes = false;
        bool seenQuotes = false;

        for (auto it = input.cbegin() ; it != input.cend(); ++it)
        {
            const CharT c = *it;

            if (c == '\\')
            {
                // escaping is only relevant inside of quotes
                if (inQuotes && (it + 1) != input.cend())
                {
                    const CharT nextC = *(it + 1);
                    if (nextC == '"' || nextC == '\\')
                    {
                        currentArgument += nextC;
                        ++it;
                    }
                    else
                    {
                        currentArgument += c;
                    }
                }
                else
                {
                    currentArgument += c;
                }
            }
            else if (c == '"')
            {
                inQuotes = !inQuotes;
                seenQuotes = true;
            }
            else if ((c == ' ' || c == '\t') && !inQuotes)
            {
                if (!currentArgument.empty() || seenQuotes)
                {
                    arguments.push_back(currentArgument);
                    currentArgument.clear();
                    seenQuotes = false;
                }
            }
            else
            {
                currentArgument += c;
            }
        }
        if (!currentArgument.empty())
        {
            arguments.push_back(currentArgument);
        }
        return arguments;
    }

    void parse(const StringT& commandLine)
    {
        std::vector<StringT> arguments = splitCommandLine(commandLine);
        std::vector<const CharT*> argv;
        argv.reserve(arguments.size() + 2);
        argv.emplace_back();
        for (const StringT& argument : arguments)
        {
            argv.push_back(argument.c_str());
        }
        argv.push_back(NULL);
        parse(static_cast<int>(arguments.size() + 1), argv.data());
    }

    ///determines the command by checking the combination of parameters provided
    [[nodiscard]] ECommand getCommand() const
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
    [[nodiscard]] [ENTRY]["C++ Type"] get[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Value;
    }

    [OR][TRIM]
    [[nodiscard]] bool get[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Passed;
    }
    
    [MACRO_END][TRIM]
    
    [MACRO_BEGIN][TRIM]
    [[nodiscard]] bool has[ENTRY]["C++ Name"]() const
    {
        return m_[ENTRY]["C++ Name"]Passed;
    }
    
    [MACRO_END][TRIM]

private:
    template <typename LocalCharT>
    static bool isOption(const LocalCharT* pArgument)
    {
        bool result = false;
        if (pArgument && pArgument[0] == '-')
        {
            if (pArgument[1] == '-')
            {
                if(
                    [BEGIN][IF][FIRST_TIME]  [OR]||[END] isEqual(pArgument + 2, "[ENTRY]["Name"]")
                )
                {
                    result = true;
                }
                
            }
            else if(
                [BEGIN][IF][FIRST_TIME]  [OR]||[END] isEqual(pArgument + 1, "[ENTRY]["Shortcut"]")
            )
            {
                result = true;
            }
        }
        return result;
    }
    
    // command line parameters are expected to be ASCII encoded.
    template <typename LocalCharTA, typename LocalCharTB>
    static bool isEqual(const LocalCharTA* pA, const LocalCharTB* pB)
    {
        if (pA != nullptr && pB != nullptr)
        {
            for (;*pA != 0 && *pB != 0; ++pA, ++pB)
            {
                if (*pA != *pB)
                {
                    return false;
                }
            }
            return (*pA == *pB);
        }
        return false;
    }

    static void printHelpCommandText(const char* text, std::ostream& stream)
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

    template <typename LocalCharTA, typename LocalCharTB, typename T>
    bool parseArg(const LocalCharTA* pOption, int argc, const LocalCharTB** argv, int& index, T& parsedValue, bool valueOptional)
    {
        const LocalCharTB* argValue = argv[ index ];
        
        if ( index >= argc || !argValue || isOption(argValue))
        {
            if (valueOptional)
            {
                return false;
            }
            else
            {
                throw std::runtime_error( std::string("Option '") + toStdString(pOption) + "' requires a value.");
            }
        }
        if ( !convertTo( parsedValue, argValue))
        {
            throw std::runtime_error( std::string("Error parsing value '") + toStdString(argValue) + "' of option '" + toStdString(pOption) + "'.");
        }
        return true;
    }

    template <typename LocalCharTA, typename LocalCharTB, typename T>
    void parseArgs(const LocalCharTA* pOption, int argc, const LocalCharTB** argv, int& index, T& container, size_t minCount, size_t maxCount)
    {
        size_t argsParsed = 0;
        for (; argsParsed <= maxCount && index < argc; ++argsParsed, ++index)
        {
            typename T::value_type parsedValue;
            if (!parseArg(pOption, argc, argv, index, parsedValue, true))
            {
                --index;
                break;
            }
            container.push_back(parsedValue);
        }
        if (argsParsed < minCount)
        {
            throw std::runtime_error( std::string("Option '") + toStdString(pOption) + "' requires more values.");
        }
    }

    //helper function for converting parameters
    static bool convertTo( std::string& value, const char* pArgument)
    {
        value = pArgument;
        return true;
    }

    //helper function for converting parameters
    static bool convertTo( std::wstring& value, const wchar_t* pArgument)
    {
        value = pArgument;
        return true;
    }

    //helper function for converting parameters
    template <typename T, typename LocalCharT>
    static bool convertTo( T& value, const LocalCharT* pArgument)
    {
        std::basic_stringstream<CharT, std::char_traits<CharT>, std::allocator<CharT>> s;
        s << pArgument;
        s >> value;
        return s.eof();
    }

    //helper function for converting parameters
    std::string toStdString( const wchar_t* pText)
    {
        std::string result;
        for(;pText && *pText;++pText)
        {
            //will only work well with ASCII encoded options
            result += static_cast<char>(*pText);
        }
        return result;
    }

    //helper function for converting parameters
    std::string toStdString( const char* pText)
    {
        std::string result(pText);
        return result;
    }
private:
    bool m_[ENTRY]["C++ Name"]Passed;

    [ENTRY]["C++ Type"] m_[ENTRY]["C++ Name"]Value;
};
