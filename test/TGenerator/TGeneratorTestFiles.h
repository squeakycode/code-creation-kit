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

#include "../include/TestInputFileWriter.h"

inline void CreateTGeneratorFiles()
{

    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/LogOutputExpected.txt
    {
        const char* fileData =
R"(Enabling log.
Disabling log.
Switching log stream.
Resetting:
Clearing table list.
Clearing set include paths.
Setting CSV Delimiter:
CSV Delimiter=;
Setting CSV Comment Chars:
CSV Comment Chars=
Setting CSV Ignore Double Quotes=:
CSV Ignore Double Quotes=0
Setting CSV Delimiter:
CSV Delimiter=:
Setting CSV Delimiter:
CSV Delimiter=;
Setting CSV Comment Chars:
CSV Comment Chars=#*
Setting CSV Comment Chars:
CSV Comment Chars=
Setting CSV Ignore Double Quotes=:
CSV Ignore Double Quotes=1
Setting CSV Ignore Double Quotes=:
CSV Ignore Double Quotes=0
Adding include directory:
Include directory=IncludeDirectoryThatDoesNotExist
Setting markup:
Prefix=&
Postfix=&
Setting markup:
Prefix=[
Postfix=]
Loading table:
Table file=TestInputFile_TDataflow.csv
Label=LabelB
Read top down=1
Read left to right=1
Row header index=1
Column header index=1
Pad rows=0
Unloading table:
Label=LabelB
Loading table:
Table file=TestInputFile_TDataflow.csv
Label=LabelA
Read top down=1
Read left to right=1
Row header index=1
Column header index=1
Pad rows=0
Processing file:
Template file=../TGenerator/TestInputFile_TDataflow.tpl.txt
Target file=TestInputFile_TDataflow.gen.txt
Use intermediate file=0
Recycle=0
Intermediate file=
Append=0
Starting to read template file:
Name=../TGenerator/TestInputFile_TDataflow.tpl.txt
Found macro (level 0):
[MACRO_BEGIN]<[ENTRY]["row1"]>[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<1>
Found macro (level 0):
[MACRO_BEGIN]<[ENTRY]["row2"]>[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<2>
Found macro (level 5):
start1<1>end1 <[ENTRY]["row2"]> start4<2>end4

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
start1<1>end1 <2> start4<2>end4

Found macro (level 0):
[MACRO_BEGIN][MACRO_BEGIN.]<[ENTRY]["row1"]>[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
[MACRO_BEGIN.]<1>
Found macro (level 1):
[MACRO_BEGIN]<1>end5start6<[ENTRY]["row2"]>[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<1>end5start6<2>
Found macro (level 0):
[MACRO_BEGIN]<[ENTRY]["row1"]>[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<1>
Found macro (level 1):
start7<1>end7start8<[ENTRY]["row3"]>end8

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
start7<1>end7start8<3>end8

Expanding at index 3:
start7<1>end7start8<3>end8

Found macro (level 0):
[MACRO_BEGIN]
    start3
[MACRO_BEGIN.]
    <[ENTRY.]["row3"]>
[MACRO_END.]
    end3
[MACRO_END]
Expanding:

    start3
[MACRO_BEGIN.]
    <[ENTRY.]["row3"]>
[MACRO_END.]
    end3

Found macro (level 1):
[MACRO_BEGIN]
    <[ENTRY]["row3"]>
[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:

    <3>

Expanding at index 3:

    <3>

Found macro (level 0):
[MACRO_BEGIN]
    <[ENTRY.]["row3"]>
[MACRO_END]
Expanding:

    <[ENTRY.]["row3"]>

Found macro (level 1):
[MACRO_BEGIN]
    start11

    <[ENTRY]["row3"]>

    end11
[MACRO_END]
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:

    start11

    <3>

    end11

Expanding at index 3:

    start11

    <3>

    end11

Found macro (level 0):
[MACRO_BEGIN]
->[MACRO_END]
Expanding:

->
Found macro (level 1):
->start10<[ENTRY]["row3"]>end10

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
->start10<3>end10

Expanding at index 3:
->start10<3>end10

Found part (level 0):
[PART_BEGIN]["partLabel"] <[ENTRY.]["row3"]>{[ENTRY]["row2"]}
[PART_END]
Adding part with label: partLabel
Expanding part with label: partLabel
Found macro (level 0):
 <[ENTRY.]["row3"]>{[ENTRY]["row2"]}

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
 <[ENTRY.]["row3"]>{2}

Found macro (level 1):
 <[ENTRY]["row3"]>{2}

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
 <3>{2}

Expanding at index 3:
 <3>{2}

Removing part with label: partLabel
Part to remove not found with label: partLabel
Found macro (level 1):
<[ENTRY]["row3"]>1/1

Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<3>1/1

Expanding at index 3:
<3>1/1

Finished reading of template file:
Name=../TGenerator/TestInputFile_TDataflow.tpl.txt
Found macro (level 1):
<[ENTRY]["row3"]>off
Start reading table top down:
Label=LabelA
Entries or reading direction do not match macro.
Start reading table left to right:
Label=LabelA
Expanding at index 2:
<3>off
Expanding at index 3:
<3>off
Processing file:
Template file=../TGenerator/TestInputFile_LogTest1.tpl
Target file=../TGenerator/TestInputFile_LogTest1.gen.txt
Use intermediate file=0
Recycle=0
Intermediate file=
Append=0
Starting to read template file:
Name=../TGenerator/TestInputFile_LogTest1.tpl
Found macro (level 0):
[MACRO_BEGIN]

[IF][IF]

[MACRO_END]
Successfully processed part of macro:
[MACRO_BEGIN]

[IF]
Processing file:
Template file=../TGenerator/TestInputFile_LogTest2.tpl
Target file=../TGenerator/TestInputFile_LogTest2.tpl
Use intermediate file=1
Recycle=0
Intermediate file=../TGenerator/TestInputFile_LogTest2.tpl.intermediate
Append=0
Inline prefix=$
Inline postfix=%
Inline generated postfix=&
Inline pad=3
Starting to read template file:
Name=../TGenerator/TestInputFile_LogTest2.tpl
Error parsing parameters in line:
 $ a line with bad tag inline parameters [ENTRY]["bad"3]1234567 % 
  a line with bad tag inline parameters [ENTRY]["bad"
Processing file:
Template file=../TGenerator/TestInputFile_LogTest3.tpl
Target file=../TGenerator/TestInputFile_LogTest1.gen.txt
Use intermediate file=0
Recycle=0
Intermediate file=
Append=0
Starting to read template file:
Name=../TGenerator/TestInputFile_LogTest3.tpl
Error parsing parameters in line:
a line with bad tag parameters [ENTRY]["bad"1]1234567 
a line with bad tag parameters [ENTRY]["bad"
Disabling log.
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "LogOutputExpected.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/LogTest1.tpl
    {
        const char* fileData =
R"(a bad macro:
[MACRO_BEGIN]

[IF][IF]

[MACRO_END])";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "LogTest1.tpl", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/LogTest2.tpl
    {
        const char* fileData =
R"(start
 $ a line with bad tag inline parameters [ENTRY]["bad"3]1234567 % 
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "LogTest2.tpl", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/LogTest3.tpl
    {
        const char* fileData =
R"(start
a line with bad tag parameters [ENTRY]["bad"1]1234567 
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "LogTest3.tpl", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/NeedsPadding.csv
    {
        const char* fileData =
R"(a
b=c
d=e=f=g
h=i
j
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "NeedsPadding.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/NeedsPaddingDuplicate.tpl
    {
        const char* fileData =
R"([BEGIN.][ENTRY.]["[INDEX]"][READ_LEFT_TO_RIGHT.][BEGIN.][IF.][LAST_TIME.][OR.]=[END.][END.][IF][ENTRY]["1"][READ_TOP_DOWN]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "NeedsPaddingDuplicate.tpl", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TAppendExpected.txt
    {
        const char* fileData =
R"(start1<1>end1 <2> start4<2>end4

start5<1>end5start6<2>end6

start7<1>end7start8<3>end8
start7<1>end7start8<3>end8

start2

    start3

    <3>

    <3>

    end3

end2

start12

    start11

    <3>

    end11

    start11

    <3>

    end11

end12

start9

->start10<3>end10
->start10<3>end10
end9

 <3>{2}
 <3>{2}

<[ENTRY]["row3"]>0/1
<3>1/1
<3>1/1
<[ENTRY]["row3"]>3/4
<3>off<3>offstart1<1>end1 <2> start4<2>end4

start5<1>end5start6<2>end6

start7<1>end7start8<3>end8
start7<1>end7start8<3>end8

start2

    start3

    <3>

    <3>

    end3

end2

start12

    start11

    <3>

    end11

    start11

    <3>

    end11

end12

start9

->start10<3>end10
->start10<3>end10
end9

 <3>{2}
 <3>{2}

<[ENTRY]["row3"]>0/1
<3>1/1
<3>1/1
<[ENTRY]["row3"]>3/4
<3>off<3>off)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TAppendExpected.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TDataflow.csv
    {
        const char* fileData =
R"(;;
row1;1;
row2;2;
row3;3;3
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TDataflow.tpl.txt
    {
        const char* fileData =
R"([COMMENT] test inline macros and unprocessed text stay in right order and tick removal ok
start1[MACRO_BEGIN]<[ENTRY]["row1"]>[MACRO_END]end1 <[ENTRY.....]["row2"]> start4[MACRO_BEGIN]<[ENTRY]["row2"]>[MACRO_END]end4

[COMMENT] test after expansions of first macro a new macro is created with the rest of line
start5[MACRO_BEGIN][MACRO_BEGIN.]<[ENTRY]["row1"]>[MACRO_END]end5start6<[ENTRY.]["row2"]>[MACRO_END.]end6

[COMMENT] first inline macro expansion then line expansion, whole line must be expanded in second step
start7[MACRO_BEGIN]<[ENTRY]["row1"]>[MACRO_END]end7start8<[ENTRY.]["row3"]>end8

[COMMENT] test recursive expansion
start2
[MACRO_BEGIN]
    start3
[MACRO_BEGIN.]
    <[ENTRY.]["row3"]>
[MACRO_END.]
    end3
[MACRO_END]
end2

[COMMENT] test recursive expansion 2
start12
[MACRO_BEGIN.]
    start11
[MACRO_BEGIN]
    <[ENTRY.]["row3"]>
[MACRO_END]
    end11
[MACRO_END.]
end12

[COMMENT] rest of multiline macro and rest of line form new line based macro
start9
[MACRO_BEGIN]
->[MACRO_END]start10<[ENTRY.]["row3"]>end10
end9

[PART_BEGIN]["partLabel"][TRIM]
 <[ENTRY.]["row3"]>{[ENTRY]["row2"]}
[PART_END][TRIM]
[PART]["partLabel"][TRIM]
[PART_REMOVE]["partLabel"][TRIM]
[PART_REMOVE]["partLabel"][TRIM]

[COMMENT] recursion level limit test
[SET_RECURSION_LEVEL_LIMIT][TRIM]
<[ENTRY.]["row3"]>0/1
[SET_RECURSION_LEVEL_LIMIT.][TRIM]
<[ENTRY.]["row3"]>1/1
[SET_RECURSION_LEVEL_LIMIT...][TRIM]
<[ENTRY....]["row3"]>3/4
[SET_RECURSION_LEVEL_LIMIT_OFF.][TRIM]
<[ENTRY.]["row3"]>off)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TDataflow.tpl.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TDataflowExpected.txt
    {
        const char* fileData =
R"(start1<1>end1 <2> start4<2>end4

start5<1>end5start6<2>end6

start7<1>end7start8<3>end8
start7<1>end7start8<3>end8

start2

    start3

    <3>

    <3>

    end3

end2

start12

    start11

    <3>

    end11

    start11

    <3>

    end11

end12

start9

->start10<3>end10
->start10<3>end10
end9

 <3>{2}
 <3>{2}

<[ENTRY]["row3"]>0/1
<3>1/1
<3>1/1
<[ENTRY]["row3"]>3/4
<3>off<3>off)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TDataflowExpected.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGenerator.xls.csv
    {
        const char* fileData =
R"(a;b;
1;1;
2;;
#this is a comment
3;3;
;4;
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGenerator.xls.csv", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGeneratorOutExpected.txt
    {
        const char* fileData =
R"(start
11
33
45
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutExpected.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGeneratorOutInlineExpected.txt
    {
        const char* fileData =
R"(start
    //<>[COMMENT] this is comment//>
//<>[INCLUDE]["TestInputFile_TIncluded.tpl.txt"][TRIM]//>
11 //$
33 //$
//<>[ENTRY]["a"][ENTRY]["b"]//>    
11    //$
33    //$
//<>[ENTRY]["a"][ENTRY]["b"][ENTRY.]["a"][ENTRY.]["b"]//>
1111//$
1133//$
3311//$
3333//$
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutInlineExpected.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGeneratorOutInlineExpected2.txt
    {
        const char* fileData =
R"(start
    //<>[COMMENT] this is comment//>
//<>[INCLUDE]["TestInputFile_TIncluded.tpl.txt"][TRIM]//>
11 //$
33 //$
45 //$
//<>[ENTRY]["a"][ENTRY]["b"]//>    
11    //$
33    //$
45    //$
//<>[ENTRY]["a"][ENTRY]["b"][ENTRY.]["a"][ENTRY.]["b"]//>
1111//$
1133//$
1145//$
3311//$
3333//$
3345//$
4511//$
4533//$
4545//$
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorOutInlineExpected2.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGeneratorTemplate.txt
    {
        const char* fileData =
R"(start
[INCLUDE]["TestInputFile_TIncluded.tpl.txt"][TRIM]
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplate.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TGeneratorTemplateInline.txt
    {
        const char* fileData =
R"(start
    //<>[COMMENT] this is comment//>
//<>[INCLUDE]["TestInputFile_TIncluded.tpl.txt"][TRIM]//>
11 //$
33 //$
45 //$
//<>[ENTRY]["a"][ENTRY]["b"]//>    
11    //$
33    //$
45    //$
//<>[ENTRY]["a"][ENTRY]["b"][ENTRY.]["a"][ENTRY.]["b"]//>
1111//$
1133//$
1145//$
3311//$
3333//$
3345//$
4511//$
4533//$
4545//$
end)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TGeneratorTemplateInline.txt", fileData);
    }



    ///////////////////////////////////////////////////////////////////////////
    //TGenerator/TIncluded.tpl.txt
    {
        const char* fileData =
R"([ENTRY]["a"][ENTRY]["b"]
)";

        WriteTestInputFile(CCK_TEST_INPUT_FILE_PREFIX "TIncluded.tpl.txt", fileData);
    }


}
