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

#ifndef INCLUDED_CLINECOLLECTOR_H_8540613
#define INCLUDED_CLINECOLLECTOR_H_8540613

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include "StringLiteral.h"

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

    ///flushes the last line to the output, returns true if something has beed flushed
    bool close()
    {
        return flush();
    }

    ///flushes the last line to the output, returns true if something has beed flushed
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

#endif /* INCLUDED_CLINECOLLECTOR_H_8540613 */
