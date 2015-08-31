[COMMENT] Produces a switch/case state machine class 
[COMMENT] Includes a dot graph description
[COMMENT] The graph is produced using Doxygen and Graphviz Dot
[COMMENT] see www.doxygen.org and www.graphviz.org 
[COMMENT] Required Parameters:
[COMMENT] "Machine Name" - the name of the class, e.g. CStateMachine
//--------------------------------------------------------------------
/**
\file
\brief  WARNING CONTAINS GENERATED CODE! ALL CHANGES WILL BE LOST!
*/
//--------------------------------------------------------------------

#ifndef INCLUDED_[ENTRY]["Machine Name"][TO_UPPER]_TPL_H
#define INCLUDED_[ENTRY]["Machine Name"][TO_UPPER]_TPL_H

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

//--------------------------------------------------------------------
/**
\class [ENTRY]["Machine Name"]
\brief State machine '[ENTRY]["Machine Name"]' with dot graph as 
        documentation generated from a table.

Derive from this class to handle the state transitions. 
 
\dot
digraph [ENTRY]["Machine Name"]
{
    start_state_[ENTRY]["Machine Name"] [shape=diamond,label="start"];
    start_state_[ENTRY]["Machine Name"] -> [ENTRY.]["State Name"];[IF.][ENTRY.]["Initial State"]
    [ENTRY]["State Name"];
    [COMMENT] first go top down through all actions, 
    [COMMENT] then go from left to right through states
    [COMMENT] expanding if transition is entered
    [ENTRY.]["State Name"] -> [ENTRY.]["[ENTRY]["Actions\\States"]"][label=[ENTRY]["Action Name"]];
}
\enddot
*/
//--------------------------------------------------------------------

class [ENTRY]["Machine Name"]
{
public:
    ///Lists all possible states
    enum EState
    {
        e[ENTRY]["State Name"], ///<[ENTRY]["State Description"][ANY]
        eNumberOfStates ///<Can be used for iterating over the states
    };
    
    [COMMENT] set initial state marked with yes (or anything else), 
    [COMMENT] build fails if not exactly one state is marked
    ///Initializes the state machine
    [ENTRY]["Machine Name"]()
        : [BEGIN]m_currentState(e[ENTRY]["State Name"][IF][ENTRY]["Initial State"])[OR][IF][LAST_TIME] ---initial state not set--- [END]
    {
    }
    
    ///-
    virtual ~[ENTRY]["Machine Name"]() {}

    [COMMENT] expand an action function for 
    [COMMENT] all entries in column "Action Name" as first step    
    [MACRO_BEGIN][TRIM]
    ///Handle action [ENTRY]["Action Name"]: [ENTRY]["Action Description"][ANY]
    virtual void action[ENTRY]["Action Name"]()
    {
        switch ( m_currentState)
        {
            [COMMENT] go from left to right through all states 
            [COMMENT] and add a transition case handler 
            [COMMENT] if a transition target state is entered
            [MACRO_BEGIN.][TRIM]
            case e[ENTRY.]["State Name"]:
                exit[ENTRY.]["State Name"]();
                [COMMENT] [ENTRY]["Actions\\States"] expands to 
                [COMMENT] the row name of the action
                enter[ENTRY.]["[ENTRY]["Actions\\States"]"]();
                m_currentState = e[ENTRY.]["[ENTRY]["Actions\\States"]"];                
                break;
            [MACRO_END.][TRIM]

            default:
            actionIgnored();
        }        
    }

    [MACRO_END][TRIM]

    [COMMENT] List enter and exit for all states     
    [MACRO_BEGIN][TRIM]
    ///Called when state [ENTRY]["State Name"] is entered
    virtual void enter[ENTRY]["State Name"]() {}
    ///Called when state [ENTRY]["State Name"] is exited
    virtual void exit[ENTRY]["State Name"]() {}

    [MACRO_END][TRIM]
    
    ///Called when an action does not trigger a transition
    virtual void actionIgnored() {}
    
    ///Returns the current state
    EState getCurrentState()
    {
        return m_currentState;
    }
private:
    EState m_currentState; ///< holds the current state of the machine
};

#endif /* INCLUDED_[ENTRY]["Machine Name"][TO_UPPER]_TPL_H */
