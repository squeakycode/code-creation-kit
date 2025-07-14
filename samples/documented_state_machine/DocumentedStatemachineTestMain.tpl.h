[COMMENT] Required Parameters:
[COMMENT] "Machine Name" - the name of the class, e.g. CStateMachine
//------------------------------------------------------------------------------
/**
\file
\brief  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
*/
//------------------------------------------------------------------------------

#include <string>
#include <iostream>

#include "[ENTRY]["Machine Name"].gen.h"

///Tester class for the generated switch case state machine
class [ENTRY]["Machine Name"]Tester : public [ENTRY]["Machine Name"]
{
public:
    virtual ~[ENTRY]["Machine Name"]Tester() {}

    [COMMENT] //List enter and exit for all states     
    [MACRO_BEGIN][TRIM]
    ///Called when state [ENTRY]["State Name"] is entered
    virtual void enter[ENTRY]["State Name"]() 
    {
        std::cout << "Entering [ENTRY]["State Name"]" << std::endl;
    }
    ///Called when state [ENTRY]["State Name"] is exited
    virtual void exit[ENTRY]["State Name"]() 
    {
        std::cout << "Exiting [ENTRY]["State Name"]" << std::endl;
    }

    [MACRO_END][TRIM]
    
    ///Called when an action does not trigger a transition
    virtual void actionIgnored() 
    {
        std::cout << "Action Ignored" << std::endl;
    }
};

///Prints the help text
void PrintHelp()
{
    std::cout 
    << "Enter the action name to trigger an action." << std::endl 
    << "Enter '?' to print the state." << std::endl
    << "Enter '>' to print all actions." << std::endl
    << "Enter '<' to exit the test." << std::endl;
}

///Runs the state machine test
int main()
{
    [ENTRY]["Machine Name"]Tester tester;
    
    PrintHelp();
    
    for(;;)
    {
        //Read the user input
        std::string line;
        std::getline( std::cin, line);
        
        //Exit?
        if ( line == "<" )
        {
            break;
        }
        //Print State?
        if ( line == "?" )
        {
            [MACRO_BEGIN][TRIM]
            [BEGIN][IF][FIRST_TIME]if[OR]else if[END] ( tester.getCurrentState() == [ENTRY.]["Machine Name"]::e[ENTRY]["State Name"] )
            {
                std::cout << "[ENTRY]["State Name"]" << std::endl;
            }
            [MACRO_END][TRIM]
            else
            {
                std::cout << "Unknown state." << std::endl;            
            }
        }
        [MACRO_BEGIN][TRIM]
        //Is action [ENTRY]["Action Name"]?
        else if ( line == "[ENTRY]["Action Name"]" )
        {
            tester.action[ENTRY]["Action Name"]();
        }
        [MACRO_END][TRIM]
        //Print all actions?
        else
        {
            //When unexpected input print help too
            if ( line != ">" )
            {
                PrintHelp();
            }
        
            std::cout << "Available Actions:" << std::endl;
            std::cout << "[ENTRY]["Action Name"]" << std::endl;
        }
    }
}
