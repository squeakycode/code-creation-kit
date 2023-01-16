//  Copyright (c) 2011-2023 Andreas Gau
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

namespace code_creation_kit
{
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
        void feedLineSink( SinkT& sink, bool /* includeNewLine */)
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
}
