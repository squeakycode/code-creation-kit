// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <vector>
#include <map>
#include "StringConvert.h"

namespace code_creation_kit
{
    ///creates an index of all columns with the same name
    template <typename TableT, typename IndexOutOfBoundsExceptionT>
    class TableIndex
    {
    public:
        typedef typename TableT::value_type::value_type StringT;
        typedef std::size_t SizeT;
        typedef SizeT IndexT;
        typedef std::vector<IndexT> IndexVectorT;

        TableIndex()
            : m_table(0)
            , m_headerIndex(0)
        {
        }

        ///connect a table
        void connectTable( const TableT* table)
        {
            m_table = table;
        }

        ///create the index
        void readTable( unsigned int headerIndex)
        {
            m_itemToIndexMap.clear(); //clear the index

            if ( !m_table ) //null is allowed, same as empty table
            {
                return;
            }

            //check index out of bounds, > used due to one based index
            if ( !m_table->empty() && headerIndex > (*m_table)[0].size())
            {
                throw IndexOutOfBoundsExceptionT();
            }

            //save header index
            m_headerIndex = headerIndex;

            //if header index set build index
            if ( headerIndex)
            {
                unsigned int zeroBasedHeaderIndex = headerIndex - 1;
                for( IndexT i = 0; i < (*m_table).size(); ++i)
                {
                    m_itemToIndexMap[ (*m_table)[ i ][ zeroBasedHeaderIndex ] ].push_back( i );
                }
            }
            else //build index with one based index numbers
            {
                for( IndexT i = 0; i < (*m_table).size(); ++i)
                {
                    m_itemToIndexMap[ ToString<StringT>( i + 1 ) ].push_back( i );
                }
            }
        }

        ///lookup the indices of all columns with the provided name
        const IndexVectorT& lookUp( const StringT& name) const
        {
            typename ItemToIndexMapT::const_iterator pos = m_itemToIndexMap.find( name);
            if ( pos != m_itemToIndexMap.end() )
            {
                return pos->second;
            }
            return m_dummyIndexVector;
        }

        ///provides the connected table
        const TableT* getTable() const
        {
            return m_table;
        }

        ///provides the used header index, can be used for start position of reading
        unsigned int getHeaderIndex() const
        {
            return m_headerIndex;
        }
    private:
        const TableT* m_table; ///<the table
        unsigned int m_headerIndex; ///index of column or row containing the names

        typedef std::map<StringT, IndexVectorT> ItemToIndexMapT; ///<index data structure
        ItemToIndexMapT m_itemToIndexMap; ///the index
        IndexVectorT m_dummyIndexVector; ///used when lookup failes
    };
}
