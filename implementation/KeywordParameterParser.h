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

#include "StringLiteral.h"
#include <stdexcept>
#include <vector>

///base functionality for parameter policy classes, that define the parsing of a parameter value
class CParameterPolicyBase
{
protected:
    struct Chars
    {
        static const char opening_par = '[';
        static const char closing_par = ']';
        static const char comma = ',';
        static const char space = ' ';
        static const char tab = '\t';
        static const char new_line = '\n';
        static const char double_quote = '"';
        static const char single_quote = '\'';
        static const char back_slash = '\\';
        static const char t = 't';
        static const char n = 'n';
        static const char digit0 = '0';
        static const char digit9 = '9';
        static const char plus = '+';
    };

    struct LChars
    {
        static const wchar_t opening_par = L'[';
        static const wchar_t closing_par = L']';
        static const wchar_t comma = L',';
        static const wchar_t space = L' ';
        static const wchar_t tab = L'\t';
        static const wchar_t new_line = L'\n';
        static const wchar_t double_quote = L'"';
        static const wchar_t single_quote = L'\'';
        static const wchar_t back_slash = L'\\';
        static const wchar_t t = L't';
        static const wchar_t n = L'n';
        static const wchar_t digit0 = L'0';
        static const wchar_t digit9 = L'9';
        static const wchar_t plus = '+';
    };

    ///skip spaces and tabs
    template <typename IteratorT>
    static void skipWhiteSpace( IteratorT& start, const IteratorT& end)
    {
        typedef typename IteratorT::value_type CharT;
        while( start != end && (*start == STRING_LITERAL( Chars::space) || *start == STRING_LITERAL( Chars::tab)))
        {
            ++start;
        }
    }

public:

    ///match parameter expression start
    template <typename ExceptionT, typename IteratorT>
    static bool parseParameterStart( IteratorT& start, const IteratorT& end, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        if ( start != end && *start == STRING_LITERAL( Chars::opening_par) )
        {
            ++start;
            skipWhiteSpace( start, end);
            return true;
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }

    ///match parameter expression end
    template <typename ExceptionT, typename IteratorT>
    static bool parseParameterEnd( IteratorT& start, const IteratorT& end, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        skipWhiteSpace( start, end);
        if ( start != end && *start == STRING_LITERAL( Chars::closing_par) )
        {
            ++start;
            return true;
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }

    ///match parameter expression separator
    template <typename ExceptionT, typename IteratorT>
    static bool parseParameterSeparator( IteratorT& start, const IteratorT& end, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        skipWhiteSpace( start, end);
        if ( start != end && *start == STRING_LITERAL( Chars::comma) )
        {
            ++start;
            skipWhiteSpace( start, end);
            return true;
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }
};

///for parameters without special characters and without double quotes
class CPlainParameterPolicy : public CParameterPolicyBase
{
public:
    ///match parameter expression value and extract it
    template <typename ExceptionT, typename IteratorT, typename StringT>
    static bool parseParameterValue( IteratorT& start, const IteratorT& end, StringT& value, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        value.clear();    
        if ( start != end && *start == STRING_LITERAL( Chars::double_quote) )  //start with "
        {
            while ( ++start != end )
            {
                if ( *start == STRING_LITERAL( Chars::double_quote) )  //end with "
                {
                    ++start;
                    return true;
                }
                value += *start; //add char to output value string
            }
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }
};

///for usigned number parameters
class CUIntParameterPolicy : public CParameterPolicyBase
{
public:
    ///match parameter expression value and extract it
    template <typename ExceptionT, typename IteratorT, typename StringT>
    static bool parseParameterValue(IteratorT& start, const IteratorT& end, StringT& value, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        value.clear();
        if (start != end && *start == STRING_LITERAL(Chars::plus))
        {
            value += *start; //add char to output value string
            ++start;
        }
        if (start != end && *start >= STRING_LITERAL(Chars::digit0) && *start <= STRING_LITERAL(Chars::digit9))
        {
            value += *start; //add char to output value string
            while (++start != end && *start >= STRING_LITERAL(Chars::digit0) && *start <= STRING_LITERAL(Chars::digit9))
            {
                value += *start; //add char to output value string
            }
            return true;
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }
};

///for parameters with (almost) C-style parameters supporting \n  \\  \"  \t
class CCStyleParameterPolicy : public CParameterPolicyBase
{
public:
    ///match parameter expression value and extract it
    template <typename ExceptionT, typename IteratorT, typename StringT>
    static bool parseParameterValue( IteratorT& start, const IteratorT& end, StringT& value, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        value.clear();    
        if ( start != end && *start == STRING_LITERAL( Chars::double_quote) ) //start with "
        {
            bool escape = false;
            while ( ++start != end )
            {
                if ( escape ) //previous was backslash 
                {
                    escape = false;
                    if ( *start == STRING_LITERAL( Chars::n))
                    {
                        value += STRING_LITERAL( Chars::new_line);
                        continue;
                    }
                    if ( *start == STRING_LITERAL( Chars::t))
                    {
                        value += STRING_LITERAL( Chars::tab);
                        continue;
                    }
                    if ( *start != STRING_LITERAL( Chars::double_quote) && *start != STRING_LITERAL( Chars::back_slash) )
                    {
                        break; //char not allowed after backslash, fail
                    }
                }
                else //no special handling
                {
                    if ( *start == STRING_LITERAL( Chars::double_quote) )
                    {
                        ++start;
                        return true;
                    }
                    escape = *start == STRING_LITERAL( Chars::back_slash); //next with special handling
                    if ( escape ) //do not add to value
                    {
                        continue;
                    }
                }
                value += *start; //add char to output value string
            }
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }
};

///for parameters with regular expressions
class CRegexParameterPolicy : public CParameterPolicyBase
{
public:
    ///match parameter expression value and extract it
    template <typename ExceptionT, typename IteratorT, typename StringT>
    static bool parseParameterValue( IteratorT& start, const IteratorT& end, StringT& value, bool returnOnly = false)
    {
        typedef typename IteratorT::value_type CharT;
        value.clear();    
        if ( start != end && *start == STRING_LITERAL( Chars::single_quote) )  //start with '
        {
            while ( ++start != end )
            {
                if ( *start == STRING_LITERAL( Chars::single_quote) )  //end with '
                {
                    ++start;
                    if ( start != end && *start == STRING_LITERAL( Chars::single_quote) )
                    {
                        //treat double single quote as one and continue
                    }
                    else
                    {
                        return true;
                    }
                }
                value += *start; //add char to output value string
            }
        }

        if (returnOnly)
        {
            return false;
        }
        throw ExceptionT();
    }
};

///parses a range for keyword parameters
namespace KeywordParameterParser
{
    class ExParameterStartExpected : public std::runtime_error 
    { public: ExParameterStartExpected() : std::runtime_error( "Parameter is missing or syntax incorrect. Expecting '['") {}};

    class ExParameterSeparatorExpected : public std::runtime_error 
    { public: ExParameterSeparatorExpected() : std::runtime_error( "Less parameters then expected or syntax incorrect. Expecting ','") {}};

    class ExParameterEndExpected : public std::runtime_error 
    { public: ExParameterEndExpected() : std::runtime_error( "More parameters then expected or syntax incorrect. Expecting ']'") {}};

    class ExParameterValueExpected : public std::runtime_error 
    { public: ExParameterValueExpected() : std::runtime_error( "Parameter value is missing or syntax incorrect") {}};

    ///parse parameter range, extract values
    template <typename PolicyT, typename IteratorT, typename ContainerT>
    void getParameters( IteratorT& start, const IteratorT& end, ContainerT& parameters)
    {
        PolicyT:: template parseParameterStart<ExParameterStartExpected>(start, end);
        for ( typename ContainerT::iterator it = parameters.begin();;)
        {
            PolicyT:: template parseParameterValue<ExParameterValueExpected>(start, end, *it);
            ++it;
            if( it != parameters.end() )
            {
                PolicyT:: template parseParameterSeparator<ExParameterSeparatorExpected>(start, end);
            }
            else
            {
                break;
            }
        }
        PolicyT:: template parseParameterEnd<ExParameterEndExpected>(start,end);
    }
}
