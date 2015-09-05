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

#ifndef INCLUDED_CTABLEINDEX_H_04827517
#define INCLUDED_CTABLEINDEX_H_04827517

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4702 ) //warning C4702: unreachable code
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#include <vector>
#include <map>

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
                m_itemToIndexMap[ boost::lexical_cast<StringT>( i + 1 ) ].push_back( i );
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

#endif /* INCLUDED_CTABLEINDEX_H_04827517 */
