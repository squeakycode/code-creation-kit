@echo off
echo Error cannot run template file.& exit /b 0 &[IF][NOT][FIRST_TIME]
setlocal

call :ParseOptions[] %*

echo.%#OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]%

exit /b 0

rem  ----- parses the command line options: call :ParseOptions[] %* to run-----
:ParseOptions[]
    @echo off
    
    set #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]=[ENTRY]["Default Value"]
    
    :ParseNext
        rem ----- check if we are done -----
        if "%1"=="" goto ParseDone
      
    [MACRO_BEGIN][BEGIN][IF][ENTRY]["Short Option Name"][OR][IF][ENTRY]["Option Name"][END][TRIM]
        rem ----- Option:[BEGIN] -[ENTRY]["Short Option Name"][OR][END][BEGIN] --[ENTRY]["Option Name"][OR][END]-----
        [BEGIN][TRIM]
        if /I "%1" EQU "-[ENTRY]["Short Option Name"]" (
            set #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]=%~2
            if "!#OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]:~0,1!"=="-" (set #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]=) else (shift)
            shift
            goto ParseNext
        )
        [OR][END][TRIM]
        [BEGIN][TRIM]
        if /I "%1" EQU "--[ENTRY]["Option Name"]" (
            set #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]=%~2
            if "!#OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]:~0,1!"=="-" (set #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER]=) else (shift)
            shift
            goto ParseNext
        )
        [OR][END][TRIM]  
    
    [MACRO_END][TRIM]
    
    :ParseDone
    
    rem  ----- check if options are complete -----
    [MACRO_BEGIN][BEGIN][IF][ENTRY]["Short Option Name"][OR][IF][ENTRY]["Option Name"][END][IF][NOT][ENTRY]["Is Optional"][AS_VOLATILE][TRIM]
    rem ----- Option: -[ENTRY]["Short Option Name"] --[ENTRY]["Option Name"]-----    
    if not defined #OPTION_[ENTRY]["Option Name Identifier"][TO_UPPER] (
      echo.Error: Parameter [BEGIN]-[ENTRY]["Short Option Name"] [END][BEGIN][IF][ENTRY]["Short Option Name"][IF][ENTRY]["Option Name"]or [END][BEGIN]--[ENTRY]["Option Name"] [END]not specified. 1>&2
      call :DisplayUsage[] 1>&2
      exit /b 1
    )
    
    [MACRO_END][TRIM]
exit /b 0

:DisplayUsage[]
    echo.
    echo.Valid options are:
    echo.
    [MACRO_BEGIN][BEGIN][IF][ENTRY]["Short Option Name"][OR][IF][ENTRY]["Option Name"][END][TRIM]
    echo.[BEGIN]-[ENTRY]["Short Option Name"][PAD_RIGHT][" ",5][OR]      [END][BEGIN] --[ENTRY]["Option Name"][PAD_RIGHT][" ",21][OR]                        [END][ENTRY]["Description"][BLOCK_FORMAT][49][REPLACE]["\n","\n    echo.                              "]
    [BEGIN][TRIM]
    echo.
    echo.                              Default Value:
    echo.                              [ENTRY]["Default Value"][BLOCK_FORMAT][49][REPLACE]["\n","\n    echo.                              "]
    [OR][END][TRIM]
    echo.
    [MACRO_END][TRIM]

exit /b 0