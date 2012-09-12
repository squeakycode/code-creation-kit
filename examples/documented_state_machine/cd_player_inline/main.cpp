// This sample demonstrates inline templates.
#include <string>
#include <iostream>

#include "CDPlayerStatemachine.h"

///Tester class for the generated switch case state machine
class CDPlayerStatemachineTester : public CDPlayerStatemachine
{
public:
    virtual ~CDPlayerStatemachineTester() {}

    //<>[COMMENT] //List enter and exit for all states
    //<>[MACRO_BEGIN][TRIM]
    //<>///Called when state [ENTRY]["State Name"] is entered
    //<>virtual void enter[ENTRY]["State Name"]() 
    //<>{
    //<>    std::cout << "Entering [ENTRY]["State Name"]" << std::endl;
    //<>}
    //<>///Called when state [ENTRY]["State Name"] is exited
    //<>virtual void exit[ENTRY]["State Name"]() 
    //<>{
    //<>    std::cout << "Exiting [ENTRY]["State Name"]" << std::endl;
    //<>}

    //<>[MACRO_END][TRIM]
    ///Called when state Stopped is entered                                               //$
    virtual void enterStopped()                                                           //$
    {                                                                                     //$
        std::cout << "Entering Stopped" << std::endl;                                     //$
    }                                                                                     //$
    ///Called when state Stopped is exited                                                //$
    virtual void exitStopped()                                                            //$
    {                                                                                     //$
        std::cout << "Exiting Stopped" << std::endl;                                      //$
    }                                                                                     //$
    ///Called when state Playing is entered                                               //$
    virtual void enterPlaying()                                                           //$
    {                                                                                     //$
        std::cout << "Entering Playing" << std::endl;                                     //$
    }                                                                                     //$
    ///Called when state Playing is exited                                                //$
    virtual void exitPlaying()                                                            //$
    {                                                                                     //$
        std::cout << "Exiting Playing" << std::endl;                                      //$
    }                                                                                     //$
    ///Called when state Paused is entered                                                //$
    virtual void enterPaused()                                                            //$
    {                                                                                     //$
        std::cout << "Entering Paused" << std::endl;                                      //$
    }                                                                                     //$
    ///Called when state Paused is exited                                                 //$
    virtual void exitPaused()                                                             //$
    {                                                                                     //$
        std::cout << "Exiting Paused" << std::endl;                                       //$
    }                                                                                     //$
    ///Called when state Open is entered                                                  //$
    virtual void enterOpen()                                                              //$
    {                                                                                     //$
        std::cout << "Entering Open" << std::endl;                                        //$
    }                                                                                     //$
    ///Called when state Open is exited                                                   //$
    virtual void exitOpen()                                                               //$
    {                                                                                     //$
        std::cout << "Exiting Open" << std::endl;                                         //$
    }                                                                                     //$
    ///Called when state Forwarding is entered                                            //$
    virtual void enterForwarding()                                                        //$
    {                                                                                     //$
        std::cout << "Entering Forwarding" << std::endl;                                  //$
    }                                                                                     //$
    ///Called when state Forwarding is exited                                             //$
    virtual void exitForwarding()                                                         //$
    {                                                                                     //$
        std::cout << "Exiting Forwarding" << std::endl;                                   //$
    }                                                                                     //$
    
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
    CDPlayerStatemachineTester tester;
    
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
            //<>[MACRO_BEGIN][TRIM]
            //<>[BEGIN][IF][FIRST_TIME]if[OR]else if[END] ( tester.getCurrentState() == CDPlayerStatemachine::e[ENTRY]["State Name"] )
            //<>{
            //<>    std::cout << "[ENTRY]["State Name"]" << std::endl;
            //<>}
            //<>[MACRO_END][TRIM]
            if ( tester.getCurrentState() == CDPlayerStatemachine::eStopped )             //$
            {                                                                             //$
                std::cout << "Stopped" << std::endl;                                      //$
            }                                                                             //$
            else if ( tester.getCurrentState() == CDPlayerStatemachine::ePlaying )        //$
            {                                                                             //$
                std::cout << "Playing" << std::endl;                                      //$
            }                                                                             //$
            else if ( tester.getCurrentState() == CDPlayerStatemachine::ePaused )         //$
            {                                                                             //$
                std::cout << "Paused" << std::endl;                                       //$
            }                                                                             //$
            else if ( tester.getCurrentState() == CDPlayerStatemachine::eOpen )           //$
            {                                                                             //$
                std::cout << "Open" << std::endl;                                         //$
            }                                                                             //$
            else if ( tester.getCurrentState() == CDPlayerStatemachine::eForwarding )     //$
            {                                                                             //$
                std::cout << "Forwarding" << std::endl;                                   //$
            }                                                                             //$
            else
            {
                std::cout << "Unknown state." << std::endl;
            }
        }
        //<>[MACRO_BEGIN][TRIM]
        //<>//Is action [ENTRY]["Action Name"]?
        //<>else if ( line == "[ENTRY]["Action Name"]" )
        //<>{
        //<>    tester.action[ENTRY]["Action Name"]();
        //<>}
        //<>[MACRO_END][TRIM]
        //Is action Play?                                                                 //$
        else if ( line == "Play" )                                                        //$
        {                                                                                 //$
            tester.actionPlay();                                                          //$
        }                                                                                 //$
        //Is action Stop?                                                                 //$
        else if ( line == "Stop" )                                                        //$
        {                                                                                 //$
            tester.actionStop();                                                          //$
        }                                                                                 //$
        //Is action Pause?                                                                //$
        else if ( line == "Pause" )                                                       //$
        {                                                                                 //$
            tester.actionPause();                                                         //$
        }                                                                                 //$
        //Is action Open?                                                                 //$
        else if ( line == "Open" )                                                        //$
        {                                                                                 //$
            tester.actionOpen();                                                          //$
        }                                                                                 //$
        //Is action Close?                                                                //$
        else if ( line == "Close" )                                                       //$
        {                                                                                 //$
            tester.actionClose();                                                         //$
        }                                                                                 //$
        //Is action PressForward?                                                         //$
        else if ( line == "PressForward" )                                                //$
        {                                                                                 //$
            tester.actionPressForward();                                                  //$
        }                                                                                 //$
        //Is action ReleaseForward?                                                       //$
        else if ( line == "ReleaseForward" )                                              //$
        {                                                                                 //$
            tester.actionReleaseForward();                                                //$
        }                                                                                 //$
        //Is action EndOfMediaReached?                                                    //$
        else if ( line == "EndOfMediaReached" )                                           //$
        {                                                                                 //$
            tester.actionEndOfMediaReached();                                             //$
        }                                                                                 //$
        //Print all actions?
        else
        {
            //When unexpected input print help too
            if ( line != ">" )
            {
                PrintHelp();
            }
        
            std::cout << "Available Actions:" << std::endl;
            //<>std::cout << "[ENTRY]["Action Name"]" << std::endl;
            std::cout << "Play" << std::endl;                                             //$
            std::cout << "Stop" << std::endl;                                             //$
            std::cout << "Pause" << std::endl;                                            //$
            std::cout << "Open" << std::endl;                                             //$
            std::cout << "Close" << std::endl;                                            //$
            std::cout << "PressForward" << std::endl;                                     //$
            std::cout << "ReleaseForward" << std::endl;                                   //$
            std::cout << "EndOfMediaReached" << std::endl;                                //$
        }
    }
}
