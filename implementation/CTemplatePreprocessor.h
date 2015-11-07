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
        else if ( token == TokenT::eTrim || token == TokenT::eTrimLeft || token == TokenT::eTrimRight)
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

