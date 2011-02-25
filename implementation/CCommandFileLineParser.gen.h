//   Copyright (C) 2011 Andreas Gau
//
//   This file is part of the code-creation-kit.
//
//   The code-creation-kit is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   The code-creation-kit is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#ifndef INCLUDED_CCommandFileLineParser_H
#define INCLUDED_CCommandFileLineParser_H

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

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
        eResetGenerator,
        eUnloadTable,
        eLoadTable,
        eAddIncludeDirectory,
        eCsvDelimiter,
        eCsvCommentChars,
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
    {
        // Declare the supported options.
        
        ;
        m_descriptionLoadTable.add_options() //("Load Table")
            ("top-down,t", value<bool >()->zero_tokens(), "Indicates that the table is to be read top down. If no direction is explicitly specified all directions will be read by default.")
            ("left-to-right,l", value<bool >()->zero_tokens(), "Indicates that the table is to be read from left to right. If no direction is explicitly specified all directions will be read by default.")
            ("row-header-index,w", value<unsigned int >(), "Specifies the one based index of the column containing the row names. If zero is passed the one based index of the row is used as row name.")
            ("column-header-index,n", value<unsigned int >(), "Specifies the one based index of the row containing the column names. If zero is passed the one based index of the column is used as column name.")
            ("label,a", value<StringT >(), "Specifies the label used for identifying the table when unloading. The label defaults to the name of the table passed.")
            ("load-table", value<StringT >(), "Name of the table file to load. Default when the option name is omitted.")
        ;
        m_descriptionUnloadTable.add_options() //("Unload Table")
            ("unload-table,x", value<std::vector<StringT> >()->multitoken(), "Unload table file identified by its label. This option accepts multiple parameters for ubloading more than one table. Tables having the same label are unloaded in the reverse order they are loaded.")
        ;
        m_descriptionGenerateFile.add_options() //("Generate File")
            ("template-source-file,s", value<StringT >(), "Specifies the file containing the template.")
            ("output-file,o", value<StringT >(), "Specifies the file to output to.")
            ("parameter,p", value<std::vector<StringT> >()->multitoken(), "Optional parameters used by the template forming an additional table. This option accepts multiple parameters. Therefore it must be provided last.")
            ("use-intermediate-output-file,u", value<bool >()->zero_tokens(), "Indicates that the output shall be written to an intermediate file first. This file replaces the primary output file when the generation succeeded and the intermediate and the primary output files are different otherwise it is being removed.")
            ("intermediate-file-extension,e", value<StringT >(), "The name of the intermediate is produced by adding the given extension to the name of the output file. The extension defaults to '.intermediate' when omitted.")
            ("markup-prefix", value<StringT >(), "Sets the initial tag markup prefix.")
            ("markup-postfix", value<StringT >(), "Sets the initial tag markup postfix.")
            ("markup,m", value<StringT >(), "Sets the initial tag markup prefix and postfix. This switch overrides the switches markup-prefix and markup-postfix.")
        ;
        m_descriptionResetGenerator.add_options() //("Reset Generator")
            ("reset,r", value<bool >()->zero_tokens(), "Reset the generator to defaults.")
        ;
        m_descriptionAddIncludeDirectories.add_options() //("Add Include Directories")
            ("add-include-directory,i", value<std::vector<StringT> >()->multitoken(), "Adds an include directory. This option accepts multiple parameters.")
        ;
        m_descriptionCsvTableProperties.add_options() //("CSV Table Properties")
            ("csv-delimiter", value<StringT >(), "Specifies the delimiter for the next csv files to load. Use 'tab' for tab separated items.")
            ("csv-comment-chars", value<StringT >(), "Specifies a list of characters as string that mark commented lines in CSV-files when found at the beginning of a line.")
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
        bool providedTopDown = hasTopDown();
        bool providedLeftToRight = hasLeftToRight();
        bool providedRowHeaderIndex = hasRowHeaderIndex();
        bool providedColumnHeaderIndex = hasColumnHeaderIndex();
        bool providedLabel = hasLabel();
        bool providedTableFile = hasTableFile();
        bool providedLabelsOfTableFilesToUnload = hasLabelsOfTableFilesToUnload();
        bool providedTemplateFile = hasTemplateFile();
        bool providedOutputFile = hasOutputFile();
        bool providedParameters = hasParameters();
        bool providedUseIntermediateOutputFile = hasUseIntermediateOutputFile();
        bool providedIntermediateOutputFileExtension = hasIntermediateOutputFileExtension();
        bool providedMarkupPrefix = hasMarkupPrefix();
        bool providedMarkupPostfix = hasMarkupPostfix();
        bool providedMarkup = hasMarkup();
        bool providedReset = hasReset();
        bool providedIncludeDirectories = hasIncludeDirectories();
        bool providedDelimiter = hasDelimiter();
        bool providedCsvCommentChars = hasCsvCommentChars();
    
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == true
            && providedOutputFile == true
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
        )
        {
            return eGenerate;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == true
            && providedOutputFile == true
            && providedUseIntermediateOutputFile == true
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
        )
        {
            return eGenerateUsingIntermediateFile;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedReset == true
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
        )
        {
            return eResetGenerator;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == true
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
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
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == false
        )
        {
            return eLoadTable;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedReset == false
            && providedIncludeDirectories == true
            && providedDelimiter == false
            && providedCsvCommentChars == false
        )
        {
            return eAddIncludeDirectory;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == true
            && providedCsvCommentChars == false
        )
        {
            return eCsvDelimiter;
        }
        
        if (
               providedTopDown == false
            && providedLeftToRight == false
            && providedRowHeaderIndex == false
            && providedColumnHeaderIndex == false
            && providedLabel == false
            && providedTableFile == false
            && providedLabelsOfTableFilesToUnload == false
            && providedTemplateFile == false
            && providedOutputFile == false
            && providedParameters == false
            && providedUseIntermediateOutputFile == false
            && providedIntermediateOutputFileExtension == false
            && providedMarkupPrefix == false
            && providedMarkupPostfix == false
            && providedMarkup == false
            && providedReset == false
            && providedIncludeDirectories == false
            && providedDelimiter == false
            && providedCsvCommentChars == true
        )
        {
            return eCsvCommentChars;
        }
        
        
        if (
               !providedTopDown
            && !providedLeftToRight
            && !providedRowHeaderIndex
            && !providedColumnHeaderIndex
            && !providedLabel
            && !providedTableFile
            && !providedLabelsOfTableFilesToUnload
            && !providedTemplateFile
            && !providedOutputFile
            && !providedParameters
            && !providedUseIntermediateOutputFile
            && !providedIntermediateOutputFileExtension
            && !providedMarkupPrefix
            && !providedMarkupPostfix
            && !providedMarkup
            && !providedReset
            && !providedIncludeDirectories
            && !providedDelimiter
            && !providedCsvCommentChars
        )
        {
            return eNoOptionsGiven;
        }
        
        return eOptionsInvalid;
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
    
    ///returns the provided value
    StringT getLabel() const
    {
        return m_vmap["label"].as<StringT >();
    }
    
    ///returns the provided value
    StringT getTableFile() const
    {
        return m_vmap["load-table"].as<StringT >();
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
    
    ///returns the provided value
    StringT getOutputFile() const
    {
        return m_vmap["output-file"].as<StringT >();
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
    
    ///indicates that the option label has been provided
    bool hasLabel() const
    {
        return m_vmap.count( "label") != 0;
    }
    
    ///indicates that the option load-table has been provided
    bool hasTableFile() const
    {
        return m_vmap.count( "load-table") != 0;
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
    boost::program_options::positional_options_description m_positionalDescription; ///<description of positional options 
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif /* INCLUDED_CCommandFileLineParser_H */
