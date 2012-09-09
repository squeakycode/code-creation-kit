//   Copyright (C) 2011-2012 Andreas Gau
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

#ifndef INCLUDED_CTARGETFILE_H_0496332
#define INCLUDED_CTARGETFILE_H_0496332

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <fstream>
#include <iostream>
#include <stdexcept>

///defines exceptions thrown by CTargetFile(s) for template argument independent access
template <typename TargetFileIdentifyingT>
class CTargetFileExceptions
{
public:
    class ExCannotOpenFile : public std::runtime_error 
    { public: ExCannotOpenFile() : std::runtime_error( "Failed to open output file.") {}};

    class ExCannotWriteToFile : public std::runtime_error 
    { public: ExCannotWriteToFile() : std::runtime_error( "Failed to write to output file.") {}};
};

///encapsulates often used input stream handling
template <typename StringT, typename TargetFileIdentifyingT>
class CTargetFile : public CTargetFileExceptions<TargetFileIdentifyingT>
{
public:
    typedef typename StringT::value_type CharT;
    typedef std::basic_ostream<CharT, std::char_traits<CharT> > OutputStreamT;
    typedef std::basic_ofstream< CharT, std::char_traits<CharT> > OutputFileStreamT;
    typedef CTargetFile<StringT,TargetFileIdentifyingT> ThisT;

    CTargetFile()
        : m_useCout( false)
    {
    }

    CTargetFile( const StringT& filename, bool useCoutInstead = false, bool append = false)
        : m_useCout( useCoutInstead)
    {
        open( filename, useCoutInstead, append);
    }

    ///open file and check
    void open( const StringT& filename, bool useCoutInstead = false, bool append = false)
    {
        //file mode
        m_useCout = useCoutInstead;

        if ( !m_useCout)
        {
            //open file
            m_file.open( filename.c_str(), append ? (std::ios::out | std::ios::app) : (std::ios::out | std::ios::trunc));
            if ( !m_file.is_open())
            {
                throw typename ThisT::ExCannotOpenFile();
            }
        }
    }

    ///returns the selected standard stream
    OutputStreamT& get()
    {
        if ( m_useCout)
        {
            return getCout((CharT*)0);
        }
        else
        {
            return m_file;
        }
    }

    ///write text to stream and check ok
    CTargetFile<StringT,TargetFileIdentifyingT> operator << ( const StringT& text)
    {
        if ( !get() << text )
        {
            throw typename ThisT::ExCannotWriteToFile();
        }
        return *this;
    }

    ///check file ok
    void checkGood()    
    {
        if ( !get() )
        {
            throw typename ThisT::ExCannotWriteToFile();
        }
    }

    ///close if file is used
    void close()
    {
        if ( !m_useCout)
        {
            m_file.close();
        }
    }

private:

    ///helper function for getting the right output stream
    std::basic_ostream<char, std::char_traits<char> >& getCout(char*)
    {
        return std::cout;
    }

    ///helper function for getting the right output stream
    std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& getCout(wchar_t*)
    {
        return std::wcout;
    }

    bool m_useCout;
    OutputFileStreamT m_file;
};

#endif /* INCLUDED_CTARGETFILE_H_0496332 */
