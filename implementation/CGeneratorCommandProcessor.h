//  Copyright (c) 2011-2019 Andreas Gau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//      * Neither the name of the copyright holder nor the
//        names of contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CCommandFileLineParser.gen.h"
#include <stdexcept>
#include "FileSystem.h"
#include "StringLiteral.h"
#include "System.h"
#include "CInlineTemplateParameters.h"

namespace code_creation_kit
{
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

        class ExInvalidOptionValueCsvIgnoreDoubleQuotes : public std::runtime_error 
        { public: ExInvalidOptionValueCsvIgnoreDoubleQuotes() : std::runtime_error( "Invalid option value for CSV ignore quotes.") {}};

        ///process a stream of commands
        template <typename InputStreamT, typename GeneratorT, typename LogFileT>
        void processCommandStream( InputStreamT& stream, GeneratorT& generator, const StringT& commandFileName, LogFileT& logFile, bool disableReset = false)
        {
            StringT command;
            while( std::getline(stream, command))
            {
#ifndef _MSC_VER
                //assume linux
                if ( !command.empty() && *(command.rbegin()) == '\r')
                {
                    command.resize( command.size() - 1);
                }
#endif

                processCommand( command, generator, commandFileName, logFile, disableReset);
            }
        }

        ///process a command for the generator
        template <typename GeneratorT, typename LogFileT>
        void processCommand( const StringT& commandText, GeneratorT& generator, const StringT& commandFileName, LogFileT& logFile, bool disableReset = false)
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
                    inlineTemplateParameters.inlinePad = m_parser.getInlinePad();

                    //if no output filename has been passed source is also target
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
                    prepareFileName(m_parser.getTemplateFile(), commandFileName),
                    prepareFileName(outputFileName, commandFileName),
                    useIntermediateFile,
                    m_parser.getRecycle(),
                    prepareFileName(outputFileName + m_parser.getIntermediateOutputFileExtension(), commandFileName),
                    m_parser.getAppendToFile(),
                    m_parser.getParameters(),
                    m_parser.getCanChangeTableList(),
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
                    , m_parser.getColumnHeaderIndex()
                    , m_parser.getPadRows());
            }
            else if ( command == ParserT::eUnloadTable)
            {
                std::vector<StringT> labels = m_parser.getLabelsOfTableFilesToUnload();
                for (const StringT& label : labels)
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
                for (const StringT& directory : includeDirectories)
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
                    else
                    {
                        delimiterChar = 0;
                    }
                }
                else
                {
                    throw ExInvalidCommandOptions();
                }

                generator.setCsvDelimiter( delimiterChar);
            }
            else if ( command == ParserT::eCsvCommentChars )
            {
                generator.setCsvCommentChars( m_parser.getCsvCommentChars());
            }
            else if ( command == ParserT::eCsvIgnoreDoubleQuotes )
            {
                if ( m_parser.hasCsvIgnoreDoubleQuotes())
                {
                    StringT val = m_parser.getCsvIgnoreDoubleQuotes();
                    if ( val == STRING_LITERAL("on"))
                    {
                        generator.setCsvIgnoreDoubleQuotes( true);
                    }
                    else if ( val == STRING_LITERAL("off"))
                    {
                        generator.setCsvIgnoreDoubleQuotes( false);
                    }
                    else
                    {
                        throw ExInvalidOptionValueCsvIgnoreDoubleQuotes();
                    }
                }
                else
                {
                    throw ExInvalidCommandOptions();
                }
            }
            else if ( command == ParserT::eSetLogFile )
            {
                generator.connectLogOutputStream( (std::basic_ostream<CharT, std::char_traits<CharT> >*)0);
                logFile.close();

                if( m_parser.getLogFile() == STRING_LITERAL("none"))
                {
                    //nothing to do
                }
                else
                {
                    logFile.open( m_parser.getLogFile(), m_parser.getLogFile() == STRING_LITERAL("-"), false);
                    generator.connectLogOutputStream( &logFile.get());
                }
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
}
