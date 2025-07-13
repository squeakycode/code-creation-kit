// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///tracks the current position in a text file
    class PositionTracker
    {
    public:
        unsigned int getColumn() const
        {
            return m_column;
        }

        unsigned int getLine() const
        {
            return m_line;
        }

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
        unsigned int m_column = 1;
        unsigned int m_line = 1;
    };
}
