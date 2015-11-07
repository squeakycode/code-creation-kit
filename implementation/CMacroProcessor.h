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

#include <list>
#include "CTableIndex.h"
#include "CVerticalTableRotator.h"
#include "CMacro.h"
#include "CMacroExpander.h"

///defines exceptions thrown by CMacroProcessor for template argument independent access
class CMacroProcessorExceptions
{
public:
    class ExTableNotFound : public std::runtime_error 
    { public: ExTableNotFound() : std::runtime_error( "A table with the specified label does not exist") {}};

    class ExRowHeaderIndexOutOfBounds : public std::overflow_error 
    { public: ExRowHeaderIndexOutOfBounds() : std::overflow_error( "Row header index exceeds bounds") {}};

    class ExColumnHeaderIndexOutOfBounds : public std::overflow_error 
    { public: ExColumnHeaderIndexOutOfBounds() : std::overflow_error( "Column header index exceeds bounds") {}};
};

///processes macro expression text and ouputs expansion result
template <typename TableT, typename OutputStreamT, typename LogOutputStreamT = CNul >
class CMacroProcessor : public CMacroProcessorExceptions, public boost::noncopyable
{
    //types used:
    typedef typename TableT::value_type::value_type StringT;
    typedef std::size_t SizeT;
    typedef SizeT IndexT;

    typedef CVerticalTableRotator<TableT> TableRotatorT;
    typedef CTableIndex<TableT, ExColumnHeaderIndexOutOfBounds> TableIndexT;
    typedef CTableIndex<TableRotatorT, ExRowHeaderIndexOutOfBounds> RotatedTableIndexT;

    ///represents a table internally
    struct Table
    {
        Table()
            : table(0)
            , rotatedTable(0)
        {
        }

        bool operator == ( const TableT* rhs) const
        {
            return table == rhs;
        }

        bool operator == ( const StringT& rhs) const
        {
            return label == rhs;
        }

        void connectTable( const TableT* connectedTable, const StringT& theLabel, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
        {
            label = theLabel;

            table = topDown ? connectedTable : 0;
            tableIndex.connectTable( table);
            tableIndex.readTable( columnHeaderIndex);

            rotatedTable = leftToRight ? new TableRotatorT( connectedTable) : 0;
            rotatedTableIndex.connectTable( rotatedTable);
            rotatedTableIndex.readTable( rowHeaderIndex);
        }

        ~Table()
        {
            delete rotatedTable;
        }

        const StringT& getLabel() const
        {
            return label;
        }

        TableIndexT tableIndex;
        RotatedTableIndexT rotatedTableIndex;
        const TableT* table;
        TableRotatorT* rotatedTable;
    private:
        StringT label; ///<label identifying the table
    };

    //types used:
    typedef std::list<Table> TableListT;
    typedef CMacro<StringT> MacroT;

public:
    CMacroProcessor()
        : m_outputStream(0)
        , m_logOutputStream(0)
    {
    }

    ///attaches a table, does not take ownership of the table
    void connectTable( const TableT* table, const StringT& label, bool topDown, bool leftRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
    {
        if ( table )
        {
            m_tableList.push_back( Table());
            m_tableList.back().connectTable( table, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex);
        }
    }

    ///detaches a table, returns true if the table is not used by the processor anymore, returns pointer to table removed
    bool disconnectTable( const StringT& label, const TableT*& table /*out*/)
    {
        table = 0;
        typename TableListT::reverse_iterator pos = std::find( m_tableList.rbegin(), m_tableList.rend(), label);
        if ( pos != m_tableList.rend())
        {
            table = pos->table;
            m_tableList.erase( --pos.base());
        }
        else
        {
            throw ExTableNotFound();
        }
        pos = std::find( m_tableList.rbegin(), m_tableList.rend(), table);
        bool tableNotUsedAnymore = pos == m_tableList.rend();
        return tableNotUsedAnymore;
    }

    ///attaches output stream as sink for expanded macros
    void connectOutputStream( OutputStreamT* stream)
    {
        m_outputStream = stream;
    }

    ///connect log output stream
    void connectLogOutputStream( LogOutputStreamT* stream)
    {
        m_logOutputStream = stream;
    }

    ///forward text of a line surrounding a macro, see definition of macro
    CMacroProcessor<TableT, OutputStreamT, LogOutputStreamT>& operator <<( const StringT& text)
    {
        *m_outputStream << text;
        return *this;
    }

    ///process macro and ouput expansion result
    CMacroProcessor<TableT, OutputStreamT, LogOutputStreamT>& operator <<( const MacroT& macro)
    {
        SizeT count = 0;
        StringT expandedMacro;
        StringT expandedLastTime;
        bool lastTime = macro.lastTime();
        bool lastTimeExpanded = false;

        if ( !macro.noLookUp()) //if something to look up in tables
        {
            BOOST_FOREACH( const Table& table, m_tableList)
            {
                if( table.table )
                {
                    //log
                    if ( m_logOutputStream)
                    {
                        *m_logOutputStream << "Start reading table top down:\n";
                        *m_logOutputStream << "Label=" << table.getLabel() << "\n";
                    }
                    processMacro( *table.table, table.tableIndex, macro, true, count, expandedMacro, expandedLastTime, lastTime, lastTimeExpanded);
                }
                if( table.rotatedTable)
                {
                    //log
                    if ( m_logOutputStream)
                    {
                        *m_logOutputStream << "Start reading table left to right:\n";
                        *m_logOutputStream << "Label=" << table.getLabel() << "\n";
                    }
                    processMacro( *table.rotatedTable, table.rotatedTableIndex, macro, false, count, expandedMacro, expandedLastTime, lastTime, lastTimeExpanded);
                }
            }

            if ( count != 0 ) //something has been expanded
            {
                if ( lastTime && lastTimeExpanded ) //last time keyword and expansion ok
                {
                    //log
                    if ( m_logOutputStream)
                    {
                        *m_logOutputStream << "Expanding with last time option (replaces previous expansion):\n";
                        *m_logOutputStream << expandedLastTime << "\n";
                    }
                    if ( !expandedLastTime.empty())
                    {
                        *m_outputStream << expandedLastTime;
                    }
                }
                else if ( !expandedMacro.empty()) //macro expanded or no output any way
                {
                    *m_outputStream << expandedMacro;
                }
            }
        }
        else
        {
            //try to expand to maybe text only alternative
            typedef CMacroExpander< MacroT, TableIndexT, TableT> ExpanderT;
            if ( ExpanderT( macro, TableT()).expand( 0, expandedMacro, count, true))
            {
                if ( !expandedMacro.empty()) //macro expanded
                {
                    //log
                    if ( m_logOutputStream)
                    {
                        *m_logOutputStream << "Expanding:\n";
                        *m_logOutputStream << expandedMacro << "\n";
                    }
                    *m_outputStream << expandedMacro;
                }
            }
        }

        return *this;
    }

    ///reset state
    void reset()
    {
        m_tableList.clear();
    }

private:
    //processes macro expression and ouput expansion result for a table
    template <typename LocalTableT, typename LocalTableIndexT>
    void processMacro( 
        const LocalTableT& table, 
        const LocalTableIndexT& tableIndex, 
        const MacroT macro, 
        bool topDown, 
        SizeT& count,
        StringT& expandedMacro,
        StringT& expandedLastTime,
        bool lastTime,
        bool& lastTimeExpanded
        )
    {
        typedef CMacroExpander< MacroT, LocalTableIndexT, LocalTableT> ExpanderT;
        ExpanderT expander( tableIndex, macro, table, topDown);

        if ( expander.canExpand())
        {
            IndexT max = table[ 0 ].size();
            IndexT lastRow = max;
            StringT expandedMacroLocal;

            for ( IndexT i = tableIndex.getHeaderIndex(); i < max; ++i)
            {
                if ( expander.expand( i, expandedMacroLocal, count, false))
                {
                    //log
                    if ( m_logOutputStream)
                    {
                        *m_logOutputStream << "Expanding at index " << (i + 1) << ":\n";
                        *m_logOutputStream << expandedMacroLocal << "\n";
                    }
                    if ( !expandedMacro.empty())
                    {
                        *m_outputStream << expandedMacro;
                    }
                    expandedMacro.swap( expandedMacroLocal);

                    lastRow = i;
                    count++;
                }
            }
            if ( lastTime && lastRow != max)
            {
                lastTimeExpanded = expander.expand( lastRow, expandedLastTime, count - 1, true);
            }
        }
        else
        {
            //log
            if ( m_logOutputStream)
            {
                *m_logOutputStream << "Entries or reading direction do not match macro." << "\n";
            }
        }
    }

private:
    OutputStreamT* m_outputStream; ///<sink for expanded macros
    LogOutputStreamT* m_logOutputStream; ///< used for logging purposes; NULL if not logging
    TableListT m_tableList; ///<list of attached tables
};

