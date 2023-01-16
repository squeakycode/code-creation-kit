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

#include "StringLiteral.h"

namespace code_creation_kit
{
    ///takes text snippets and produces linewise output
    template <typename StringT, typename OutputStreamT>
    class CLineCollector
    {
    public:
        typedef typename StringT::value_type CharT;

        CLineCollector()
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
        CLineCollector<StringT, OutputStreamT>& operator <<( const RangeT& text)
        {
            for ( typename RangeT::const_iterator it = text.begin(); it != text.end(); ++it)
            {
                m_line += *it;
                if ( *it == STRING_LITERAL('\n'))
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
