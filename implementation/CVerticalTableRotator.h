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

namespace code_creation_kit
{
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
}
