// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace code_creation_kit
{
    ///defines exceptions thrown by SourceFile(s) for template argument independent access
    template <typename SourceFileIdentifyingT>
    class SourceFileExceptions
    {
    public:
        class ExCannotOpenFile : public std::runtime_error 
        { public: ExCannotOpenFile() : std::runtime_error( "Failed to open input file.") {}};

        class ExCannotReadFile : public std::runtime_error 
        { public: ExCannotReadFile() : std::runtime_error( "Failed to read from input file.") {}};
    };

    ///encapsulates often used input stream handling
    template <typename StringT, typename SourceFileIdentifyingT>
    class SourceFile : public SourceFileExceptions<SourceFileIdentifyingT>
    {
    public:
        typedef typename StringT::value_type CharT;
        typedef std::basic_istream<CharT, std::char_traits<CharT> > InputStreamT;
        typedef std::basic_ifstream< CharT, std::char_traits<CharT> > InputFileStreamT;
        typedef SourceFile<StringT,SourceFileIdentifyingT> ThisT;

        SourceFile()
            : m_useCin( false)
        {
        }

        explicit SourceFile( const StringT& filename, bool useCinInstead = false)
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
                feedLineSink( getCin(static_cast<const CharT*>(nullptr)), sink, includeNewLine, counter);
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
                return getCin(static_cast<const CharT*>(nullptr));
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
                    line += '\n';
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
        static std::basic_istream<char, std::char_traits<char> >& getCin(const char*)
        {
            return std::cin;
        }

        ///helper function for getting the right input stream
        static std::basic_istream<wchar_t, std::char_traits<wchar_t> >& getCin(const wchar_t*)
        {
            return std::wcin;
        }

        bool m_useCin;
        InputFileStreamT m_file;
    };
}
