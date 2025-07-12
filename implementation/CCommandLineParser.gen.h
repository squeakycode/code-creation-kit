// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

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
class CCommandLineParser
{
    typedef typename StringT::value_type CharT;
    static const size_t cLeftColumnSize = 40;

public:
    ///lists valid option combinations
    enum ECommand
    {
        eHelp,
        eExecuteCommand,
        eExecuteCommandFile,
        eCommandFileDependencies,
        eProcessInstantTemplate,
        eNoOptionsGiven,
        eOptionsInvalid
    };

    CCommandLineParser()
    {
        reset();
    }

    ///reset the parsed data
    void reset()
    {
        m_HelpPassed = false;
        m_CommandsPassed = false;
        m_CommandFilesPassed = false;
        m_InstantTemplateFilesPassed = false;
        m_OutputDependenciesStylePassed = false;
        m_PromptPassed = false;

        m_CommandsValue.clear();
        m_CommandFilesValue.clear();
        m_InstantTemplateFilesValue.clear();
        m_OutputDependenciesStyleValue.clear();
    }

    void printDescription(std::ostream& stream)
    {
        stream << "Command Line Options" << std::endl;
        stream << "Help" << ":" << std::endl;
        printHelpCommandText("  -h [ --help ]", stream);
        stream << "Print help message" << std::endl;
        stream << "Command" << ":" << std::endl;
        printHelpCommandText("  -c [ --command ] arg", stream);
        stream << "Execute a command. This option can be" << std::endl;
        stream << "                                        specified multiple times. The commands" << std::endl;
        stream << "                                        are executed in the order they are" << std::endl;
        stream << "                                        specified." << std::endl;
        stream << "Command File" << ":" << std::endl;
        printHelpCommandText("  -f [ --command-file ] arg", stream);
        stream << "Execute a command file. This option can" << std::endl;
        stream << "                                        be specified multiple times. The" << std::endl;
        stream << "                                        command files are executed in the order" << std::endl;
        stream << "                                        they are specified. Default when the" << std::endl;
        stream << "                                        option name is omitted." << std::endl;
        printHelpCommandText("  -t [ --process-instant-template ] arg", stream);
        stream << "Process an instant template file" << std::endl;
        stream << "                                        (extension itpl). An instant template" << std::endl;
        stream << "                                        file contains or loads all needed" << std::endl;
        stream << "                                        tables. The name of the output file is" << std::endl;
        stream << "                                        build by removing the extension. This" << std::endl;
        stream << "                                        option can be specified multiple times." << std::endl;
        stream << "                                        The commands are executed in the order" << std::endl;
        stream << "                                        they are specified." << std::endl;
        printHelpCommandText("  -d [ --output-dependencies ] arg", stream);
        stream << "Output  command files dependencies" << std::endl;
        stream << "                                        instead of processing. Available styles" << std::endl;
        stream << "                                        are mpc (Meta Project Creator) or vs" << std::endl;
        stream << "                                        (Visual Studio)." << std::endl;
        printHelpCommandText("  -p [ --prompt ]", stream);
        stream << "Wait after processing or dependency" << std::endl;
        stream << "                                        output on a key press. Prompt for" << std::endl;
        stream << "                                        rerunning the command file(s)." << std::endl;
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
                if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("help"))) || isEqual(arg + 1, STRING_LITERAL("h")))
                {
                    argumentConsumed = true;
                    m_HelpPassed = true;
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("command"))) || isEqual(arg + 1, STRING_LITERAL("c")))
                {
                    argumentConsumed = true;
                    m_CommandsPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_CommandsValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("command-file"))) || isEqual(arg + 1, STRING_LITERAL("f")))
                {
                    argumentConsumed = true;
                    m_CommandFilesPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_CommandFilesValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("process-instant-template"))) || isEqual(arg + 1, STRING_LITERAL("t")))
                {
                    argumentConsumed = true;
                    m_InstantTemplateFilesPassed = true;
                    parseArgs(arg, argc, argv, ++i, m_InstantTemplateFilesValue, 1, SIZE_MAX);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("output-dependencies"))) || isEqual(arg + 1, STRING_LITERAL("d")))
                {
                    argumentConsumed = true;
                    m_OutputDependenciesStylePassed = true;
                    parseArg(arg, argc, argv, ++i, m_OutputDependenciesStyleValue, false);
                }
                else if ((arg[1] == '-' && isEqual(arg + 2, STRING_LITERAL("prompt"))) || isEqual(arg + 1, STRING_LITERAL("p")))
                {
                    argumentConsumed = true;
                    m_PromptPassed = true;
                }
            }
            
            if (!argumentConsumed && !m_CommandFilesPassed)
            {
                argumentConsumed = true;
                m_CommandFilesPassed = true;
                parseArgs("--command-file", argc, argv, i, m_CommandFilesValue, 1, static_cast<size_t>(-1));
            }
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
        if (
               m_HelpPassed == true
            && m_CommandsPassed == false
            && m_CommandFilesPassed == false
            && m_InstantTemplateFilesPassed == false
            && m_OutputDependenciesStylePassed == false
            && m_PromptPassed == false
        )
        {
            return eHelp;
        }
        
        if (
               m_HelpPassed == false
            && m_CommandsPassed == true
            && m_CommandFilesPassed == false
            && m_InstantTemplateFilesPassed == false
            && m_OutputDependenciesStylePassed == false
            && m_PromptPassed == false
        )
        {
            return eExecuteCommand;
        }
        
        if (
               m_HelpPassed == false
            && m_CommandsPassed == false
            && m_CommandFilesPassed == true
            && m_InstantTemplateFilesPassed == false
            && m_OutputDependenciesStylePassed == false
        )
        {
            return eExecuteCommandFile;
        }
        
        if (
               m_HelpPassed == false
            && m_CommandsPassed == false
            && m_CommandFilesPassed == true
            && m_InstantTemplateFilesPassed == false
            && m_OutputDependenciesStylePassed == true
        )
        {
            return eCommandFileDependencies;
        }
        
        if (
               m_HelpPassed == false
            && m_CommandsPassed == false
            && m_CommandFilesPassed == false
            && m_InstantTemplateFilesPassed == true
            && m_OutputDependenciesStylePassed == false
        )
        {
            return eProcessInstantTemplate;
        }
        
        
        if (
               !m_HelpPassed
            && !m_CommandsPassed
            && !m_CommandFilesPassed
            && !m_InstantTemplateFilesPassed
            && !m_OutputDependenciesStylePassed
            && !m_PromptPassed
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }
    
    bool getHelp() const
    {
        return m_HelpPassed;
    }
    
    std::vector<StringT> getCommands() const
    {
        return m_CommandsValue;
    }

    std::vector<StringT> getCommandFiles() const
    {
        return m_CommandFilesValue;
    }

    std::vector<StringT> getInstantTemplateFiles() const
    {
        return m_InstantTemplateFilesValue;
    }

    StringT getOutputDependenciesStyle() const
    {
        return m_OutputDependenciesStyleValue;
    }

    bool getPrompt() const
    {
        return m_PromptPassed;
    }
    
    
    bool hasHelp() const
    {
        return m_HelpPassed;
    }
    
    bool hasCommands() const
    {
        return m_CommandsPassed;
    }
    
    bool hasCommandFiles() const
    {
        return m_CommandFilesPassed;
    }
    
    bool hasInstantTemplateFiles() const
    {
        return m_InstantTemplateFilesPassed;
    }
    
    bool hasOutputDependenciesStyle() const
    {
        return m_OutputDependenciesStylePassed;
    }
    
    bool hasPrompt() const
    {
        return m_PromptPassed;
    }
    

    
private:    
    bool isOption(const CharT* arg)
    {
        bool result = false;
        if (arg && arg[0] == '-')
        {
            if (arg[1] == '-')
            {
                if(
                       isEqual(arg + 2, STRING_LITERAL("help"))
                    || isEqual(arg + 2, STRING_LITERAL("command"))
                    || isEqual(arg + 2, STRING_LITERAL("command-file"))
                    || isEqual(arg + 2, STRING_LITERAL("process-instant-template"))
                    || isEqual(arg + 2, STRING_LITERAL("output-dependencies"))
                    || isEqual(arg + 2, STRING_LITERAL("prompt"))
                )
                {
                    result = true;
                }
                
            }
            else if(
                   isEqual(arg + 1, STRING_LITERAL("h"))
                || isEqual(arg + 1, STRING_LITERAL("c"))
                || isEqual(arg + 1, STRING_LITERAL("f"))
                || isEqual(arg + 1, STRING_LITERAL("t"))
                || isEqual(arg + 1, STRING_LITERAL("d"))
                || isEqual(arg + 1, STRING_LITERAL("p"))
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
    bool m_HelpPassed;
    bool m_CommandsPassed;
    bool m_CommandFilesPassed;
    bool m_InstantTemplateFilesPassed;
    bool m_OutputDependenciesStylePassed;
    bool m_PromptPassed;

    std::vector<StringT> m_CommandsValue;
    std::vector<StringT> m_CommandFilesValue;
    std::vector<StringT> m_InstantTemplateFilesValue;
    StringT m_OutputDependenciesStyleValue;
};
