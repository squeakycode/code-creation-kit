// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    ///adapter for processing a table from left to right
    template <typename TableT>
    class VerticalTableRotator
    {
        typedef std::size_t SizeT;
        typedef SizeT IndexT;
    public:
        typedef typename TableT::value_type::value_type StringT;

        explicit VerticalTableRotator( const TableT* table)
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

            [[nodiscard]] SizeT size() const
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
        [[nodiscard]] SizeT size() const
        {
            if ( !m_table->empty())
            {
                return (*m_table)[0].size();
            }
            return 0;
        }

        [[nodiscard]] bool empty() const
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
}
