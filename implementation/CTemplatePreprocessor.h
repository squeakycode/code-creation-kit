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

#ifndef INCLUDED_CTEMPLATEPREPROCESSOR_H_7384149
#define INCLUDED_CTEMPLATEPREPROCESSOR_H_7384149

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <stdexcept>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4702 )
#endif
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#pragma warning( pop ) 
#endif


///defines exceptions thrown by CTemplatePreprocessor for template argument independent access
class CTemplatePreprocessorExceptions
{
public:
    class ExFileInclusionNotSupported : public std::runtime_error 
    { public: ExFileInclusionNotSupported() : std::runtime_error( "The inclusion of template files is not supported.") {}};

    class ExBadlyPlacedTrim : public std::runtime_error
    { public: ExBadlyPlacedTrim() : std::runtime_error( "Trim directives are expected at the end of a line. Trailing whitespace is allowed.") {}};

    class ExBadlyPlacedComment : public std::runtime_error
    { public: ExBadlyPlacedComment() : std::runtime_error( "COMMENT is expected at the beginning of a line. Leading whitespace is allowed.") {}};

    class ExPrefixLeadingWhiteSpace : public std::runtime_error //collides with TRIM
    { public: ExPrefixLeadingWhiteSpace() : std::runtime_error( "Leading white space for markup prefix not allowed") {}};

    class ExPostfixTrailingWhiteSpace : public std::runtime_error  //collides with TRIM
    { public: ExPostfixTrailingWhiteSpace() : std::runtime_error( "Trailing white space for markup postfix not allowed") {}};
};

///performs preprocessing operations
template <typename PreprocessedStreamT, typename MarkupObserverT, typename TemplateLoaderT, typename TokenT, typename StringT>
class CTemplatePreprocessor : public CTemplatePreprocessorExceptions
{
public:
    CTemplatePreprocessor()
        : m_preprocessedStream(0)
        , m_markupObserver(0)
        , m_templateLoader(0)
    {
    }

    ///perform preprocessing operations
    CTemplatePreprocessor<PreprocessedStreamT, MarkupObserverT, TemplateLoaderT, TokenT, StringT>& operator<<( const TokenT& token)
    {
        if ( token == TokenT::eMarkup )
        {
            //get the parameters
            StringT prefix = token.getStringList()->front();
            StringT postfix = token.getStringList()->back();

            if ( prefix != boost::trim_left_copy( prefix) )
            {
                throw ExPrefixLeadingWhiteSpace();  //collides with TRIM
            }

            if ( postfix != boost::trim_right_copy( postfix) )
            {
                throw ExPostfixTrailingWhiteSpace();  //collides with TRIM
            }

            m_markupObserver->setMarkup( prefix, postfix);
        }
        else if ( token == TokenT::eInclude )
        {
            if ( m_templateLoader)
            {
                //save settings
                StringT prefix = m_prefix;
                StringT postfix = m_postfix;

                //get filename
                StringT filename = token.getStringList()->front();

                //load the included file
                m_templateLoader->loadTemplateFile( filename);

                //loaded file has changed markup
                if ( m_prefix != prefix || m_postfix != postfix )
                {
                    //reset markup
                    m_markupObserver->setMarkup( prefix, postfix);
                }
            }
            else
            {
                throw ExFileInclusionNotSupported();
            }
        }
        else if ( token == TokenT::eTrim || token == TokenT::eTrimLeft )
        {
            throw ExBadlyPlacedTrim();
        }
        else if ( token == TokenT::eComment )
        {
            throw ExBadlyPlacedComment();
        }
        else
        {
            *m_preprocessedStream << token;
        }
        return *this;
    }

    //set markup prefix
    void setPrefix( const StringT& value) { m_prefix = value; }
    //set markup postfix
    void setPostfix( const StringT& value) { m_postfix = value; }

    ///connect receiver of preprocessed stream
    void connectPreprocessedStream( PreprocessedStreamT* stream)
    {
        m_preprocessedStream = stream;
    }

    ///connect template loader for including other files
    void connectTemplateLoader( TemplateLoaderT* loader)
    {
        m_templateLoader = loader;
    }

    ///connect 'keyword list' for updating markup
    void connectMarkupObserver( MarkupObserverT* list)
    {
        m_markupObserver = list;
    }

private:

    StringT m_prefix;
    StringT m_postfix;

    PreprocessedStreamT* m_preprocessedStream;
    MarkupObserverT* m_markupObserver;
    TemplateLoaderT* m_templateLoader;
};

#endif /* INCLUDED_CTEMPLATEPREPROCESSOR_H_7384149 */
