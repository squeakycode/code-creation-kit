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
class CCommandFileLineParser
{
    typedef typename StringT::value_type CharT; 
    typedef boost::program_options::basic_command_line_parser<CharT> CommandLineParserT;

public:
    ///lists valid option combinations
    enum ECommand
    {
        eGenerate,
        eGenerateUsingIntermediateFile,
        eProcessInlineTemplateFile,
        eResetGenerator,
        eUnloadTable,
        eLoadTable,
        eAddIncludeDirectory,
        eCsvDelimiter,
        eCsvCommentChars,
        eCsvIgnoreDoubleQuotes,
        eSetLogFile,
        eNoOptionsGiven,
        eOptionsInvalid
    };
    
    ///sets up the option description used by boost program options library
    CCommandFileLineParser()
      : m_description("Command Options")
      , m_descriptionLoadTable("Load Table")
      , m_descriptionUnloadTable("Unload Table")
      , m_descriptionGenerateFile("Generate File")
      , m_descriptionResetGenerator("Reset Generator")
      , m_descriptionAddIncludeDirectories("Add Include Directories")
      , m_descriptionCsvTableProperties("CSV Table Properties")
      , m_descriptionLogging("Logging")
    {
        // Declare the supported options.
        
        ;
        m_descriptionLoadTable.add_options() //("Load Table")
            ("load-table", value<StringT >(), "Name of the table file to load. Default when the option name is omitted.")
            ("label,a", value<StringT >(), "Specifies the label used for identifying the table when unloading. The label defaults to the name of the table passed.")
            ("top-down,t", value<bool >()->zero_tokens(), "Indicates that the table is to be read top down. If no direction is explicitly specified all directions will be read by default.")
            ("left-to-right,l", value<bool >()->zero_tokens(), "Indicates that the table is to be read from left to right. If no direction is explicitly specified all directions will be read by default.")
            ("row-header-index,w", value<unsigned int >(), "Specifies the one based index of the column containing the row names. If zero is passed the one based index of the row is used as row name.")
            ("column-header-index,n", value<unsigned int >(), "Specifies the one based index of the row containing the column names. If zero is passed the one based index of the column is used as column name.")
            ("pad-rows", value<bool >()->zero_tokens(), "Automatically fills missing row items at the end of a row with an empty string.")
        ;
        m_descriptionUnloadTable.add_options() //("Unload Table")
            ("unload-table,x", value<std::vector<StringT> >()->multitoken(), "Unload table file identified by its label. This option accepts multiple parameters for unloading more than one table. Tables having the same label are unloaded in the reverse order they are loaded.")
        ;
        m_descriptionGenerateFile.add_options() //("Generate File")
            ("template-source-file,s", value<StringT >(), "Specifies the file containing the template.")
            ("output-file,o", value<StringT >(), "Specifies the file to output to.")
            ("parameter,p", value<std::vector<StringT> >()->multitoken(), "Optional parameters used by the template forming an additional table. This option accepts multiple parameters. Therefore it must be provided last.")
            ("use-intermediate-output-file,u", value<bool >()->zero_tokens(), "Indicates that the output shall be written to an intermediate file first. This file replaces the primary output file when the generation succeeded and the intermediate and the primary output files are different otherwise it is being removed.")
            ("intermediate-file-extension,e", value<StringT >(), "The name of the intermediate file is produced by adding the given extension to the name of the output file. The extension defaults to '.intermediate' when omitted.")
            ("markup-prefix", value<StringT >(), "Sets the initial tag markup prefix.")
            ("markup-postfix", value<StringT >(), "Sets the initial tag markup postfix.")
            ("markup,m", value<StringT >(), "Sets the initial tag markup prefix and postfix. This switch overrides the switches markup-prefix and markup-postfix.")
            ("append-to-file", value<bool >()->zero_tokens(), "The output is appended to the target file. This option is ignored when used together with the use-intermediate-output-file option.")
            ("inlined", value<bool >()->zero_tokens(), "Indicates that a file with inline templates is processed. An intermediate file is automatically used when processing files with inline templates if no output file is provided. WARNING: Use this option carefully to prevent data loss. Consider using the recycle option.")
            ("inline-prefix,b", value<StringT >(), "A prefix that marks a line of an inline template file as template content. This string must not be empty.")
            ("inline-postfix,c", value<StringT >(), "A postfix that marks a line of an inline template file as template content. This string can be empty.")
            ("inline-generated-postfix,d", value<StringT >(), "A postfix that marks a line of an inline template file as generated content. This string must not be empty.")
            ("inline-pad", value<unsigned int >(), "The number of characters a generated line is padded up to with spaces before the generated postfix is appended.")
            ("recycle,y", value<bool >()->zero_tokens(), "Used together with inlined option.  If possible the target file is moved to the recycle bin of the system before it is replaced by the intermediate file. ")
        ;
        m_descriptionResetGenerator.add_options() //("Reset Generator")
            ("reset,r", value<bool >()->zero_tokens(), "Reset the generator to defaults.")
        ;
        m_descriptionAddIncludeDirectories.add_options() //("Add Include Directories")
            ("add-include-directory,i", value<std::vector<StringT> >()->multitoken(), "Adds an include directory. This option accepts multiple parameters.")
        ;
        m_descriptionCsvTableProperties.add_options() //("CSV Table Properties")
            ("csv-delimiter", value<StringT >(), "Specifies the delimiter for the next CSV-files to load. Use 'tab' for tab separated items. Use an empty string for no delimiter.")
            ("csv-comment-chars", value<StringT >(), "Specifies a list of characters as string that mark commented lines in CSV-files when found at the beginning of a line.")
            ("csv-ignore-quotes", value<StringT >(), "Double quotes in table entries are treated as normal character when on. Valid values are 'on' and 'off'. The default setting is 'off'. ")
        ;
        m_descriptionLogging.add_options() //("Logging")
            ("log-file", value<StringT >(), "Sets up logging as follows: 'none' is off, '-' is log to stdout, and any other parameter value is the name of a log file.")
            ;
        // Add the positional descriptions
        m_positionalDescription.add( "load-table", 1);

        // Connect descriptions
        m_description.add( m_descriptionLoadTable);
        m_description.add( m_descriptionUnloadTable);
        m_description.add( m_descriptionGenerateFile);
        m_description.add( m_descriptionResetGenerator);
        m_description.add( m_descriptionAddIncludeDirectories);
        m_description.add( m_descriptionCsvTableProperties);
        m_description.add( m_descriptionLogging);
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
        bool providedTableFile = hasTableFile();
        bool providedLabel = hasLabel();
        bool providedTopDown = hasTopDown();
        bool providedLeftToRight = hasLeftToRight();
        bool providedRowHeaderIndex = hasRowHeaderIndex();
        bool providedColumnHeaderIndex = hasColumnHeaderIndex();
        bool providedPadRows = hasPadRows();
        bool providedLabelsOfTableFilesToUnload = hasLabelsOfTableFilesToUnload();
        bool providedTemplateFile = hasTemplateFile();
        bool providedOutputFile = hasOutputFile();
        bool providedParameters = hasParameters();
        bool providedUseIntermediateOutputFile = hasUseIntermediateOutputFile();
        bool providedIntermediateOutputFileExtension = hasIntermediateOutputFileExtension();
        bool providedMarkupPrefix = hasMarkupPrefix();
        bool providedMarkupPostfix = hasMarkupPostfix();
        bool providedMarkup = hasMarkup();
        bool providedAppendToFile = hasAppendToFile();
        bool providedInline = hasInline();
        bool providedInlinePrefix = hasInlinePrefix();
        bool providedInlinePostfix = hasInlinePostfix();
        bool providedInlineGeneratedPostfix = hasInlineGeneratedPostfix();
        bool providedInlinePad = hasInlinePad();
        bool providedRecycle = hasRecycle();
        bool providedReset = hasReset();
        bool providedIncludeDirectories = hasIncludeDirectories();
        bool providedDelimiter = hasDelimiter();
        bool providedCsvCommentChars = hasCsvCommentChars();
        bool providedCsvIgnoreDoubleQuotes = hasCsvIgnoreDoubleQuotes();
        bool providedLogFile = hasLogFile();
    
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == true
            && providedOutputFile == true
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eGenerate;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == true
            && providedOutputFile == true
            && providedUseIntermediateOutputFile == true
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eGenerateUsingIntermediateFile;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == true
            && providedInline == true
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eProcessInlineTemplateFile;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == true
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eResetGenerator;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == true
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eUnloadTable;
        }
        
        if (
               providedTableFile == true
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eLoadTable;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == true
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eAddIncludeDirectory;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == true
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eCsvDelimiter;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == true
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == false
        )
        {
            return eCsvCommentChars;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == true
            && providedLogFile == false
        )
        {
            return eCsvIgnoreDoubleQuotes;
        }
        
        if (
               providedTableFile == false
            && providedLabel == false
            && providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedPadRows == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedAppendToFile == false
            && providedInline == false
            && providedInlinePrefix == false
            && providedInlinePostfix == false
            && providedInlineGeneratedPostfix == false
            && providedInlinePad == false
            && providedRecycle == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
            && providedCsvIgnoreDoubleQuotes == false
            && providedLogFile == true
        )
        {
            return eSetLogFile;
        }
        
        
        if (
               !providedTableFile
            && !providedLabel
            && !providedTopDown
            && !providedLeftToRight
            && !providedRowHeaderIndex
            && !providedColumnHeaderIndex
            && !providedPadRows
            && !providedLabelsOfTableFilesToUnload
            && !providedTemplateFile
            && !providedOutputFile
            && !providedParameters
            && !providedUseIntermediateOutputFile
            && !providedIntermediateOutputFileExtension
            && !providedMarkupPrefix
            && !providedMarkupPostfix
            && !providedMarkup
            && !providedAppendToFile
            && !providedInline
            && !providedInlinePrefix
            && !providedInlinePostfix
            && !providedInlineGeneratedPostfix
            && !providedInlinePad
            && !providedRecycle
            && !providedReset
            && !providedIncludeDirectories
            && !providedDelimiter
            && !providedCsvCommentChars
            && !providedCsvIgnoreDoubleQuotes
            && !providedLogFile
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
    }

    ///returns the provided value
    StringT getTableFile() const
    {
        return m_vmap["load-table"].as<StringT >();
    }
    
    ///returns the provided value
    StringT getLabel() const
    {
        return m_vmap["label"].as<StringT >();
    }
    
    ///returns the provided value or false as default
    bool getTopDown() const
    {
        if ( hasTopDown())
        {
            return m_vmap["top-down"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value or false as default
    bool getLeftToRight() const
    {
        if ( hasLeftToRight())
        {
            return m_vmap["left-to-right"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value or 1 as default
    unsigned int getRowHeaderIndex() const
    {
        if ( hasRowHeaderIndex())
        {
            return m_vmap["row-header-index"].as<unsigned int >();
        }
        return 1;
    }
    
    ///returns the provided value or 1 as default
    unsigned int getColumnHeaderIndex() const
    {
        if ( hasColumnHeaderIndex())
        {
            return m_vmap["column-header-index"].as<unsigned int >();
        }
        return 1;
    }
    
    ///returns the provided value or false as default
    bool getPadRows() const
    {
        if ( hasPadRows())
        {
            return m_vmap["pad-rows"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value
    std::vector<StringT> getLabelsOfTableFilesToUnload() const
    {
        return m_vmap["unload-table"].as<std::vector<StringT> >();
    }
    
    ///returns the provided value
    StringT getTemplateFile() const
    {
        return m_vmap["template-source-file"].as<StringT >();
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("") as default
    StringT getOutputFile() const
    {
        if ( hasOutputFile())
        {
            return m_vmap["output-file"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("");
    }
    
    ///returns the provided value or std::vector<StringT>() as default
    std::vector<StringT> getParameters() const
    {
        if ( hasParameters())
        {
            return m_vmap["parameter"].as<std::vector<StringT> >();
        }
        return std::vector<StringT>();
    }
    
    ///returns the provided value or false as default
    bool getUseIntermediateOutputFile() const
    {
        if ( hasUseIntermediateOutputFile())
        {
            return m_vmap["use-intermediate-output-file"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>(".intermediate") as default
    StringT getIntermediateOutputFileExtension() const
    {
        if ( hasIntermediateOutputFileExtension())
        {
            return m_vmap["intermediate-file-extension"].as<StringT >();
        }
        return boost::lexical_cast<StringT>(".intermediate");
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("[") as default
    StringT getMarkupPrefix() const
    {
        if ( hasMarkupPrefix())
        {
            return m_vmap["markup-prefix"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("[");
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("]") as default
    StringT getMarkupPostfix() const
    {
        if ( hasMarkupPostfix())
        {
            return m_vmap["markup-postfix"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("]");
    }
    
    ///returns the provided value
    StringT getMarkup() const
    {
        return m_vmap["markup"].as<StringT >();
    }
    
    ///returns the provided value or false as default
    bool getAppendToFile() const
    {
        if ( hasAppendToFile())
        {
            return m_vmap["append-to-file"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value
    bool getInline() const
    {
        return m_vmap["inlined"].as<bool >();
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("//<>") as default
    StringT getInlinePrefix() const
    {
        if ( hasInlinePrefix())
        {
            return m_vmap["inline-prefix"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("//<>");
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("") as default
    StringT getInlinePostfix() const
    {
        if ( hasInlinePostfix())
        {
            return m_vmap["inline-postfix"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("");
    }
    
    ///returns the provided value or boost::lexical_cast<StringT>("//$") as default
    StringT getInlineGeneratedPostfix() const
    {
        if ( hasInlineGeneratedPostfix())
        {
            return m_vmap["inline-generated-postfix"].as<StringT >();
        }
        return boost::lexical_cast<StringT>("//$");
    }
    
    ///returns the provided value or 0 as default
    unsigned int getInlinePad() const
    {
        if ( hasInlinePad())
        {
            return m_vmap["inline-pad"].as<unsigned int >();
        }
        return 0;
    }
    
    ///returns the provided value or false as default
    bool getRecycle() const
    {
        if ( hasRecycle())
        {
            return m_vmap["recycle"].as<bool >();
        }
        return false;
    }
    
    ///returns the provided value
    bool getReset() const
    {
        return m_vmap["reset"].as<bool >();
    }
    
    ///returns the provided value
    std::vector<StringT> getIncludeDirectories() const
    {
        return m_vmap["add-include-directory"].as<std::vector<StringT> >();
    }
    
    ///returns the provided value
    StringT getDelimiter() const
    {
        return m_vmap["csv-delimiter"].as<StringT >();
    }
    
    ///returns the provided value
    StringT getCsvCommentChars() const
    {
        return m_vmap["csv-comment-chars"].as<StringT >();
    }
    
    ///returns the provided value
    StringT getCsvIgnoreDoubleQuotes() const
    {
        return m_vmap["csv-ignore-quotes"].as<StringT >();
    }
    
    ///returns the provided value
    StringT getLogFile() const
    {
        return m_vmap["log-file"].as<StringT >();
    }
    

    ///indicates that the option load-table has been provided
    bool hasTableFile() const
    {
        return m_vmap.count( "load-table") != 0;
    }
    
    ///indicates that the option label has been provided
    bool hasLabel() const
    {
        return m_vmap.count( "label") != 0;
    }
    
    ///indicates that the option top-down has been provided
    bool hasTopDown() const
    {
        return m_vmap.count( "top-down") != 0;
    }
    
    ///indicates that the option left-to-right has been provided
    bool hasLeftToRight() const
    {
        return m_vmap.count( "left-to-right") != 0;
    }
    
    ///indicates that the option row-header-index has been provided
    bool hasRowHeaderIndex() const
    {
        return m_vmap.count( "row-header-index") != 0;
    }
    
    ///indicates that the option column-header-index has been provided
    bool hasColumnHeaderIndex() const
    {
        return m_vmap.count( "column-header-index") != 0;
    }
    
    ///indicates that the option pad-rows has been provided
    bool hasPadRows() const
    {
        return m_vmap.count( "pad-rows") != 0;
    }
    
    ///indicates that the option unload-table has been provided
    bool hasLabelsOfTableFilesToUnload() const
    {
        return m_vmap.count( "unload-table") != 0;
    }
    
    ///indicates that the option template-source-file has been provided
    bool hasTemplateFile() const
    {
        return m_vmap.count( "template-source-file") != 0;
    }
    
    ///indicates that the option output-file has been provided
    bool hasOutputFile() const
    {
        return m_vmap.count( "output-file") != 0;
    }
    
    ///indicates that the option parameter has been provided
    bool hasParameters() const
    {
        return m_vmap.count( "parameter") != 0;
    }
    
    ///indicates that the option use-intermediate-output-file has been provided
    bool hasUseIntermediateOutputFile() const
    {
        return m_vmap.count( "use-intermediate-output-file") != 0;
    }
    
    ///indicates that the option intermediate-file-extension has been provided
    bool hasIntermediateOutputFileExtension() const
    {
        return m_vmap.count( "intermediate-file-extension") != 0;
    }
    
    ///indicates that the option markup-prefix has been provided
    bool hasMarkupPrefix() const
    {
        return m_vmap.count( "markup-prefix") != 0;
    }
    
    ///indicates that the option markup-postfix has been provided
    bool hasMarkupPostfix() const
    {
        return m_vmap.count( "markup-postfix") != 0;
    }
    
    ///indicates that the option markup has been provided
    bool hasMarkup() const
    {
        return m_vmap.count( "markup") != 0;
    }
    
    ///indicates that the option append-to-file has been provided
    bool hasAppendToFile() const
    {
        return m_vmap.count( "append-to-file") != 0;
    }
    
    ///indicates that the option inlined has been provided
    bool hasInline() const
    {
        return m_vmap.count( "inlined") != 0;
    }
    
    ///indicates that the option inline-prefix has been provided
    bool hasInlinePrefix() const
    {
        return m_vmap.count( "inline-prefix") != 0;
    }
    
    ///indicates that the option inline-postfix has been provided
    bool hasInlinePostfix() const
    {
        return m_vmap.count( "inline-postfix") != 0;
    }
    
    ///indicates that the option inline-generated-postfix has been provided
    bool hasInlineGeneratedPostfix() const
    {
        return m_vmap.count( "inline-generated-postfix") != 0;
    }
    
    ///indicates that the option inline-pad has been provided
    bool hasInlinePad() const
    {
        return m_vmap.count( "inline-pad") != 0;
    }
    
    ///indicates that the option recycle has been provided
    bool hasRecycle() const
    {
        return m_vmap.count( "recycle") != 0;
    }
    
    ///indicates that the option reset has been provided
    bool hasReset() const
    {
        return m_vmap.count( "reset") != 0;
    }
    
    ///indicates that the option add-include-directory has been provided
    bool hasIncludeDirectories() const
    {
        return m_vmap.count( "add-include-directory") != 0;
    }
    
    ///indicates that the option csv-delimiter has been provided
    bool hasDelimiter() const
    {
        return m_vmap.count( "csv-delimiter") != 0;
    }
    
    ///indicates that the option csv-comment-chars has been provided
    bool hasCsvCommentChars() const
    {
        return m_vmap.count( "csv-comment-chars") != 0;
    }
    
    ///indicates that the option csv-ignore-quotes has been provided
    bool hasCsvIgnoreDoubleQuotes() const
    {
        return m_vmap.count( "csv-ignore-quotes") != 0;
    }
    
    ///indicates that the option log-file has been provided
    bool hasLogFile() const
    {
        return m_vmap.count( "log-file") != 0;
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
    boost::program_options::options_description m_descriptionLoadTable; ///<the option description of group: Load Table
    boost::program_options::options_description m_descriptionUnloadTable; ///<the option description of group: Unload Table
    boost::program_options::options_description m_descriptionGenerateFile; ///<the option description of group: Generate File
    boost::program_options::options_description m_descriptionResetGenerator; ///<the option description of group: Reset Generator
    boost::program_options::options_description m_descriptionAddIncludeDirectories; ///<the option description of group: Add Include Directories
    boost::program_options::options_description m_descriptionCsvTableProperties; ///<the option description of group: CSV Table Properties
    boost::program_options::options_description m_descriptionLogging; ///<the option description of group: Logging
    boost::program_options::positional_options_description m_positionalDescription; ///<description of positional options 
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

