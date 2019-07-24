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
class CCommandLineParser
{
    typedef typename StringT::value_type CharT; 
    typedef boost::program_options::basic_command_line_parser<CharT> CommandLineParserT;

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
    
    ///sets up the option description used by boost program options library
    CCommandLineParser()
      : m_description("Command Line Options")
      , m_descriptionhelp("Help")
      , m_descriptionCommand("Command")
      , m_descriptionCommandFile("Command File")
    {
        // Declare the supported options.
        
        ;
        m_descriptionhelp.add_options() //("Help")
            ("help,h", "Print help message")
        ;
        m_descriptionCommand.add_options() //("Command")
            ("command,c", value<std::vector<StringT> >(), "Execute a command. This option can be specified multiple times. The commands are executed in the order they are specified.")
        ;
        m_descriptionCommandFile.add_options() //("Command File")
            ("command-file,f", value<std::vector<StringT> >(), "Execute a command file. This option can be specified multiple times. The command files are executed in the order they are specified. Default when the option name is omitted.")
            ("process-instant-template,t", value<std::vector<StringT> >(), "Process an instant template file (extension itpl). An instant template file contains or loads all needed tables. The name of the output file is build by removing the extension. This option can be specified multiple times. The commands are executed in the order they are specified.")
            ("output-dependencies,d", value<StringT >(), "Output  command files dependencies instead of processing. Available styles are mpc (Meta Project Creator) or vs (Visual Studio).")
            ("prompt,p", value<bool >()->zero_tokens(), "Wait after processing or dependency output on a key press. Prompt for rerunning the command file(s).")
            ;
        // Add the positional descriptions
        m_positionalDescription.add( "command-file", -1);

        // Connect descriptions
        m_description.add( m_descriptionhelp);
        m_description.add( m_descriptionCommand);
        m_description.add( m_descriptionCommandFile);
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
        bool providedHelp = hasHelp();
        bool providedCommands = hasCommands();
        bool providedCommandFiles = hasCommandFiles();
        bool providedInstantTemplateFiles = hasInstantTemplateFiles();
        bool providedOutputDependenciesStyle = hasOutputDependenciesStyle();
        bool providedPrompt = hasPrompt();
    
        if (
               providedHelp == true
            && providedCommands == false
            && providedCommandFiles == false
            && providedInstantTemplateFiles == false
            && providedOutputDependenciesStyle == false
            && providedPrompt == false
        )
        {
            return eHelp;
        }
        
        if (
               providedHelp == false
            && providedCommands == true
            && providedCommandFiles == false
            && providedInstantTemplateFiles == false
            && providedOutputDependenciesStyle == false
            && providedPrompt == false
        )
        {
            return eExecuteCommand;
        }
        
        if (
               providedHelp == false
            && providedCommands == false
            && providedCommandFiles == true
            && providedInstantTemplateFiles == false
            && providedOutputDependenciesStyle == false
        )
        {
            return eExecuteCommandFile;
        }
        
        if (
               providedHelp == false
            && providedCommands == false
            && providedCommandFiles == true
            && providedInstantTemplateFiles == false
            && providedOutputDependenciesStyle == true
        )
        {
            return eCommandFileDependencies;
        }
        
        if (
               providedHelp == false
            && providedCommands == false
            && providedCommandFiles == false
            && providedInstantTemplateFiles == true
            && providedOutputDependenciesStyle == false
        )
        {
            return eProcessInstantTemplate;
        }
        
        
        if (
               !providedHelp
            && !providedCommands
            && !providedCommandFiles
            && !providedInstantTemplateFiles
            && !providedOutputDependenciesStyle
            && !providedPrompt
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }

    ///returns the provided value
    std::vector<StringT> getCommands() const
    {
        return m_vmap["command"].as<std::vector<StringT> >();
    }
    
    ///returns the provided value
    std::vector<StringT> getCommandFiles() const
    {
        return m_vmap["command-file"].as<std::vector<StringT> >();
    }
    
    ///returns the provided value
    std::vector<StringT> getInstantTemplateFiles() const
    {
        return m_vmap["process-instant-template"].as<std::vector<StringT> >();
    }
    
    ///returns the provided value
    StringT getOutputDependenciesStyle() const
    {
        return m_vmap["output-dependencies"].as<StringT >();
    }
    
    ///returns the provided value
    bool getPrompt() const
    {
        return m_vmap["prompt"].as<bool >();
    }
    

    ///indicates that the option help has been provided
    bool hasHelp() const
    {
        return m_vmap.count( "help") != 0;
    }
    
    ///indicates that the option command has been provided
    bool hasCommands() const
    {
        return m_vmap.count( "command") != 0;
    }
    
    ///indicates that the option command-file has been provided
    bool hasCommandFiles() const
    {
        return m_vmap.count( "command-file") != 0;
    }
    
    ///indicates that the option process-instant-template has been provided
    bool hasInstantTemplateFiles() const
    {
        return m_vmap.count( "process-instant-template") != 0;
    }
    
    ///indicates that the option output-dependencies has been provided
    bool hasOutputDependenciesStyle() const
    {
        return m_vmap.count( "output-dependencies") != 0;
    }
    
    ///indicates that the option prompt has been provided
    bool hasPrompt() const
    {
        return m_vmap.count( "prompt") != 0;
    }
    
    
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
    boost::program_options::options_description m_descriptionhelp; ///<the option description of group: Help
    boost::program_options::options_description m_descriptionCommand; ///<the option description of group: Command
    boost::program_options::options_description m_descriptionCommandFile; ///<the option description of group: Command File
    boost::program_options::positional_options_description m_positionalDescription; ///<description of positional options 
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

