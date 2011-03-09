        catch( [ENTRY]["Scope"]::[ENTRY]["Exception Name"]& e)
        {
            (void) e;
            
            //prevent test output to be listed as error
#if defined BOOST_TEST_MAIN
            FormatT formatter(STRING_LITERAL("[BEGIN][ENTRY]["Prefix"] : just_testing [OR] Just_Testing [END]TC[ENTRY]["Error Number"]: [ENTRY]["Description"]\n"));
#else
            FormatT formatter(STRING_LITERAL("[BEGIN][ENTRY]["Prefix"] : error [OR] Error [END]TC[ENTRY]["Error Number"]: [ENTRY]["Description"]\n"));
#endif
            formatter % [ENTRY]["Source"][MERGE][" % "];
            toErrorStream( formatter.str());
            throw CErrorPrinted();
        }
