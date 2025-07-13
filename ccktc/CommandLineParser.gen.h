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
class CCommandLineParser
{
    typedef typename StringT::value_type CharT;
    constexpr static size_t cLeftColumnSize = 40;

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
            const CharT* pArgument = argv[ i ];
            if (pArgument && pArgument[0] == '-')
            {
                if ((pArgument[1] == '-' && isEqual(pArgument + 2, "help")) || isEqual(pArgument + 1, "h"))
                {
                    argumentConsumed = true;
                    m_HelpPassed = true;
                }
                else if ((pArgument[1] == '-' && isEqual(pArgument + 2, "command")) || isEqual(pArgument + 1, "c"))
                {
                    argumentConsumed = true;
                    m_CommandsPassed = true;
                    parseArgs(pArgument, argc, argv, ++i, m_CommandsValue, 1, SIZE_MAX);
                }
                else if ((pArgument[1] == '-' && isEqual(pArgument + 2, "command-file")) || isEqual(pArgument + 1, "f"))
                {
                    argumentConsumed = true;
                    m_CommandFilesPassed = true;
                    parseArgs(pArgument, argc, argv, ++i, m_CommandFilesValue, 1, SIZE_MAX);
                }
                else if ((pArgument[1] == '-' && isEqual(pArgument + 2, "process-instant-template")) || isEqual(pArgument + 1, "t"))
                {
                    argumentConsumed = true;
                    m_InstantTemplateFilesPassed = true;
                    parseArgs(pArgument, argc, argv, ++i, m_InstantTemplateFilesValue, 1, SIZE_MAX);
                }
                else if ((pArgument[1] == '-' && isEqual(pArgument + 2, "output-dependencies")) || isEqual(pArgument + 1, "d"))
                {
                    argumentConsumed = true;
                    m_OutputDependenciesStylePassed = true;
                    parseArg(pArgument, argc, argv, ++i, m_OutputDependenciesStyleValue, false);
                }
                else if ((pArgument[1] == '-' && isEqual(pArgument + 2, "prompt")) || isEqual(pArgument + 1, "p"))
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
    
    [[nodiscard]] bool getHelp() const
    {
        return m_HelpPassed;
    }
    
    [[nodiscard]] std::vector<StringT> getCommands() const
    {
        return m_CommandsValue;
    }

    [[nodiscard]] std::vector<StringT> getCommandFiles() const
    {
        return m_CommandFilesValue;
    }

    [[nodiscard]] std::vector<StringT> getInstantTemplateFiles() const
    {
        return m_InstantTemplateFilesValue;
    }

    [[nodiscard]] StringT getOutputDependenciesStyle() const
    {
        return m_OutputDependenciesStyleValue;
    }

    [[nodiscard]] bool getPrompt() const
    {
        return m_PromptPassed;
    }
    
    
    [[nodiscard]] bool hasHelp() const
    {
        return m_HelpPassed;
    }
    
    [[nodiscard]] bool hasCommands() const
    {
        return m_CommandsPassed;
    }
    
    [[nodiscard]] bool hasCommandFiles() const
    {
        return m_CommandFilesPassed;
    }
    
    [[nodiscard]] bool hasInstantTemplateFiles() const
    {
        return m_InstantTemplateFilesPassed;
    }
    
    [[nodiscard]] bool hasOutputDependenciesStyle() const
    {
        return m_OutputDependenciesStylePassed;
    }
    
    [[nodiscard]] bool hasPrompt() const
    {
        return m_PromptPassed;
    }
    

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
                       isEqual(pArgument + 2, "help")
                    || isEqual(pArgument + 2, "command")
                    || isEqual(pArgument + 2, "command-file")
                    || isEqual(pArgument + 2, "process-instant-template")
                    || isEqual(pArgument + 2, "output-dependencies")
                    || isEqual(pArgument + 2, "prompt")
                )
                {
                    result = true;
                }
                
            }
            else if(
                   isEqual(pArgument + 1, "h")
                || isEqual(pArgument + 1, "c")
                || isEqual(pArgument + 1, "f")
                || isEqual(pArgument + 1, "t")
                || isEqual(pArgument + 1, "d")
                || isEqual(pArgument + 1, "p")
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
