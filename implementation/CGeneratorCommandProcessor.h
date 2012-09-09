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

#ifndef INCLUDED_CGENERATORCOMMANDPROCESSOR_H_5817028
#define INCLUDED_CGENERATORCOMMANDPROCESSOR_H_5817028

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "CCommandFileLineParser.gen.h"
#include <boost/foreach.hpp>
#include <stdexcept>
#include "FileSystem.h"
#include "StringLiteral.h"
#include "System.h"
#include "CInlineTemplateParameters.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

///processes user commands
template <typename StringT>
class CGeneratorCommandProcessor
{
    typedef CCommandFileLineParser<StringT> ParserT;
    typedef typename StringT::value_type CharT;
public:
    class ExInvalidCommandOptions : public std::runtime_error 
    { public: ExInvalidCommandOptions() : std::runtime_error( "Invalid command options. Please use --help to get the option description.") {}};

    ///process a stream of commands
    template <typename InputStreamT, typename GeneratorT>
    void processCommandStream( InputStreamT& stream, GeneratorT& generator, const StringT& commandFileName, bool disableReset = false)
    {
        StringT command;
        while( std::getline(stream, command))
        {
            processCommand( command, generator, commandFileName, disableReset);
        }    
    }

    ///process a command for the generator
    template <typename GeneratorT>
    void processCommand( const StringT& commandText, GeneratorT& generator, const StringT& commandFileName, bool disableReset = false)
    {
        typedef typename StringT::value_type CharT;

        //check for comment
        if ( !commandText.empty() && commandText[ 0 ] == STRING_LITERAL('#') )
        {
            //exit is comment
            return;
        }

        m_parser.parse( commandText);

        const typename ParserT::ECommand command = m_parser.getCommand();

        if ( command == ParserT::eGenerate 
            || command == ParserT::eGenerateUsingIntermediateFile
            || command == ParserT::eProcessInlineTemplateFile)
        {
            CInlineTemplateParameters<StringT> inlineTemplateParameters;
            StringT outputFileName( m_parser.getOutputFile());
            bool useIntermediateFile = m_parser.getUseIntermediateOutputFile();

            if ( command == ParserT::eProcessInlineTemplateFile)
            {
                //pass inline template processing parameters
                inlineTemplateParameters.enabled = true;
                inlineTemplateParameters.inlinePrefix = m_parser.getInlinePrefix();
                inlineTemplateParameters.inlinePostfix = m_parser.getInlinePostfix();
                inlineTemplateParameters.inlineGeneratedPostfix = m_parser.getInlineGeneratedPostfix();

                //if no output file name has been passed source is also target
                if ( outputFileName.empty())
                {
                    outputFileName = m_parser.getTemplateFile();
                    useIntermediateFile = true;
                }
            }

            if ( m_parser.hasMarkup()) //switch for setting prefix and postfix at once
            {
                generator.setMarkup( m_parser.getMarkup(), m_parser.getMarkup());
            }
            else //if not overridden by markup switch apply other switches or default
            {
                generator.setMarkup( m_parser.getMarkupPrefix(), m_parser.getMarkupPostfix());
            }
            generator.generate( 
                prepareFileName( m_parser.getTemplateFile(), commandFileName),
                prepareFileName( outputFileName, commandFileName), 
                useIntermediateFile,
                prepareFileName( outputFileName + m_parser.getIntermediateOutputFileExtension(), commandFileName),
                m_parser.getAppendToFile(),
                m_parser.getParameters(),
                inlineTemplateParameters);
        }
        else if ( command == ParserT::eLoadTable )
        {
            //if no direction is explicitly provided, read all directions
            bool noDirectionSet = !m_parser.getTopDown() && !m_parser.getLeftToRight();

            generator.loadTable( 
                prepareFileName( m_parser.getTableFile(), commandFileName)
                , m_parser.hasLabel() ? m_parser.getLabel() : m_parser.getTableFile()
                , m_parser.getTopDown() || noDirectionSet
                , m_parser.getLeftToRight() || noDirectionSet
                , m_parser.getRowHeaderIndex()
                , m_parser.getColumnHeaderIndex());
        }
        else if ( command == ParserT::eUnloadTable)
        {
            std::vector<StringT> labels = m_parser.getLabelsOfTableFilesToUnload();
            BOOST_FOREACH( const StringT& label, labels)
            {
                generator.unloadTable( label);
            }
        }
        else if ( command == ParserT::eResetGenerator)
        {
            if ( !disableReset)
            {
                generator.reset();
            }
        }
        else if ( command == ParserT::eAddIncludeDirectory)
        {
            //add include directories relative to command file
            std::vector<StringT> includeDirectories = m_parser.getIncludeDirectories();
            BOOST_FOREACH( const StringT& directory, includeDirectories)
            {
                generator.addIncludeDirectory( prepareFileName( directory, commandFileName));
            }
        }
        else if ( command == ParserT::eCsvDelimiter )
        {
            CharT delimiterChar = STRING_LITERAL(';');

            if ( m_parser.hasDelimiter())
            {
                StringT delimiter = m_parser.getDelimiter();
                if ( delimiter == STRING_LITERAL("tab"))
                {
                    delimiterChar = STRING_LITERAL('\t');
                }
                else if ( delimiter.size())
                {
                    delimiterChar = delimiter[0];
                }
            }

            generator.setCsvDelimiter( delimiterChar);
        }
        else if ( command == ParserT::eCsvCommentChars )
        {
            generator.setCsvCommentChars( m_parser.getCsvCommentChars());
        }
        else if ( command == ParserT::eNoOptionsGiven)
        {
            //empty lines are OK
        }
        else
        {
            throw ExInvalidCommandOptions();
        }
    }

    //prints parser help
    void printDescription()
    {
        m_parser.printDescription();
    }

private:
    ///expand environment variables and determine location relative to command file if command file and not std stream
    static StringT prepareFileName( const StringT& filename, const StringT& base)
    {
        return relativeTo( System::expandEnvironmentVariables( filename), base);
    }

    ///determine location relative to command file if command file and not std stream
    static StringT relativeTo( const StringT& filename, const StringT& base)
    {
        //std stream specified return filename
        if ( filename == STRING_LITERAL("-"))
        {
            return filename;
        }
        else
        {
            //if no base file given, determine position relative to executable startup directory
            if ( base.empty())
            {
                return FileSystem::determineDependentLocation( filename);
            }
            else //determine path relative to base
            {
                return FileSystem::determineDependentLocation( base, filename);
            }
        }
    }
private:    
    ParserT m_parser;    
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif /* INCLUDED_CGENERATORCOMMANDPROCESSOR_H_5817028 */
