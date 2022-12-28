//  Copyright (c) 2011-2019 Andreas Gau
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

#include <vector>
#include <map>
#include "StringConvert.h"

namespace code_creation_kit
{
    ///creates an index of all columns with the same name
    template <typename TableT, typename IndexOutOfBoundsExceptionT>
    class CTableIndex
    {
    public:
        typedef typename TableT::value_type::value_type StringT;
        typedef std::size_t SizeT;
        typedef SizeT IndexT;
        typedef std::vector<IndexT> IndexVectorT;

        CTableIndex()
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
