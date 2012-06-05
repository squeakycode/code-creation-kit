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

#ifndef INCLUDED_CSOURCEFILE_H_9569666
#define INCLUDED_CSOURCEFILE_H_9569666

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <fstream>
#include <iostream>
#include <stdexcept>

///defines exceptions thrown by CSourceFile(s) for template argument independent access
template <typename SourceFileIdentifyingT>
class CSourceFileExceptions
{
public:
    class ExCannotOpenFile : public std::runtime_error 
    { public: ExCannotOpenFile() : std::runtime_error( "Failed to open input file.") {}};

    class ExCannotReadFile : public std::runtime_error 
    { public: ExCannotReadFile() : std::runtime_error( "Failed to read from input file.") {}};
};

///encapsulates often used input stream handling
template <typename StringT, typename SourceFileIdentifyingT>
class CSourceFile : public CSourceFileExceptions<SourceFileIdentifyingT>
{
public:
    typedef typename StringT::value_type CharT;
    typedef std::basic_istream<CharT, std::char_traits<CharT> > InputStreamT;
    typedef std::basic_ifstream< CharT, std::char_traits<CharT> > InputFileStreamT;
    typedef CSourceFile<StringT,SourceFileIdentifyingT> ThisT;

    CSourceFile()
        : m_useCin( false)
    {
    }

    CSourceFile( const StringT& filename, bool useCinInstead = false)
        : m_useCin( useCinInstead)
    {
        if ( !m_useCin )
        {
            open( filename);
        }
    }

    ///open file and check
    void open( const StringT& filename)
    {
        //file mode
        m_useCin = false;

        //open file
        m_file.open( filename.c_str());
        if ( !m_file.is_open())
        {
            throw typename ThisT::ExCannotOpenFile();
        }
    }

    ///read file line by line
    template <typename SinkT>
    void feedLineSink( SinkT& sink, bool includeNewLine)
    {
        int dummyCounter = 0;
        feedLineSink( sink, dummyCounter);
    }

    ///read file line by line and count
    template <typename SinkT, typename CounterT>
    void feedLineSink( SinkT& sink, bool includeNewLine, CounterT& counter)
    {
        if ( m_useCin)
        {
            feedLineSink( getCin((CharT*)0), sink, includeNewLine, counter);
        }
        else
        {
            feedLineSink( m_file, sink, includeNewLine, counter);
        }
    }

    ///returns the selected standard stream
    InputStreamT& get()
    {
        if ( m_useCin)
        {
            return getCin((CharT*)0);
        }
        else
        {
            return m_file;
        }
    }

    ///check if read was ended by end of file, otherwise an error occured
    void checkEofReached()
    {
        checkEofReached( get());
    }

    ///read stream line by line implementation
    template <typename StreamT, typename SinkT, typename CounterT>
    static void feedLineSink( StreamT& stream, SinkT& sink, bool includeNewLine, CounterT& counter)
    {
        //read file line by line
        StringT line;
        while( std::getline( stream, line))
        {
            if ( !stream.eof() && includeNewLine)
            {
                line += stream.widen('\n');
            }

            ++counter;
            sink << line;
        }

        checkEofReached( stream);
    }

private:
    ///check if read was ended by end of file, otherwise an error occured
    template <typename StreamT>
    static void checkEofReached( StreamT& stream)
    {
        if ( stream.bad() || (stream.fail() && !stream.eof()) )
        {
            throw typename ThisT::ExCannotReadFile();
        }
    }

    ///helper function for getting the right input stream
    std::basic_istream<char, std::char_traits<char> >& getCin(char*)
    {
        return std::cin;
    }

    ///helper function for getting the right input stream
    std::basic_istream<wchar_t, std::char_traits<wchar_t> >& getCin(wchar_t*)
    {
        return std::wcin;
    }

    bool m_useCin;
    InputFileStreamT m_file;
};

#endif /* INCLUDED_CSOURCEFILE_H_9569666 */
