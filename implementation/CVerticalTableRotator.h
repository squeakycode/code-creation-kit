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

#ifndef INCLUDED_CVERTICALTABLEROTATOR_H_9937403
#define INCLUDED_CVERTICALTABLEROTATOR_H_9937403

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

///adapter for processing a table from left to right
template <typename TableT>
class CVerticalTableRotator
{
    typedef std::size_t SizeT;
    typedef SizeT IndexT;
public:
    typedef typename TableT::value_type::value_type StringT;

    CVerticalTableRotator( const TableT* table)
        : m_table(table)
    {
    }

    ///represents a column
    class CVerticalTableRotatorColumn
    {
    public:
        CVerticalTableRotatorColumn( const TableT* table, IndexT index)
            : m_table(table)
            , m_index(index)
        {
        }

        const StringT& operator[] ( IndexT index) const
        {
            return (*m_table)[ index ][ m_index ];
        }

        SizeT size() const
        {
            return m_table->size();
        }

        typedef StringT value_type;
    private:
        const TableT* m_table;
        IndexT m_index;
    };

    ///returns a column
    const CVerticalTableRotatorColumn operator[] ( IndexT index) const
    {
        return CVerticalTableRotatorColumn( m_table, index);
    }

    ///returns the size
    SizeT size() const
    {
        if ( !m_table->empty())
        {
            return (*m_table)[0].size();
        }
        return 0;
    }

    bool empty() const
    {
        if ( !m_table->empty())
        {
            return (*m_table)[0].empty();
        }
        return true;
    }

    typedef CVerticalTableRotatorColumn value_type;
private:
    const TableT* m_table; ///<the original table
};

#endif /* INCLUDED_CVERTICALTABLEROTATOR_H_9937403 */
