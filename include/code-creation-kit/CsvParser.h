// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <stdexcept>

namespace code_creation_kit
{
    /*
      tablebuilder duck-typing interface
        void addItem(const std::string& item); // called when a table entry has been parsed
        void addRow(); // called when the end of a row has been parsed
        void finished(); // called when table end has been reached
      position tracker duck-typing interface
        void reset();
        void nextLine();
        void nextColumn();
    */
    ///parses a csv file, csv data is fed into a table builder, position can be tracked
    class CCsvParser
    {
    public:
        class ExBadDelimiter : public std::invalid_argument
        {
        public:
            explicit ExBadDelimiter(const std::string& name) : std::invalid_argument(name + " cannot be used as delimiting character.") {}
        };

        class ExBadCommentChars : public std::invalid_argument
        {
        public: explicit ExBadCommentChars(const std::string& name) : std::invalid_argument(name + " cannot be used for commenting lines.") {}
        };

        class ExBadQuoteChars : public std::invalid_argument
        {
        public: explicit ExBadQuoteChars(const std::string& name) : std::invalid_argument(name + " cannot be used for text item quote.") {}
        };

        class ExRequireDelimitingChar : public std::runtime_error
        {
        public: ExRequireDelimitingChar() : std::runtime_error("Expecting new line or delimiter at the end of item in quotes. Check quotes.") {}
        };

        class ExUnexpectedQuote : public std::runtime_error
        {
        public: ExUnexpectedQuote() : std::runtime_error("Unexpected quote. Check quotes.") {}
        };

        class ExStreamNotReady : public std::runtime_error
        {
        public: ExStreamNotReady() : std::runtime_error("Parser input stream not ready.") {}
        };

        class ExStreamBad : public std::runtime_error
        {
        public: ExStreamBad() : std::runtime_error("Failed to read from parser input stream.") {}
        };

        enum EUsedCsvCharsCheck
        {
            UsedCsvCharsCheck_Delimiter,
            UsedCsvCharsCheck_Quote,
            UsedCsvCharsCheck_Commenting,
            UsedCsvCharsCheck_All
        };

        ///check chars used, throws when bad
        template <typename StringT>
        static void checkCharsUsedForCsvParsing(EUsedCsvCharsCheck check, const StringT& delimiterChars, const StringT& quoteChars, const StringT& commentChars)
        {
            if (check == UsedCsvCharsCheck_Delimiter || check == UsedCsvCharsCheck_All)
            {
                checkChars<StringT, ExBadDelimiter>(delimiterChars, quoteChars, "Quote character ");
                checkChars<StringT, ExBadDelimiter>(delimiterChars, commentChars, "Commenting character ");
            }
            if (check == UsedCsvCharsCheck_Quote || check == UsedCsvCharsCheck_All)
            {
                checkChars<StringT, ExBadQuoteChars>(quoteChars, delimiterChars, "Delimiter ");
                checkChars<StringT, ExBadQuoteChars>(quoteChars, commentChars, "Commenting character ");
            }
            if (check == UsedCsvCharsCheck_Commenting || check == UsedCsvCharsCheck_All)
            {
                checkChars<StringT, ExBadCommentChars>(commentChars, quoteChars, "Quote character ");
                checkChars<StringT, ExBadCommentChars>(commentChars, delimiterChars, "Delimiter ");
            }
        }

        ///parses a csv file with the delimiter given, csv data is fed into table builder, postion is fed into postion tracker
        template <typename StreamT, typename TableBuilderT, typename PositionTrackerT>
        static void parse(
            StreamT& stream,
            TableBuilderT& tableBuilder,
            const typename TableBuilderT::StringT& delimiterChars,
            const typename TableBuilderT::StringT& quoteChars,
            const typename TableBuilderT::StringT& commentChars,
            PositionTrackerT& positionTracker)
        {
            positionTracker.reset();

            if (!stream)
            {
                throw ExStreamNotReady();
            }

            typedef typename TableBuilderT::StringT::value_type CharT;
            typedef typename TableBuilderT::StringT StringT;

            auto isDelimiter = [&delimiterChars](CharT c) -> bool
            {
                for (auto x : delimiterChars)
                {
                    if (x == c)
                    {
                        return true;
                    }
                }
                return false;
            };

            auto isQuote = [&quoteChars](CharT c) -> bool
            {
                for (auto x : quoteChars)
                {
                    if (x == c)
                    {
                        return true;
                    }
                }
                return false;
            };

            const CharT new_line = stream.widen('\n');
            const CharT carriage_return = stream.widen('\r');

            checkCharsUsedForCsvParsing(UsedCsvCharsCheck_All, delimiterChars, quoteChars, commentChars);

            CharT c = 0;
            StringT item;
            bool newLine = true;

            while (stream.get(c))
            {
                if (c == carriage_return) continue; //ignore carriage return

                //check for comment if needed
                if (newLine && !commentChars.empty())
                {
                    bool isComment = false;
                    for (typename StringT::const_iterator it = commentChars.begin(); it != commentChars.end(); ++it)
                    {
                        if (*it == c) //is comment
                        {
                            while (stream.get(c)) //consume one line
                            {
                                if (c == new_line)
                                {
                                    positionTracker.nextLine();
                                    isComment = true;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    if (isComment) //back to start?
                    {
                        continue;
                    }
                }

                item.clear();
                if (isQuote(c)) //item in quotes
                {
                    CharT quote = c; //quoted item must use quote character consistently

                    positionTracker.nextColumn();
                    while (stream.get(c))
                    {
                        if (c == carriage_return) continue; //ignore carriage return
                        if (c == quote) //quote in item in quotes
                        {
                            positionTracker.nextColumn();
                            if (stream.get(c)) //read next character to find out what to do
                            {
                                if (c == carriage_return) //ignore carriage return
                                {
                                    if (!stream.get(c))
                                    {
                                        c = new_line; //end of file will be treated as new line
                                        break;
                                    }
                                }

                                if (c == quote) //double quote results in single quote
                                {
                                    //no action, added below
                                }
                                else if (isDelimiter(c) || c == new_line) //delimiter or end of line marking end of item
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
                        if (c == new_line) //update position tracking
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
                        if (c == carriage_return) continue; //ignore carriage return
                        if (isDelimiter(c) || c == new_line) break; //end of item
                        if (isQuote(c)) //error condition
                        {
                            throw ExUnexpectedQuote();
                        }
                        item += c; //add character to item
                        positionTracker.nextColumn();
                    } while (stream.get(c));
                }

                tableBuilder.addItem(item);
                if (c == new_line) //begin new row
                {
                    tableBuilder.addRow();
                    positionTracker.nextLine();
                    newLine = true;
                }
                else if (isDelimiter(c)) //the delimiter is not added to item and that's why the position is not updated yet
                {
                    positionTracker.nextColumn();
                    newLine = false;
                }
            }

            if (stream.bad() || (stream.fail() && !stream.eof()))
            {
                throw ExStreamBad();
            }
            else if (
                !newLine //last character of file is not new line
                && isDelimiter(c) //the previous character was a delimiter
                )
            {
                //last item is empty followed by end of file
                //add this empty item
                item.clear();
                tableBuilder.addItem(item);
            }

            tableBuilder.finished();
        }

        ///parse with no position tracker required
        template <typename StreamT, typename TableBuilderT>
        static void parse(
            StreamT& stream,
            TableBuilderT& tableBuilder,
            const typename TableBuilderT::StringT& delimiterChars,
            const typename TableBuilderT::StringT& commentChars,
            const typename TableBuilderT::StringT& quoteChars
            )
        {
            CNoTracker noTracker;
            parse(stream, tableBuilder, delimiterChars, commentChars, quoteChars, noTracker);
        }

    protected:
         ///check chars used for throw exception when bad
        template <typename StringT, typename ExceptionT>
        static void checkChars(const StringT& chars, const StringT& forbiddenChars, const char* forbiddenCharsName)
        {
            typedef typename StringT::value_type CharT;
            const CharT new_line = '\n';
            const CharT carriage_return = '\r';

            for (auto c : chars)
            {
                for (auto f : forbiddenChars)
                {
                    if (f == c)
                    {
                        throw ExceptionT(std::string(forbiddenCharsName) + static_cast<char>(c));
                    }
                }
                if (c == new_line)
                {
                    throw ExceptionT("New line");
                }
                if (c == carriage_return)
                {
                    throw ExceptionT("Carriage return");
                }
            }
        }

        ///dummy position tracker used when prosition tracking is not required
        struct CNoTracker { static void reset(){} static void nextLine(){} static void nextColumn(){} };
    };
}
