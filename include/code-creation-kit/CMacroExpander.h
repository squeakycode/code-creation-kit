// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <vector>
#include <string>
#include "StringConvert.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

namespace code_creation_kit
{
    ///defines exceptions thrown by CMacroExpander for template argument independent access
    class CMacroExpanderExceptions
    {
    public:
        template <typename StringT>
        class ExErrorTagExpanded : public std::runtime_error 
        { 
        public: 
            ExErrorTagExpanded( const StringT& message) 
                : std::runtime_error( "An error tag has been triggered.") 
                , m_message( message)
            {
            }
            ~ExErrorTagExpanded()
            {
            }
            const StringT& getMessage()
            {
                return m_message;
            }
        private:
            StringT m_message;
        };
    };

    ///uses the macro data structure, a table index and a table index to create the expanded macro
    template <typename MacroT, typename TableIndexT, typename TableT>
    class CMacroExpander : public CMacroExpanderExceptions
    {
    public:
        typedef CMacroExpander<MacroT,TableIndexT,TableT> MacroExpanderT;
        typedef typename TableT::value_type::value_type StringT;
        typedef typename MacroT::IndexT IndexT;
        typedef typename MacroT::MacroExpressionT MacroExpressionT;
        typedef StringT ExpandedMacroT;
        typedef bool (*ConstraintFunctionT) ( typename MacroT::SubstitutionT&, const StringT&);
        typedef void (*ConversionFunctionT) ( typename MacroT::SubstitutionT&, const StringT&, StringT&);


        CMacroExpander( const MacroT& macro, const TableT& dummy)
            : m_macro(macro)
            , m_table(dummy) //will not be touched as the list of indices is empty
        {
            m_canExpand = true;
            //create internal vector for substitution data
            m_substitutions.resize( macro.getSubstitutions().size());
        }

        CMacroExpander( const TableIndexT& tableIndex, const MacroT& macro, const TableT& table, bool topDown)
            : m_macro(macro)
            , m_table(table)
        {
            m_canExpand = true;
            //create internal vector for substitution data
            m_substitutions.resize( macro.getSubstitutions().size());

            //for each substitution
            for( IndexT i = 0; i < m_substitutions.size(); ++i)
            {
                if ( !macro.getSubstitutions()[ i ].noLookUp())
                {
                    const typename MacroT::SubstitutionT& substitutionData = m_macro.getSubstitutions()[ i ];

                    //get vector of column indices
                    m_substitutions[ i ].indexVector = tableIndex.lookUp( substitutionData.entryName());
                    //check expansion using required reading direction is possible
                    bool topDownSet = substitutionData.topDown();
                    bool leftToRightSet = substitutionData.leftToRight();
                    bool directionOk = (!topDownSet && !leftToRightSet) || (topDownSet && topDown) || (leftToRightSet && !topDown);
                    //check expansion is possible
                    m_canExpand = m_canExpand && (!m_substitutions[ i ].indexVector.empty() || substitutionData.volatil()) && directionOk;
                }
            }
        }

        //return true if all requirements are met
        bool canExpand()
        {
            return m_canExpand;
        }

        ///perform expansion, return true on success
        bool expand( IndexT row, ExpandedMacroT& expandedMacro, IndexT count, bool lastTime)
        {
            expandedMacro.clear();
            if ( !expandExpression( m_macro.getExpression(), row, expandedMacro, count, lastTime))
            {
                //expansion failed clear output
                expandedMacro.clear();
                return false;
            }
            return true;
        }

    private:
        ///performs expansion for the expression tree recursivly
        bool expandExpression( const MacroExpressionT& expression, IndexT row, ExpandedMacroT& expandedMacro, IndexT count, bool lastTime)
        {
            bool ok = false;

            if ( !expression.isLeaf()) //if list of expressions
            {
                //remember starting point
                std::size_t pos = expandedMacro.size();
                if ( expression.isOred() ) //handle ored case
                {
                    for (const MacroExpressionT& subExpression : expression.getSubExpressions())
                    {
                        if ( expandExpression( subExpression, row, expandedMacro, count, lastTime))
                        {
                            ok = true;
                            break;
                        }
                        //failed to expand reset expanded macro
                        expandedMacro.resize( pos);
                    }
                }
                else //handle sequential list of expressions
                {
                    ok = true;
                    for (const MacroExpressionT& subExpression : expression.getSubExpressions())
                    {
                        if ( !expandExpression( subExpression, row, expandedMacro, count, lastTime))
                        {
                            //failed to expand reset expanded macro
                            expandedMacro.resize( pos);
                            ok = false;
                            break;
                        }
                    }
                }
            }
            else
            {
                if ( expression.isTextLeaf())
                {
                    //if text leaf just add the text
                    expandedMacro += expression.getText();
                    ok = true;
                }
                else
                {
                    //handle substitution
                    ok = substitution( expression.getIndex(), row, expandedMacro, count, lastTime);
                }
            }

            return ok;
        }

        ///performs the expansion for a substitution
        bool substitution( IndexT indexOfSubstitution, IndexT row, StringT& expandedMacro, IndexT count, bool lastTime)
        {
            //get substitution data
            const typename MacroT::SubstitutionT& substitutionData = m_macro.getSubstitutions()[ indexOfSubstitution ];
            const Substitution& localSubstitutionData = m_substitutions[ indexOfSubstitution ];

            //variables holding the result of the substitution
            std::vector<StringT> output;
            bool success = false;

            if ( substitutionData == MacroT::SubstitutionT::eError_)
            {
                throw ExErrorTagExpanded<StringT>( substitutionData.errorMessage());
            }
            else if ( substitutionData == MacroT::SubstitutionT::eFirstTime)
            {
                success = (count == 0) != substitutionData.not_();
            }
            else if ( substitutionData == MacroT::SubstitutionT::eLastTime)
            {
                success = lastTime != substitutionData.not_();
            }
            else if (  substitutionData == MacroT::SubstitutionT::eCount)
            {
                StringT result = ToString<StringT>(count + 1);
                success = checkConstraints( substitutionData.getConstraints(), result) != substitutionData.not_();
                if ( !substitutionData.if_())
                {
                    output.push_back( result);
                }
            }
            else if ( substitutionData == MacroT::SubstitutionT::eIndex)
            {
                StringT result = ToString<StringT>(row + 1);
                success = checkConstraints( substitutionData.getConstraints(), result) != substitutionData.not_();
                if ( !substitutionData.if_())
                {
                    output.push_back( result);
                }
            }
            else if ( substitutionData == MacroT::SubstitutionT::eEntry)
            {
                success = expandSubstitution( substitutionData.getConstraints(), localSubstitutionData.indexVector, output, row, substitutionData.if_()) != substitutionData.not_();
            }
            else
            {
                throw std::runtime_error( "Cannot expand unknown substitution.");
            }

            if ( success)
            {
                if ( !output.empty() && !substitutionData.getConversions().empty())
                {
                    const typename MacroT::SubstitutionT::ConversionListT& conversions = substitutionData.getConversions();
                    for (const typename MacroT::SubstitutionT::ConversionListT::value_type& conversion : conversions)
                    {
                        conversion->modify( output);
                    }
                }
                for (const StringT& text : output)
                {
                    expandedMacro += text;
                }
            }
            return success;
        }


        template <typename ConstraintListT>
        bool checkConstraints( const ConstraintListT& constraintList, const StringT& text)
        {
            typedef const typename ConstraintListT::value_type ConstraintT;

            //check if any constraint is matched
            for (ConstraintT& constraint : constraintList)
            {
                if ( constraint->matchesConstraint( text))
                {
                    //constraints are ored
                    return true;
                }
            }

            //default is not equals empty string if no constraints are given
            return constraintList.empty();
        }


        template <typename ConstraintListT, typename IndexVectorT, typename OutputT>
        bool expandSubstitution( const ConstraintListT& constraintList, const IndexVectorT& indexVector, OutputT& output, const IndexT row, bool suppressOutput) const
        {
            typedef const typename ConstraintListT::value_type ConstraintT;
            bool flush = false;

            if ( constraintList.empty())
            {
                //default no constraints, output non empty entries
                output.reserve( indexVector.size());
                for (IndexT column : indexVector)
                {
                    const StringT& entry = m_table[ column ][ row ];
                    if ( !entry.empty())
                    {
                        if ( suppressOutput)
                        {
                            return true;
                        }
                        output.push_back( entry);
                    }
                }
            }
            else
            {
                // check constraints with flush flag first, if one entry meets the constraint the 
                for (ConstraintT& constraint : constraintList)
                {
                    if ( constraint->flush() && !constraint->forAll())
                    {
                        for (IndexT column : indexVector)
                        {
                            if ( constraint->matchesConstraint( m_table[ column ][ row ]))
                            {
                                flush = true;
                                break;
                            }
                        }
                        if ( flush)
                        {
                            break;
                        }
                    }
                }

                // if not already flush everything is set check for all constraints
                if ( !flush)
                {
                    for (ConstraintT& constraint : constraintList)
                    {
                        if ( constraint->forAll())
                        {
                            flush = true;
                            for (IndexT column : indexVector)
                            {
                                if ( !constraint->matchesConstraint( m_table[ column ][ row ]))
                                {
                                    flush = false;
                                    break;
                                }
                            }
                            if ( flush)
                            {
                                break;
                            }
                        }
                    }
                }

                // if not already flush everything is set check constraint for each table entry
                if ( !flush)
                {
                    //try to find a matching constraint for every entry
                    for (IndexT column : indexVector)
                    {
                        const StringT& entry = m_table[ column ][ row ];
                        for (ConstraintT& constraint : constraintList)
                        {
                            if (   !constraint->forAll()
                                && !constraint->flush() 
                                && constraint->matchesConstraint( entry))
                            {
                                if ( suppressOutput)
                                {
                                    return true;
                                }

                                output.push_back( entry);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if ( suppressOutput)
                    {
                        return true;
                    }

                    //flush every entry to the output
                    output.reserve( indexVector.size());
                    for (IndexT column : indexVector)
                    {
                        output.push_back( m_table[ column ][ row ]);
                    }
                }
            }

            return !output.empty();
        }

    private:

        const MacroT& m_macro; ///<data structure representing a macro, created from parsed macro
        const TableT& m_table; ///<the data table with data to insert
        bool m_canExpand; ///<indicates that useful expansion is possible

        //holds local substitution data
        struct Substitution
        {
            typedef typename TableIndexT::IndexVectorT IndexVectorT;
            IndexVectorT indexVector;
        };

        std::vector<Substitution> m_substitutions; ///<local substitution data
    };

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
}
