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

#ifndef INCLUDED_CTEMPLATELOADER_H_3935205
#define INCLUDED_CTEMPLATELOADER_H_3935205

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <list>
#include <stdexcept>
#include "FileSystem.h"
#include "CSourceFile.h"
#include <boost/foreach.hpp>

class TemplateFileT;

///Holds exceptions thrown by CTemplateLoader for template argument independent access
class CTemplateLoaderExceptions
{
public:
    class ExCyclicInclusion : public std::runtime_error 
    { public: ExCyclicInclusion() : std::runtime_error( "Cyclic inclusion detected.") {}};
};

///handles the line based loading of template files and the inclusion of other files
template <typename OutputStreamT, typename StringT>
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
    {
    }

    ///connects the ouput line stream
    void connectOutputStream( OutputStreamT* stream)
    {
        m_outputStream = stream;
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
            BOOST_FOREACH( const StringT& includeDirectory, m_includeDirectories)
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

        //open the file
        InputFileT file( filedata.name, useCinInstead);

        //read file line by line
        file.feedLineSink( *m_outputStream, true, lineNumber);

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
    FileDataListT m_openedFiles; ///list of currently open files
    IncludeDirectoryListT m_includeDirectories; ///list
};

#endif /* INCLUDED_CTEMPLATELOADER_H_3935205 */
