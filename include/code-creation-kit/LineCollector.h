// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///takes text snippets and produces linewise output
    template <typename StringT, typename OutputStreamT>
    class LineCollector
    {
    public:
        typedef typename StringT::value_type CharT;

        LineCollector()
            : m_outputStream(0)
        {
        }

        ///attaches an output stream
        void connectOutputStream( OutputStreamT* stream)
        {
            m_outputStream = stream;
        }

        ///resets the collector for next input stream, added for symmetry to close
        void open()
        {
            reset();
        }

        ///collects text fragments and creates a linewise output from the fragments
        template <typename RangeT>
        LineCollector<StringT, OutputStreamT>& operator <<( const RangeT& text)
        {
            for ( typename RangeT::const_iterator it = text.begin(); it != text.end(); ++it)
            {
                m_line += *it;
                if ( *it == '\n')
                {
                    *m_outputStream << m_line;
                    m_line.clear();
                }
            }

            return *this;
        }

        ///return true if currently processing a macro
        bool processingInProgress()
        {
            return !m_line.empty();
        }

        ///flushes the last line to the output, returns true if something has been flushed
        bool close()
        {
            return flush();
        }

        ///flushes the last line to the output, returns true if something has been flushed
        bool flush()
        {
            if ( !m_line.empty())
            {
                *m_outputStream << m_line;
                m_line.clear();
                return true;
            }
            return false;
        }

        ///resets the text internal buffer
        void reset()
        {
            m_line.clear();
        }

    private:
        StringT m_line; ///<buffers incomplete lines
        OutputStreamT* m_outputStream; ///<the output stream
    };
}
