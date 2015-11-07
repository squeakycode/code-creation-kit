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

///parses a csv file, csv data is fed into a table builder, position can be tracked
class CCsvParser
{
public:
    class ExBadDelimiter : public std::invalid_argument
    { public: ExBadDelimiter( const std::string& name) : std::invalid_argument( name + " cannot be used as delimiting character.") {}};

    class ExBadCommentChars : public std::invalid_argument
    { public: ExBadCommentChars() : std::invalid_argument( "Quote, new line, carriage return, and the delimiter cannot be used for commenting lines.") {}};

    class ExRequireDelimitingChar : public std::runtime_error 
    { public: ExRequireDelimitingChar() : std::runtime_error( "Expecting new line or delimiter at the end of item in quotes. Check quotes.") {}};

    class ExUnexpectedQuote : public std::runtime_error 
    { public: ExUnexpectedQuote() : std::runtime_error( "Unexpected quote. Check quotes.") {}};

    class ExStreamNotReady : public std::runtime_error 
    { public: ExStreamNotReady() : std::runtime_error( "Parser input stream not ready.") {}};

    class ExStreamBad : public std::runtime_error 
    { public: ExStreamBad() : std::runtime_error( "Failed to read from parser input stream.") {}};

    ///check chars used for commenting, throws ExBadCommentChars when bad
    template <typename StringT, typename CharT, typename StreamT>
    static void checkCharsUsedForCommenting( const StringT& commentChars, const CharT delimiter, bool ignoreDoubleQuotes, const StreamT& stream)
    {
        const CharT quote = stream.widen('"');
        const CharT new_line = stream.widen('\n');
        const CharT carriage_return = stream.widen('\r');

        //check characters for commenting a line
        for ( typename StringT::const_iterator it = commentChars.begin(); it != commentChars.end(); ++it)
        {
            if (    *it == delimiter
                ||  (*it == quote && !ignoreDoubleQuotes)
                ||  *it == new_line
                ||  *it == carriage_return)
            {
                throw ExBadCommentChars();
            }
        }
    }

    ///check chars used for commenting, throws ExBadCommentChars when bad
    template <typename CharT, typename StreamT>
    static void checkDelimiter( const CharT delimiter, bool ignoreDoubleQuotes, const StreamT& stream)
    {
        const CharT quote = stream.widen('"');
        const CharT new_line = stream.widen('\n');
        const CharT carriage_return = stream.widen('\r');

        if ( delimiter == quote && !ignoreDoubleQuotes)
        {
            throw ExBadDelimiter( "Quote");
        }
        if ( delimiter == new_line )
        {
            throw ExBadDelimiter( "New line");
        }
        if ( delimiter == carriage_return)
        {
            throw ExBadDelimiter( "Carriage return");
        }
    }

    ///parses a csv file with the delimiter given, csv data is fed into table builder, postion is fed into postion tracker
    template <typename StreamT, typename TableBuilderT, typename PositionTrackerT>
    static void parse( StreamT& stream, TableBuilderT& tableBuilder, typename TableBuilderT::StringT::value_type delimiter, const typename TableBuilderT::StringT& commentChars, bool ignoreDoubleQuotes,PositionTrackerT& positionTracker)
    {
        positionTracker.reset();

        if ( !stream )
        {
            throw ExStreamNotReady();
        }

        typedef typename TableBuilderT::StringT::value_type CharT;
        typedef typename TableBuilderT::StringT StringT;

        const CharT quote = stream.widen('"');
        const CharT new_line = stream.widen('\n');
        const CharT carriage_return = stream.widen('\r');

        checkDelimiter( delimiter, ignoreDoubleQuotes, stream); 
        checkCharsUsedForCommenting( commentChars, delimiter, ignoreDoubleQuotes, stream);

        CharT c;
        StringT item;
        bool newLine = true;

        while( stream.get(c))
        {
            if ( c == carriage_return ) continue; //ignore carriage return

            //check for comment if needed
            if ( newLine && !commentChars.empty())
            {
                bool isComment = false;
                for ( typename StringT::const_iterator it = commentChars.begin(); it != commentChars.end(); ++it)
                {
                    if ( *it == c) //is comment
                    {
                        while( stream.get(c)) //consume one line
                        {
                            if ( c == new_line)
                            {
                                positionTracker.nextLine();
                                isComment = true;
                                break;
                            }
                        }
                        break;
                    }
                }
                if ( isComment) //back to start?
                {
                    continue;
                }
            }

            item.clear();
            if ( c == quote && !ignoreDoubleQuotes) //item in quotes
            {
                positionTracker.nextColumn();
                while( stream.get( c))
                {
                    if ( c == carriage_return ) continue; //ignore carriage return
                    if ( c == quote ) //quote in item in quotes
                    {
                        positionTracker.nextColumn();
                        if ( stream.get( c)) //read next character to find out what to do
                        {
                            if ( c == carriage_return ) //ignore carriage return
                            {
                                if ( !stream.get( c))
                                {
                                    c = new_line; //end of file will be treated as new line
                                    break;
                                }
                            }

                            if ( c == quote ) //double quote results in single quote
                            {
                                //no action, added below
                            }
                            else if ( c == delimiter || c == new_line ) //delimiter or end of line marking end of item
                            {
                                break;
                            }
                            else //error condition otherwise
                            {
                                throw ExRequireDelimitingChar();
                            }
                        }
                        else
                        {
                            c = new_line; //end of file will be treated as new line
                            break;
                        }
                    }

                    item += c; //add character to item
                    if ( c == new_line ) //update position tracking
                    {
                        positionTracker.nextLine();
                    }
                    else
                    {
                        positionTracker.nextColumn();
                    }
                }
            }
            else //item without quotes
            {
                do
                {
                    if ( c == carriage_return ) continue; //ignore carriage return
                    if ( c == delimiter || c == new_line ) break; //end of item
                    if ( c == quote && !ignoreDoubleQuotes) //error condition
                    {
                        throw ExUnexpectedQuote();
                    }
                    item += c; //add character to item
                    positionTracker.nextColumn();
                }
                while( stream.get( c));
            }

            tableBuilder.addItem( item);
            if ( c == new_line ) //begin new row
            {
                tableBuilder.addRow();
                positionTracker.nextLine();
                newLine = true;
            }
            else if ( c == delimiter ) //the delimiter is not added to item and that's why the position is not updated yet
            {
                positionTracker.nextColumn();
                newLine = false;
            }
        }

        if ( stream.bad() || (stream.fail() && !stream.eof()))
        {
            throw ExStreamBad();
        }

        tableBuilder.finished();
    }

    ///parse with no position tracker required
    template <typename StreamT, typename TableBuilderT>
    static void parse( StreamT& stream, TableBuilderT& tableBuilder, typename TableBuilderT::StringT::value_type delimiter, const typename TableBuilderT::StringT& commentChars, bool ignoreDoubleQuotes)
    {
        CNoTracker noTracker;
        parse( stream, tableBuilder, delimiter, commentChars, ignoreDoubleQuotes, noTracker);
    }

private:
    ///dummy position tracker used when prosition tracking is not required
    struct CNoTracker{void reset(){}void nextLine(){}void nextColumn(){}};
};
