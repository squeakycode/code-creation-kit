//  Copyright (c) 2011-2015 Andreas Gau
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
            if ( m_file.is_open())
            {
                m_file.close();
            }
            m_file.clear();
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

