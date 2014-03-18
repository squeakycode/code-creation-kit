//   Copyright (C) 2011-2014 Andreas Gau
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

#ifndef INCLUDED_CVERTICALTABLEBUILDER_H_290188
#define INCLUDED_CVERTICALTABLEBUILDER_H_290188

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <stdexcept>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

///defines exceptions thrown by CVerticalTableBuilder for template argument independent access
class CVerticalTableBuilderExceptions
{
public:
    class ExOverflow : public std::overflow_error 
    { public: ExOverflow() : std::overflow_error( "Row contains more items than first row.") {}};

    class ExUnderflow : public std::underflow_error 
    { public: ExUnderflow() : std::underflow_error( "Row contains less items than first row.") {}};
};

///Supports adding table items to a container and checks for over- and underflows
template <typename VerticalContainerT>
class CVerticalTableBuilder : public CVerticalTableBuilderExceptions
{
public:
    typedef typename VerticalContainerT::value_type::value_type StringT;
    typedef typename VerticalContainerT::value_type ColumnT;

    CVerticalTableBuilder( VerticalContainerT& container, bool pad = false) 
        : m_container( container)
        , m_pad(pad)
        , m_currentRowIndex(0)
    {
        container.clear();
    }

    ///adds an item and advances the writing head, checks for overflows in a row
    void addItem( const StringT& item)
    {
        if ( isFirstRow())
        {
            //add column
            m_container.push_back( ColumnT());
            //add item
            m_container.back().push_back(item);
        }
        else
        {
            if ( m_iterator == m_container.end() )
            {
                if ( m_pad)
                {
                    //add column
                    m_container.push_back( ColumnT());
                    //fill up with empty items
                    m_container.back().resize( m_currentRowIndex);
                    //add item
                    m_container.back().push_back( item);
                    //update iterator
                    m_iterator = m_container.end();
                }
                else
                {
                    throw ExOverflow();
                }
            }
            else
            {
                m_iterator->push_back( item);
                ++m_iterator;
            }
        }   
    }

    ///resets the writing head to first column, checks for underflows in a row
    void addRow()
    {
        if ( !isFirstRow())
        {
            if ( m_iterator != m_container.end())
            {
                if ( m_pad)
                {
                    for (;m_iterator != m_container.end(); ++m_iterator)
                    {
                        m_iterator->push_back( StringT());
                    }
                }
                else
                {
                    throw ExUnderflow();
                }
            }
        }
        ++m_currentRowIndex;
        m_iterator = m_container.begin();
    }

    ///checks for underflows in the last row
    void finished()
    {
        if ( !isFirstRow())
        {
            //is partial filled row?
            if ( m_iterator != m_container.begin() && m_iterator != m_container.end())
            {
                if ( m_pad)
                {
                    //pad if needed
                    addRow();
                }
                else
                {
                    throw ExUnderflow();
                }
            }
        }
    }

private:

    ///used for over- and underflowchecks when not in first row
    bool isFirstRow()
    {
        return m_currentRowIndex == 0;
    }

    VerticalContainerT& m_container; ///<reference to the container to be filled
    bool m_pad; ///<auto pad rows when rows are too short
    size_t m_currentRowIndex; ///< index of the current row
    typename VerticalContainerT::iterator m_iterator; ///<the writing head
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif

#endif /* INCLUDED_CVERTICALTABLEBUILDER_H_290188 */
