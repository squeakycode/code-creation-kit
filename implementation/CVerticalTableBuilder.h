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

#include <stdexcept>
#include <assert.h>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

namespace code_creation_kit
{
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
}
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
