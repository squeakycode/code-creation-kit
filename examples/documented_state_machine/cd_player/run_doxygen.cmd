rem run doxygen, turn default GENERATE_LATEX off
cmd /c "doxygen -s -g - & echo GENERATE_LATEX=NO" | doxygen -
rem view the result
start html\annotated.html