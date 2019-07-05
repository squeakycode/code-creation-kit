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

#include <list>
#include <stdexcept>
#include "FileSystem.h"
#include "CSourceFile.h"
#include "CNul.h"

namespace code_creation_kit
{
    class TemplateFileT;

    ///Holds exceptions thrown by CTemplateLoader for template argument independent access
    class CTemplateLoaderExceptions
    {
    public:
        class ExCyclicInclusion : public std::runtime_error 
        { public: ExCyclicInclusion() : std::runtime_error( "Cyclic inclusion detected.") {}};
    };

    ///handles the line based loading of template files and the inclusion of other files
    template <typename OutputStreamT, typename StringT, typename LogOutputStreamT = CNul >
    class CTemplateLoader : public CTemplateLoaderExceptions
    {
        typedef typename StringT::value_type CharT;
        typedef std::list<StringT> IncludeDirectoryListT;
    public:
        typedef CSourceFile<StringT,TemplateFileT> InputFileT;
        typedef typename InputFileT::InputStreamT InputStreamT;

        ///holds the data of currently processed file
        struct FileData
        {
            bool operator == ( const StringT& aName)
            {
                return name == aName;
            }

            StringT name;
            unsigned int line;
            bool canResolveFileName;
        };

        typedef std::list<FileData> FileDataListT;

        CTemplateLoader()
            : m_outputStream(0)
            , m_logOutputStream(0)
        {
        }

        ///connects the ouput line stream
        void connectOutputStream( OutputStreamT* stream)
        {
            m_outputStream = stream;
        }

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_logOutputStream = stream;
        }

        ///resolve file name
        StringT resolveFileName( const StringT& filename)
        {
            StringT resolvedName = filename;

            if ( !m_openedFiles.empty() && m_openedFiles.back().canResolveFileName)
            {
                resolvedName = FileSystem::determineDependentLocation( m_openedFiles.back().name, filename);
            }

            //if file exists use it, otherwise check in include directories
            if ( !FileSystem::isRegularFile( resolvedName))
            {
                for (const StringT& includeDirectory : m_includeDirectories)
                {
                    resolvedName = FileSystem::determineDependentLocation( includeDirectory, filename, false);
                    if ( FileSystem::isRegularFile( resolvedName))
                    {
                        break; //ok, found a file
                    }
                }
            }

            return resolvedName;
        }

        void loadTemplateStream( InputStreamT& inputStream)
        {
            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Reading template stream.\n";
            }

            //add data for error information
            FileData filedata = { STRING_LITERAL("Input Stream"), 0, true};
            m_openedFiles.push_back( filedata);
            unsigned int& lineNumber = m_openedFiles.back().line;    
        
            //read the stream
            InputFileT::feedLineSink( inputStream, *m_outputStream, true, lineNumber);

            //remove data
            m_openedFiles.pop_back();
        }


        ///reads the template file forwards the data, checks for cyclic inclusion
        void loadTemplateFile( const StringT& filename, bool useCinInstead = false)
        {
            FileData filedata = { useCinInstead ? STRING_LITERAL("stdin") : resolveFileName( filename), 0, !useCinInstead};

            //check if already loading the file
            if ( std::find( m_openedFiles.begin(), m_openedFiles.end(), filedata.name) != m_openedFiles.end())
            {
                throw ExCyclicInclusion();
            }

            //note file name for cyclic inclusion check
            m_openedFiles.push_back( filedata);
            unsigned int& lineNumber = m_openedFiles.back().line;

            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Starting to read template file:\n";
                *m_logOutputStream << "Name=" << filedata.name << "\n";
            }

            //open the file
            InputFileT file( filedata.name, useCinInstead);

            //read file line by line
            file.feedLineSink( *m_outputStream, true, lineNumber);

            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Finished reading of template file:\n";
                *m_logOutputStream << "Name=" << filedata.name << "\n";
            }

            //remove file from check list
            m_openedFiles.erase( std::find( m_openedFiles.begin(), m_openedFiles.end(), filedata.name));
        }

        ///resets the list of currently open files
        void resetInclusionHierarchy()
        {
            m_openedFiles.clear();
        }

        ///reset state
        void reset()
        {
            m_openedFiles.clear();
            m_includeDirectories.clear();
        }

        ///adds an include directory to the list
        void addIncludeDirectory( const StringT& directory)
        {
            m_includeDirectories.push_back( directory);
        }

        ///returns stack of files currently opened
        const FileDataListT& getInclusionHierarchy() const
        {
            return m_openedFiles;
        }

    private:
        OutputStreamT* m_outputStream; ///<data sink
        FileDataListT m_openedFiles; ///<list of currently open files
        IncludeDirectoryListT m_includeDirectories; ///<list
        LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    };
}
