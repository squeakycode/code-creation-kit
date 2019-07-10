//  Copyright (c) 2011-2019 Andreas Gau
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

//------------------------------------------------------------------------------
//  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
//------------------------------------------------------------------------------

#include "StringLiteral.h"

template <typename TokenizerT, typename StringT> 
void setKeywords( TokenizerT& tokenizer)
{
    typedef typename StringT::value_type CharT;
    tokenizer.setMarkup( STRING_LITERAL("%"), STRING_LITERAL("%"));
}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testSingleTokens()
{
    typedef typename StringT::value_type CharT;
    TokenizerT tokenizer;
    setKeywords<TokenizerT, StringT>( tokenizer);
    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;
    std::vector<TokenT> expected;

    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));
    expected.push_back( TokenT());
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("end")));

    {//COMMENT
        result.clear();
        tokenizer << STRING_LITERAL("start%COMMENT%end");
        expected[1] = TokenT( TokenT::eComment);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//INCLUDE
        result.clear();
        tokenizer << STRING_LITERAL("start%INCLUDE%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eInclude, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//SET_MARKUP
        result.clear();
        tokenizer << STRING_LITERAL("start%SET_MARKUP%[\"parameter1\",\"parameter2\"]end");
        expected[1] = TokenT( TokenT::eMarkup, STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TRIM
        result.clear();
        tokenizer << STRING_LITERAL("start%TRIM%end");
        expected[1] = TokenT( TokenT::eTrim);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TRIM_LEFT
        result.clear();
        tokenizer << STRING_LITERAL("start%TRIM_LEFT%end");
        expected[1] = TokenT( TokenT::eTrimLeft);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TRIM_RIGHT
        result.clear();
        tokenizer << STRING_LITERAL("start%TRIM_RIGHT%end");
        expected[1] = TokenT( TokenT::eTrimRight);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PART
        result.clear();
        tokenizer << STRING_LITERAL("start%PART%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::ePart, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PART_BEGIN
        result.clear();
        tokenizer << STRING_LITERAL("start%PART_BEGIN%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::ePartBegin, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PART_END
        result.clear();
        tokenizer << STRING_LITERAL("start%PART_END%end");
        expected[1] = TokenT( TokenT::ePartEnd);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PART_REMOVE
        result.clear();
        tokenizer << STRING_LITERAL("start%PART_REMOVE%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::ePartRemove, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TABLE_BEGIN
        result.clear();
        tokenizer << STRING_LITERAL("start%TABLE_BEGIN%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eTableBegin, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TABLE_END
        result.clear();
        tokenizer << STRING_LITERAL("start%TABLE_END%end");
        expected[1] = TokenT( TokenT::eTableEnd);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TABLE_LOAD
        result.clear();
        tokenizer << STRING_LITERAL("start%TABLE_LOAD%[\"parameter1\",\"parameter2\"]end");
        expected[1] = TokenT( TokenT::eTableLoad, STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TABLE_REMOVE
        result.clear();
        tokenizer << STRING_LITERAL("start%TABLE_REMOVE%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eTableRemove, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//ANY
        result.clear();
        tokenizer << STRING_LITERAL("start%ANY%end");
        expected[1] = TokenT( TokenT::eAny);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//AS_VOLATILE
        result.clear();
        tokenizer << STRING_LITERAL("start%AS_VOLATILE%end");
        expected[1] = TokenT( TokenT::eVolatil);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//BEGIN
        result.clear();
        tokenizer << STRING_LITERAL("start%BEGIN%end");
        expected[1] = TokenT( TokenT::eBegin);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//BLOCK_FORMAT
        result.clear();
        tokenizer << STRING_LITERAL("start%BLOCK_FORMAT%[20]end");
        expected[1] = TokenT(TokenT::eBlockFormat, STRING_LITERAL("20"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//CONTAINS
        result.clear();
        tokenizer << STRING_LITERAL("start%CONTAINS%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eContains, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//COUNT
        result.clear();
        tokenizer << STRING_LITERAL("start%COUNT%end");
        expected[1] = TokenT( TokenT::eCount);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//END
        result.clear();
        tokenizer << STRING_LITERAL("start%END%end");
        expected[1] = TokenT( TokenT::eEnd);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//ENDS_WITH
        result.clear();
        tokenizer << STRING_LITERAL("start%ENDS_WITH%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eEndsWith, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//ENTRY
        result.clear();
        tokenizer << STRING_LITERAL("start%ENTRY%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eEntry, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//EQUALS
        result.clear();
        tokenizer << STRING_LITERAL("start%EQUALS%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eMatches, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//ERROR
        result.clear();
        tokenizer << STRING_LITERAL("start%ERROR%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eError_, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//FIRST_TIME
        result.clear();
        tokenizer << STRING_LITERAL("start%FIRST_TIME%end");
        expected[1] = TokenT( TokenT::eFirstTime);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//FLUSH
        result.clear();
        tokenizer << STRING_LITERAL("start%FLUSH%end");
        expected[1] = TokenT( TokenT::eFlush);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//FOR_ALL
        result.clear();
        tokenizer << STRING_LITERAL("start%FOR_ALL%end");
        expected[1] = TokenT( TokenT::eForAll);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//HTML_ESCAPE
        result.clear();
        tokenizer << STRING_LITERAL("start%HTML_ESCAPE%end");
        expected[1] = TokenT( TokenT::eHtmlEscape);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//IF
        result.clear();
        tokenizer << STRING_LITERAL("start%IF%end");
        expected[1] = TokenT( TokenT::eIf_);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//IGNORE_CASE
        result.clear();
        tokenizer << STRING_LITERAL("start%IGNORE_CASE%end");
        expected[1] = TokenT( TokenT::eIgnoreCase);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//INDEX
        result.clear();
        tokenizer << STRING_LITERAL("start%INDEX%end");
        expected[1] = TokenT( TokenT::eIndex);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//LAST_TIME
        result.clear();
        tokenizer << STRING_LITERAL("start%LAST_TIME%end");
        expected[1] = TokenT( TokenT::eLastTime);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//MACRO_BEGIN
        result.clear();
        tokenizer << STRING_LITERAL("start%MACRO_BEGIN%end");
        expected[1] = TokenT( TokenT::eMacroBegin);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//MACRO_END
        result.clear();
        tokenizer << STRING_LITERAL("start%MACRO_END%end");
        expected[1] = TokenT( TokenT::eMacroEnd);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//MATCHES_REGEX
        result.clear();
        tokenizer << STRING_LITERAL("start%MATCHES_REGEX%['parameter1']end");
        expected[1] = TokenT( TokenT::eRegexMatches, STRING_LITERAL("parameter1") );
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//MERGE
        result.clear();
        tokenizer << STRING_LITERAL("start%MERGE%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eMerge, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//NOT
        result.clear();
        tokenizer << STRING_LITERAL("start%NOT%end");
        expected[1] = TokenT( TokenT::eNot_);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//OR
        result.clear();
        tokenizer << STRING_LITERAL("start%OR%end");
        expected[1] = TokenT( TokenT::eOr_);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PAD_LEFT
        result.clear();
        tokenizer << STRING_LITERAL("start%PAD_LEFT%[\"parameter1\",42]end");
        expected[1] = TokenT(TokenT::ePadLeft, STRING_LITERAL("parameter1"), STRING_LITERAL("42"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//PAD_RIGHT
        result.clear();
        tokenizer << STRING_LITERAL("start%PAD_RIGHT%[\"parameter1\",42]end");
        expected[1] = TokenT(TokenT::ePadRight, STRING_LITERAL("parameter1"), STRING_LITERAL("42"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//READ_LEFT_TO_RIGHT
        result.clear();
        tokenizer << STRING_LITERAL("start%READ_LEFT_TO_RIGHT%end");
        expected[1] = TokenT( TokenT::eLeftToRight);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//READ_TOP_DOWN
        result.clear();
        tokenizer << STRING_LITERAL("start%READ_TOP_DOWN%end");
        expected[1] = TokenT( TokenT::eTopDown);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//REGEX_REPLACE
        result.clear();
        tokenizer << STRING_LITERAL("start%REGEX_REPLACE%['parameter1','parameter2']end");
        expected[1] = TokenT( TokenT::eRegexReplace, STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//REPLACE
        result.clear();
        tokenizer << STRING_LITERAL("start%REPLACE%[\"parameter1\",\"parameter2\"]end");
        expected[1] = TokenT( TokenT::eReplace, STRING_LITERAL("parameter1"), STRING_LITERAL("parameter2"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//STARTS_WITH
        result.clear();
        tokenizer << STRING_LITERAL("start%STARTS_WITH%[\"parameter1\"]end");
        expected[1] = TokenT( TokenT::eStartsWith, STRING_LITERAL("parameter1"));
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TO_CSTRING
        result.clear();
        tokenizer << STRING_LITERAL("start%TO_CSTRING%end");
        expected[1] = TokenT( TokenT::eToCString);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TO_LOWER
        result.clear();
        tokenizer << STRING_LITERAL("start%TO_LOWER%end");
        expected[1] = TokenT( TokenT::eToLower);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

    {//TO_UPPER
        result.clear();
        tokenizer << STRING_LITERAL("start%TO_UPPER%end");
        expected[1] = TokenT( TokenT::eToUpper);
        BOOST_CHECK( result.size() == 3);
        BOOST_CHECK( expected == result);
    }

}

template <typename TokenizerT, typename OutputT, typename TokenT, typename StringT>
void testRemoveDelayMarks( TokenizerT& tokenizer, bool bypassMode)
{
    typedef typename StringT::value_type CharT;
    setKeywords<TokenizerT, StringT>( tokenizer);
    OutputT helper;
    tokenizer.connectOutputStream( &helper); 
    std::vector<TokenT>& result = helper.result;

    std::vector<TokenT> expected;
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("start")));
    expected.push_back( TokenT());
    expected.push_back( TokenT( TokenT::eTextFragment, bypassMode ? STRING_LITERAL("%") : STRING_LITERAL(".%")));
    expected.push_back( TokenT( TokenT::eTextFragment, STRING_LITERAL("end")));

    {//COMMENT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%COMMENT.%end") : STRING_LITERAL("start%COMMENT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%COMMENT"));
        BOOST_CHECK( expected == result);
    }

    {//INCLUDE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%INCLUDE.%end") : STRING_LITERAL("start%INCLUDE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%INCLUDE"));
        BOOST_CHECK( expected == result);
    }

    {//SET_MARKUP

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%SET_MARKUP.%end") : STRING_LITERAL("start%SET_MARKUP..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%SET_MARKUP"));
        BOOST_CHECK( expected == result);
    }

    {//TRIM

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TRIM.%end") : STRING_LITERAL("start%TRIM..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM"));
        BOOST_CHECK( expected == result);
    }

    {//TRIM_LEFT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TRIM_LEFT.%end") : STRING_LITERAL("start%TRIM_LEFT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM_LEFT"));
        BOOST_CHECK( expected == result);
    }

    {//TRIM_RIGHT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TRIM_RIGHT.%end") : STRING_LITERAL("start%TRIM_RIGHT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TRIM_RIGHT"));
        BOOST_CHECK( expected == result);
    }

    {//PART

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PART.%end") : STRING_LITERAL("start%PART..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PART"));
        BOOST_CHECK( expected == result);
    }

    {//PART_BEGIN

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PART_BEGIN.%end") : STRING_LITERAL("start%PART_BEGIN..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PART_BEGIN"));
        BOOST_CHECK( expected == result);
    }

    {//PART_END

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PART_END.%end") : STRING_LITERAL("start%PART_END..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PART_END"));
        BOOST_CHECK( expected == result);
    }

    {//PART_REMOVE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PART_REMOVE.%end") : STRING_LITERAL("start%PART_REMOVE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PART_REMOVE"));
        BOOST_CHECK( expected == result);
    }

    {//TABLE_BEGIN

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TABLE_BEGIN.%end") : STRING_LITERAL("start%TABLE_BEGIN..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TABLE_BEGIN"));
        BOOST_CHECK( expected == result);
    }

    {//TABLE_END

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TABLE_END.%end") : STRING_LITERAL("start%TABLE_END..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TABLE_END"));
        BOOST_CHECK( expected == result);
    }

    {//TABLE_LOAD

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TABLE_LOAD.%end") : STRING_LITERAL("start%TABLE_LOAD..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TABLE_LOAD"));
        BOOST_CHECK( expected == result);
    }

    {//TABLE_REMOVE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TABLE_REMOVE.%end") : STRING_LITERAL("start%TABLE_REMOVE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TABLE_REMOVE"));
        BOOST_CHECK( expected == result);
    }

    {//ANY

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%ANY.%end") : STRING_LITERAL("start%ANY..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%ANY"));
        BOOST_CHECK( expected == result);
    }

    {//AS_VOLATILE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%AS_VOLATILE.%end") : STRING_LITERAL("start%AS_VOLATILE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%AS_VOLATILE"));
        BOOST_CHECK( expected == result);
    }

    {//BEGIN

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%BEGIN.%end") : STRING_LITERAL("start%BEGIN..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%BEGIN"));
        BOOST_CHECK( expected == result);
    }

    {//BLOCK_FORMAT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%BLOCK_FORMAT.%end") : STRING_LITERAL("start%BLOCK_FORMAT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%BLOCK_FORMAT"));
        BOOST_CHECK( expected == result);
    }

    {//CONTAINS

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%CONTAINS.%end") : STRING_LITERAL("start%CONTAINS..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%CONTAINS"));
        BOOST_CHECK( expected == result);
    }

    {//COUNT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%COUNT.%end") : STRING_LITERAL("start%COUNT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%COUNT"));
        BOOST_CHECK( expected == result);
    }

    {//END

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%END.%end") : STRING_LITERAL("start%END..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%END"));
        BOOST_CHECK( expected == result);
    }

    {//ENDS_WITH

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%ENDS_WITH.%end") : STRING_LITERAL("start%ENDS_WITH..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%ENDS_WITH"));
        BOOST_CHECK( expected == result);
    }

    {//ENTRY

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%ENTRY.%end") : STRING_LITERAL("start%ENTRY..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%ENTRY"));
        BOOST_CHECK( expected == result);
    }

    {//EQUALS

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%EQUALS.%end") : STRING_LITERAL("start%EQUALS..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%EQUALS"));
        BOOST_CHECK( expected == result);
    }

    {//ERROR

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%ERROR.%end") : STRING_LITERAL("start%ERROR..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%ERROR"));
        BOOST_CHECK( expected == result);
    }

    {//FIRST_TIME

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%FIRST_TIME.%end") : STRING_LITERAL("start%FIRST_TIME..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%FIRST_TIME"));
        BOOST_CHECK( expected == result);
    }

    {//FLUSH

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%FLUSH.%end") : STRING_LITERAL("start%FLUSH..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%FLUSH"));
        BOOST_CHECK( expected == result);
    }

    {//FOR_ALL

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%FOR_ALL.%end") : STRING_LITERAL("start%FOR_ALL..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%FOR_ALL"));
        BOOST_CHECK( expected == result);
    }

    {//HTML_ESCAPE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%HTML_ESCAPE.%end") : STRING_LITERAL("start%HTML_ESCAPE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%HTML_ESCAPE"));
        BOOST_CHECK( expected == result);
    }

    {//IF

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%IF.%end") : STRING_LITERAL("start%IF..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%IF"));
        BOOST_CHECK( expected == result);
    }

    {//IGNORE_CASE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%IGNORE_CASE.%end") : STRING_LITERAL("start%IGNORE_CASE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%IGNORE_CASE"));
        BOOST_CHECK( expected == result);
    }

    {//INDEX

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%INDEX.%end") : STRING_LITERAL("start%INDEX..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%INDEX"));
        BOOST_CHECK( expected == result);
    }

    {//LAST_TIME

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%LAST_TIME.%end") : STRING_LITERAL("start%LAST_TIME..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%LAST_TIME"));
        BOOST_CHECK( expected == result);
    }

    {//MACRO_BEGIN

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%MACRO_BEGIN.%end") : STRING_LITERAL("start%MACRO_BEGIN..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%MACRO_BEGIN"));
        BOOST_CHECK( expected == result);
    }

    {//MACRO_END

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%MACRO_END.%end") : STRING_LITERAL("start%MACRO_END..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%MACRO_END"));
        BOOST_CHECK( expected == result);
    }

    {//MATCHES_REGEX

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%MATCHES_REGEX.%end") : STRING_LITERAL("start%MATCHES_REGEX..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%MATCHES_REGEX"));
        BOOST_CHECK( expected == result);
    }

    {//MERGE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%MERGE.%end") : STRING_LITERAL("start%MERGE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%MERGE"));
        BOOST_CHECK( expected == result);
    }

    {//NOT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%NOT.%end") : STRING_LITERAL("start%NOT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%NOT"));
        BOOST_CHECK( expected == result);
    }

    {//OR

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%OR.%end") : STRING_LITERAL("start%OR..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%OR"));
        BOOST_CHECK( expected == result);
    }

    {//PAD_LEFT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PAD_LEFT.%end") : STRING_LITERAL("start%PAD_LEFT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PAD_LEFT"));
        BOOST_CHECK( expected == result);
    }

    {//PAD_RIGHT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%PAD_RIGHT.%end") : STRING_LITERAL("start%PAD_RIGHT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%PAD_RIGHT"));
        BOOST_CHECK( expected == result);
    }

    {//READ_LEFT_TO_RIGHT

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%READ_LEFT_TO_RIGHT.%end") : STRING_LITERAL("start%READ_LEFT_TO_RIGHT..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%READ_LEFT_TO_RIGHT"));
        BOOST_CHECK( expected == result);
    }

    {//READ_TOP_DOWN

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%READ_TOP_DOWN.%end") : STRING_LITERAL("start%READ_TOP_DOWN..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%READ_TOP_DOWN"));
        BOOST_CHECK( expected == result);
    }

    {//REGEX_REPLACE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%REGEX_REPLACE.%end") : STRING_LITERAL("start%REGEX_REPLACE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%REGEX_REPLACE"));
        BOOST_CHECK( expected == result);
    }

    {//REPLACE

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%REPLACE.%end") : STRING_LITERAL("start%REPLACE..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%REPLACE"));
        BOOST_CHECK( expected == result);
    }

    {//STARTS_WITH

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%STARTS_WITH.%end") : STRING_LITERAL("start%STARTS_WITH..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%STARTS_WITH"));
        BOOST_CHECK( expected == result);
    }

    {//TO_CSTRING

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TO_CSTRING.%end") : STRING_LITERAL("start%TO_CSTRING..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TO_CSTRING"));
        BOOST_CHECK( expected == result);
    }

    {//TO_LOWER

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TO_LOWER.%end") : STRING_LITERAL("start%TO_LOWER..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TO_LOWER"));
        BOOST_CHECK( expected == result);
    }

    {//TO_UPPER

        result.clear();
        tokenizer << (bypassMode ? STRING_LITERAL("start%TO_UPPER.%end") : STRING_LITERAL("start%TO_UPPER..%end"));
        expected[1] = TokenT( TokenT::eTextFragment, STRING_LITERAL("%TO_UPPER"));
        BOOST_CHECK( expected == result);
    }

}
