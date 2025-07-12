// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <list>
#include <memory.h>
#include "CTableIndex.h"
#include "CVerticalTableRotator.h"
#include "CMacro.h"
#include "CMacroExpander.h"

namespace code_creation_kit
{
    ///defines exceptions thrown by CMacroProcessor for template argument independent access
    class CMacroProcessorExceptions
    {
    public:
        class ExTableNotFound : public std::runtime_error 
        { public: ExTableNotFound() : std::runtime_error( "A table with the specified label does not exist") {}};
        
        class ExTableLabelAlreadyDefined : public std::runtime_error
        {
        public: ExTableLabelAlreadyDefined() : std::runtime_error("Table label already defined. Cannot add a second table with the same label.") {}
        };

        class ExCannotChangeTableList : public std::runtime_error
        {
        public: ExCannotChangeTableList() : std::runtime_error("Changing the table list permanently is disabled. Only temporary tables can be added or removed.") {}
        };

        class ExRowHeaderIndexOutOfBounds : public std::overflow_error
        { public: ExRowHeaderIndexOutOfBounds() : std::overflow_error( "Row header index exceeds bounds") {}};

        class ExColumnHeaderIndexOutOfBounds : public std::overflow_error 
        { public: ExColumnHeaderIndexOutOfBounds() : std::overflow_error( "Column header index exceeds bounds") {}};
    };

    ///processes macro expression text and ouputs expansion result
    template <typename TableT, typename OutputStreamT, typename LogOutputStreamT = CNul >
    class CMacroProcessor : public CMacroProcessorExceptions
    {
        //types used:
        typedef typename TableT::value_type::value_type StringT;
        typedef std::size_t SizeT;
        typedef SizeT IndexT;

        typedef CVerticalTableRotator<TableT> TableRotatorT;
        typedef CTableIndex<TableT, ExColumnHeaderIndexOutOfBounds> TableIndexT;
        typedef CTableIndex<TableRotatorT, ExRowHeaderIndexOutOfBounds> RotatedTableIndexT;
        typedef std::shared_ptr<const TableT> SharedConstTableT;
        typedef std::shared_ptr<TableRotatorT> SharedTableRotatorT;

        ///represents a table internally
        struct TableData
        {
            TableData()
                : isTemporaryFlag(false)
            {
            }

            bool operator == ( const SharedConstTableT& ptrRhs) const
            {
                return ptrTable == ptrRhs;
            }

            bool operator == ( const StringT& rhs) const
            {
                return label == rhs;
            }

            void connectTable(
                const SharedConstTableT& ptrConnectedTable,
                const StringT& theLabel,
                bool topDown,
                bool leftToRight,
                unsigned int rowHeaderIndex,
                unsigned int columnHeaderIndex,
                bool isTemporary
            )
            {
                label = theLabel;
                isTemporaryFlag = isTemporary;

                ptrTable = topDown ? ptrConnectedTable : SharedConstTableT();
                tableIndex.connectTable( ptrTable.get());
                tableIndex.readTable( columnHeaderIndex);

                if (!ptrTable)
                {
                    ptrSourceTable = ptrConnectedTable; //keep holding a reference to the table
                }

                ptrRotatedTable = leftToRight ? std::make_shared<TableRotatorT>( ptrConnectedTable.get()) : SharedTableRotatorT();
                rotatedTableIndex.connectTable( ptrRotatedTable.get());
                rotatedTableIndex.readTable( rowHeaderIndex);
            }

            ~TableData() = default;

            const StringT& getLabel() const
            {
                return label;
            }
            
            [[nodiscard]] bool isTemporary() const
            {
                return isTemporaryFlag;
            }

            SharedConstTableT ptrTable;
            SharedTableRotatorT ptrRotatedTable;
            TableIndexT tableIndex;
            RotatedTableIndexT rotatedTableIndex;
        private:
            SharedConstTableT ptrSourceTable; //ptrTable can not be set if reading only left to right so we must keep holding a reference to the table here
            bool isTemporaryFlag;
            StringT label; ///<label identifying the table
        };

        //types used:
        typedef std::list<TableData> TableListT; ///<tables are processed in the order they are connected
        typedef CMacro<StringT> MacroT;

    public:
        CMacroProcessor()
            : m_outputStream(nullptr)
            , m_pLogOutputStream(nullptr)
            , m_canChangeNonTemporaryTableList(true)
        {
        }

        CMacroProcessor(CMacroProcessor& rhs) = delete;
        CMacroProcessor& operator=(CMacroProcessor& rhs) = delete;

        ///this allows controlling the changes made by a template
        void setCanChangeNonTemporaryTableList(bool canChangeNonTemporaryTableList)
        {
            m_canChangeNonTemporaryTableList = canChangeNonTemporaryTableList;
        }

        ///attaches a table, does not take ownership of the table
        void connectTable(
            const SharedConstTableT& ptrConnectedTable,
            const StringT& label,
            bool topDown,
            bool leftRight,
            unsigned int rowHeaderIndex,
            unsigned int columnHeaderIndex,
            bool isTemporary //provided temporarily during processing
        )
        {
            //log that we are about to add the table with the given label
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << "Adding table with label: " << label << "\n";
            }

            //block any declaring of tables added permanently
            if (!m_canChangeNonTemporaryTableList && !isTemporary)
            {
                throw ExCannotChangeTableList();
            }

            typename TableListT::reverse_iterator pos = std::find( m_tableList.rbegin(), m_tableList.rend(), label);
            if (pos != m_tableList.rend())
            {
                throw ExTableLabelAlreadyDefined();
            }
            
            if (ptrConnectedTable)
            {
                m_tableList.push_back(TableData());
                m_tableList.back().connectTable( ptrConnectedTable, label, topDown, leftRight, rowHeaderIndex, columnHeaderIndex, isTemporary);
            }
        }

        ///detaches a table
        bool disconnectTable(const StringT& label, bool throwIfNotFound, SharedConstTableT& ptrDisconnectedTable)
        {
            bool result = false;
            ptrDisconnectedTable.reset();
            typename TableListT::reverse_iterator pos = std::find( m_tableList.rbegin(), m_tableList.rend(), label);
            if ( pos != m_tableList.rend())
            {
                //log that we are about to remove the table with the given label
                if (m_pLogOutputStream)
                {
                    *m_pLogOutputStream << "Removing table with label: " << label << "\n";
                }

                //block any removing of tables added permanently
                if (!m_canChangeNonTemporaryTableList && !pos->isTemporary())
                {
                    throw ExCannotChangeTableList();
                }

                ptrDisconnectedTable = pos->ptrTable;
                m_tableList.erase( --pos.base());
                result = true;
            }
            else if (throwIfNotFound)
            {
                throw ExTableNotFound();
            }
            else
            {
                //if the table is not found consider it removed.
                if (m_pLogOutputStream)
                {
                    *m_pLogOutputStream << "Template-provided table to remove not found with label: " << label << "\n";
                }
            }
            return result;
        }
        
        ///removed tables provided temporarily during processing
        void removeTemporaryTables()
        {
            for (auto it = m_tableList.begin(); it != m_tableList.end();)
            {
                if (it->isTemporary())
                {
                    //log that we are about to remove the table with the given label
                    if (m_pLogOutputStream)
                    {
                        *m_pLogOutputStream << "Removing table with label: " << it->getLabel() << "\n";
                    }
                    it = m_tableList.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        
        ///attaches output stream as sink for expanded macros
        void connectOutputStream( OutputStreamT* stream)
        {
            m_outputStream = stream;
        }

        ///connect log output stream
        void connectLogOutputStream( LogOutputStreamT* stream)
        {
            m_pLogOutputStream = stream;
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
                for (const TableData& listTableEntry : m_tableList)
                {
                    if( listTableEntry.ptrTable )
                    {
                        //log
                        if ( m_pLogOutputStream)
                        {
                            *m_pLogOutputStream << "Start reading table top down:\n";
                            *m_pLogOutputStream << "Label=" << listTableEntry.getLabel() << "\n";
                        }
                        processMacro( *listTableEntry.ptrTable, listTableEntry.tableIndex, macro, true, count, expandedMacro, expandedLastTime, lastTime, lastTimeExpanded);
                    }
                    if( listTableEntry.ptrRotatedTable)
                    {
                        //log
                        if ( m_pLogOutputStream)
                        {
                            *m_pLogOutputStream << "Start reading table left to right:\n";
                            *m_pLogOutputStream << "Label=" << listTableEntry.getLabel() << "\n";
                        }
                        processMacro( *listTableEntry.ptrRotatedTable, listTableEntry.rotatedTableIndex, macro, false, count, expandedMacro, expandedLastTime, lastTime, lastTimeExpanded);
                    }
                }

                if ( count != 0 ) //something has been expanded
                {
                    if ( lastTime && lastTimeExpanded ) //last time keyword and expansion ok
                    {
                        //log
                        if ( m_pLogOutputStream)
                        {
                            *m_pLogOutputStream << "Expanding with last time option (replaces previous expansion):\n";
                            *m_pLogOutputStream << expandedLastTime << "\n";
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
                        if ( m_pLogOutputStream)
                        {
                            *m_pLogOutputStream << "Expanding:\n";
                            *m_pLogOutputStream << expandedMacro << "\n";
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
            //log
            if (m_pLogOutputStream)
            {
                *m_pLogOutputStream << "Clearing table list.\n";
            }

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
                        if ( m_pLogOutputStream)
                        {
                            *m_pLogOutputStream << "Expanding at index " << (i + 1) << ":\n";
                            *m_pLogOutputStream << expandedMacroLocal << "\n";
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
                if ( m_pLogOutputStream)
                {
                    *m_pLogOutputStream << "Entries or reading direction do not match macro." << "\n";
                }
            }
        }

    private:
        OutputStreamT* m_outputStream; ///<sink for expanded macros
        LogOutputStreamT* m_pLogOutputStream; ///< used for logging purposes; NULL if not logging
        TableListT m_tableList; ///<list of attached tables
        bool m_canChangeNonTemporaryTableList;
    };
}
