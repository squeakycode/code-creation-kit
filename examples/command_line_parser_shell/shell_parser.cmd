@echo off
setlocal

call :ParseOptions[] %*

echo.%#OPTION_HELP%
echo.%#OPTION_WAIT%
echo.%#OPTION_TIME%
echo.%#OPTION_EXTRA%
echo.%#OPTION_MYTIMEOUT%

exit /b 0

rem  ----- parses the command line options: call :ParseOptions[] %* to run-----
:ParseOptions[]
    @echo off
    
    set #OPTION_TIME=15
    set #OPTION_EXTRA=mode
    set #OPTION_MYTIMEOUT=10.5
    
    :ParseNext
        rem ----- check if we are done -----
        if "%1"=="" goto ParseDone
      
        rem ----- Option: -h --help-----
        if /I "%1" EQU "-h" (
            set #OPTION_HELP=%~2
            if "!#OPTION_HELP:~0,1!"=="-" (set #OPTION_HELP=) else (shift)
            shift
            goto ParseNext
        )
        if /I "%1" EQU "--help" (
            set #OPTION_HELP=%~2
            if "!#OPTION_HELP:~0,1!"=="-" (set #OPTION_HELP=) else (shift)
            shift
            goto ParseNext
        )
    
        rem ----- Option: -w --wait-----
        if /I "%1" EQU "-w" (
            set #OPTION_WAIT=%~2
            if "!#OPTION_WAIT:~0,1!"=="-" (set #OPTION_WAIT=) else (shift)
            shift
            goto ParseNext
        )
        if /I "%1" EQU "--wait" (
            set #OPTION_WAIT=%~2
            if "!#OPTION_WAIT:~0,1!"=="-" (set #OPTION_WAIT=) else (shift)
            shift
            goto ParseNext
        )
    
        rem ----- Option: -t-----
        if /I "%1" EQU "-t" (
            set #OPTION_TIME=%~2
            if "!#OPTION_TIME:~0,1!"=="-" (set #OPTION_TIME=) else (shift)
            shift
            goto ParseNext
        )
    
        rem ----- Option: -x --extra-----
        if /I "%1" EQU "-x" (
            set #OPTION_EXTRA=%~2
            if "!#OPTION_EXTRA:~0,1!"=="-" (set #OPTION_EXTRA=) else (shift)
            shift
            goto ParseNext
        )
        if /I "%1" EQU "--extra" (
            set #OPTION_EXTRA=%~2
            if "!#OPTION_EXTRA:~0,1!"=="-" (set #OPTION_EXTRA=) else (shift)
            shift
            goto ParseNext
        )
    
        rem ----- Option: --my-timeout-----
        if /I "%1" EQU "--my-timeout" (
            set #OPTION_MYTIMEOUT=%~2
            if "!#OPTION_MYTIMEOUT:~0,1!"=="-" (set #OPTION_MYTIMEOUT=) else (shift)
            shift
            goto ParseNext
        )
    
    
    :ParseDone
    
    rem  ----- check if options are complete -----
    rem ----- Option: -w --wait-----    
    if not defined #OPTION_WAIT (
      echo.Error: Parameter -w or --wait not specified. 1>&2
      call :DisplayUsage[] 1>&2
      exit /b 1
    )
    
exit /b 0

:DisplayUsage[]
    echo.
    echo.Valid options are:
    echo.
    echo.-h     --help                 Print help.
    echo.
    echo.-w     --wait                 Wait for keypress before exit.
    echo.
    echo.-t                            Time to run in minutes. This option takes an
    echo.                              unsigned int as parameter.
    echo.
    echo.                              Default Value:
    echo.                              15
    echo.
    echo.-x     --extra                Extra value. This option takes a string as
    echo.                              parameter.
    echo.
    echo.                              Default Value:
    echo.                              mode
    echo.
    echo.       --my-timeout           Large description block:
    echo.                              
    echo.                              1234567890123456789012345678901234567890123456789
    echo.                              0
    echo.                              A      
    echo.                              B
    echo.                              C
    echo.                              text text text text text text text text text text
    echo.                              text text text text text text text text 
    echo.                              texttexttexttexttexttexttexttexttexttexttexttextt
    echo.                              exttexttexttexttexttexttexttexttexttexttexttext
    echo.                              This option takes a double as parameter.
    echo.
    echo.                              Default Value:
    echo.                              10.5
    echo.

exit /b 0