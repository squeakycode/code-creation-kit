            catch( [ENTRY]["Scope"]::[ENTRY]["Exception Name"]& e)
            {
                (void) e;

[BEGIN][IF][ENTRY]["Condition"][TRIM]
[MACRO_BEGIN.][IF.][ENTRY.]["Scope"][EQUALS.]["[ENTRY]["Scope"]"][IF.][ENTRY.]["Exception Name"][EQUALS.]["[ENTRY]["Exception Name"]"][IF.][ENTRY.]["Condition Group"][EQUALS.]["[ENTRY]["Condition Group"]"][TRIM.]
                [BEGIN.]if ([ENTRY.]["Condition"])[OR.]else[END.]
                {
                    //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
                    FormatT formatter(STRING_LITERAL("[BEGIN.][ENTRY.]["Prefix"] : just_testing [OR.] Just_Testing [END.]TC[ENTRY.]["Error Number"]: [ENTRY.]["Description"]\n"));
#else
                    FormatT formatter(STRING_LITERAL("[BEGIN.][ENTRY.]["Prefix"] : error [OR.] Error [END.]TC[ENTRY.]["Error Number"]: [ENTRY.]["Description"]\n"));
#endif
                    formatter% [ENTRY.]["Source"][MERGE.][" % "];
                    toErrorStream(formatter.str());
                }
[MACRO_END.][TRIM.]
[OR][TRIM]
                //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
                FormatT formatter(STRING_LITERAL("[BEGIN][ENTRY]["Prefix"] : just_testing [OR] Just_Testing [END]TC[ENTRY]["Error Number"]: [ENTRY]["Description"]\n"));
#else
                FormatT formatter(STRING_LITERAL("[BEGIN][ENTRY]["Prefix"] : error [OR] Error [END]TC[ENTRY]["Error Number"]: [ENTRY]["Description"]\n"));
#endif
                formatter % [ENTRY]["Source"][MERGE][" % "];
                toErrorStream( formatter.str());
[END][TRIM]
                throw CErrorPrinted();
            }
