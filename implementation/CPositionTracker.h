//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_CPOSITIONTRACKER_H_8456385
#define INCLUDED_CPOSITIONTRACKER_H_8456385

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

///tracks the current position in a text file
class CPositionTracker
{
public:
    CPositionTracker() : m_column(1), m_line(1) {}

    unsigned int getColumn() { return m_column;}
    unsigned int getLine() { return m_line;}

    void reset()
    {
        m_line = 1;
        m_column = 1;
    }

    void nextLine()
    {
        m_line++;
        m_column = 1;
    }

    void nextColumn()
    {
        m_column++;
    }

private:
    unsigned int m_column;
    unsigned int m_line;
};

#endif /* INCLUDED_CPOSITIONTRACKER_H_8456385 */
