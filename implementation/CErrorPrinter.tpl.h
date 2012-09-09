//   Copyright (C) 2011-2012 Andreas Gau
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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#pragma once

#include <boost/format.hpp>

#include "FileSystem.h"
#include <set>
#include "StringLiteral.h"

class CErrorPrinted{};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4512 ) // assignment operator could not be generated
#endif

template <typename StringT, typename GeneratorT>
class CErrorPrinter
{
public:
    typedef boost::basic_format< typename StringT::value_type> FormatT;
    typedef typename StringT::value_type CharT; 

    CErrorPrinter( GeneratorT& generator) : m_generator( generator) {}

    template <typename ParameterListT>
    void generate(
        const StringT& templateFileName,
        const StringT& targetFileName,
        bool useIntermediateFile,
        const StringT& intermediateFileName,
        bool append,
        const ParameterListT& parameters,
        const CInlineTemplateParameters<StringT>& inlineTemplateParameters = CInlineTemplateParameters<StringT>()
    )
    {
        try
        {
            m_generator.generate( 
                templateFileName,
                targetFileName,
                useIntermediateFile,
                intermediateFileName,
                append,
                parameters,
                inlineTemplateParameters);
        }
        [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["generate"][TRIM]
        [INCLUDE]["ErrorPrinterCatch.tpl.h"][TRIM]
        [MACRO_END][TRIM]
    }

    void reset()
    {
        m_generator.reset();
    }

    ///set delimiter for next csv table to load
    void setCsvDelimiter( CharT delimiter)
    {
        try
        {
            m_generator.setCsvDelimiter( delimiter);
        }
        [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["setCsvDelimiter"][TRIM]
        [INCLUDE]["ErrorPrinterCatch.tpl.h"][TRIM]
        [MACRO_END][TRIM]
    }

    ///set list of characters as string that mark commented lines for next csv table to load
    void setCsvCommentChars( const StringT& commentChars)
    {
        try
        {
            m_generator.setCsvCommentChars( commentChars);
        }
        [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["setCsvCommentChars"][TRIM]
        [INCLUDE]["ErrorPrinterCatch.tpl.h"][TRIM]
        [MACRO_END][TRIM]
    }

    void loadTable( const StringT& tableFileName, const StringT& label, bool topDown, bool leftToRight, unsigned int rowHeaderIndex, unsigned int columnHeaderIndex)
    {
        try
        {
            m_generator.loadTable( tableFileName, label, topDown, leftToRight, rowHeaderIndex, columnHeaderIndex);
        }
        [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["loadTable"][TRIM]
        [INCLUDE]["ErrorPrinterCatch.tpl.h"][TRIM]
        [MACRO_END][TRIM]
    }

    void unloadTable( const StringT& label)
    {
        try
        {
            m_generator.unloadTable( label);
        }
        [MACRO_BEGIN][IF][ENTRY]["Operation"][EQUALS]["unloadTable"][TRIM]
        [INCLUDE]["ErrorPrinterCatch.tpl.h"][TRIM]
        [MACRO_END][TRIM]
    }

    ///sets new tag markup
    void setMarkup( const StringT& prefix, const StringT& postfix)
    {
        m_generator.setMarkup( prefix, postfix);
    }

    ///adds an include directory to the list
    void addIncludeDirectory( const StringT& directory)
    {
        m_generator.addIncludeDirectory( directory);
    }

    typedef std::set<StringT> FileSetT;

    ///get list of loaded tables, statistic only
    const FileSetT& getTableFiles() const
    {
        return m_generator.getTableFiles();
    }

    ///get list of generated files, statistic only
    const FileSetT& getGeneratedFiles() const
    {
        return m_generator.getGeneratedFiles();
    }

    ///get list of template files, statistic only
    const FileSetT& getTemplateFiles() const
    {
        return m_generator.getTemplateFiles();
    }

private:
    void toErrorStream( std::string text)
    {
        std::cerr << text;
    }

    void toErrorStream( std::wstring text)
    {
        std::wcerr << text;
    }

    unsigned int getCurrentLineNumber()
    {
        const typename GeneratorT::FileDataListT& list = m_generator.getInclusionHierarchy();
        if ( list.empty())
        {
            return 1;
        }
        return list.back().line;
    }

    StringT getCurrentFileName()
    {
        const typename GeneratorT::FileDataListT& list = m_generator.getInclusionHierarchy();
        if ( list.empty())
        {
            return STRING_LITERAL("???");
        }

        return list.back().name;
    }

    StringT addPath( const StringT& location)
    {
        return FileSystem::determineDependentLocation( location);
    }

    GeneratorT& m_generator;
};

#ifdef _MSC_VER
#pragma warning( pop ) 
#endif
