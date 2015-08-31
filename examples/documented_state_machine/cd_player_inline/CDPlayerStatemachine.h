//--------------------------------------------------------------------
/**
\file
\brief  CDPlayerStateMachine with inline templates
*/
//--------------------------------------------------------------------

#ifndef INCLUDED_CDPLAYERSTATEMACHINEINLINE_H_5742456
#define INCLUDED_CDPLAYERSTATEMACHINEINLINE_H_5742456

//--------------------------------------------------------------------
/**
\class CDPlayerStatemachine
\brief State machine CDPlayerStatemachine with dot graph as 
       documentation generated from a table.

Derive from this class to handle the state transitions. 
 
\dot
digraph CDPlayerStatemachine
{
    //<>start_state_[ENTRY]["Machine Name"] [shape=diamond,label="start"];
    //<>start_state_[ENTRY]["Machine Name"] -> [ENTRY.]["State Name"];[IF.][ENTRY.]["Initial State"]
    //<>[ENTRY]["State Name"];
    Stopped;                                                                              //$
    Playing;                                                                              //$
    Paused;                                                                               //$
    Open;                                                                                 //$
    Forwarding;                                                                           //$
    //<>[COMMENT] first go top down through all actions, 
    //<>[COMMENT] then go from left to right through states
    //<>[COMMENT] expanding if transition is entered
    //<>[ENTRY.]["State Name"] -> [ENTRY.]["[ENTRY]["Actions\\States"]"][label=[ENTRY]["Action Name"]];
    Stopped -> Playing[label=Play];                                                       //$
    Paused -> Playing[label=Play];                                                        //$
    Playing -> Stopped[label=Stop];                                                       //$
    Paused -> Stopped[label=Stop];                                                        //$
    Playing -> Paused[label=Pause];                                                       //$
    Stopped -> Open[label=Open];                                                          //$
    Playing -> Open[label=Open];                                                          //$
    Paused -> Open[label=Open];                                                           //$
    Forwarding -> Open[label=Open];                                                       //$
    Open -> Stopped[label=Close];                                                         //$
    Playing -> Forwarding[label=PressForward];                                            //$
    Forwarding -> Playing[label=ReleaseForward];                                          //$
    Playing -> Stopped[label=EndOfMediaReached];                                          //$
    Forwarding -> Stopped[label=EndOfMediaReached];                                       //$
}
\enddot
*/
//--------------------------------------------------------------------
class CDPlayerStatemachine
{
public:
    /** \brief Lists all possible states.*/
    enum EState
    {
        //<>[MACRO_BEGIN][TRIM]
        //<>/** \brief [ENTRY]["State Description"][ANY]*/
        //<>e[ENTRY]["State Name"],
        //<>[MACRO_END][TRIM]
        /** \brief The player is stopped.*/                                               //$
        eStopped,                                                                         //$
        /** \brief The player is playing.*/                                               //$
        ePlaying,                                                                         //$
        /** \brief The player is paused.*/                                                //$
        ePaused,                                                                          //$
        /** \brief The player is open. The media can be exchanged.*/                      //$
        eOpen,                                                                            //$
        /** \brief The player is fast forwarding.*/                                       //$
        eForwarding,                                                                      //$
        /** \brief Can be used for iterating over the states.*/
        eNumberOfStates 
    };
    
    //<>[COMMENT] set initial state marked with yes (or anything else), 
    //<>[COMMENT] build fails if not exactly one state is marked
    /** \brief Initializes the state machine.*/
    CDPlayerStatemachine()
        //<>: [BEGIN]m_currentState(e[ENTRY]["State Name"][IF][ENTRY]["Initial State"])[OR][IF][LAST_TIME] ---initial state not set--- [END]
        : m_currentState(eStopped)                                                        //$
    {
    }
    
    /** \brief Make the destructor virtual.*/
    virtual ~CDPlayerStatemachine()
    {
    }

    //<>[COMMENT] expand an action function for 
    //<>[COMMENT] all entries in column "Action Name" as first step
    //<>[MACRO_BEGIN][TRIM]
    //<>/** \brief Handle action [ENTRY]["Action Name"]: [ENTRY]["Action Description"][ANY]*/
    //<>virtual void action[ENTRY]["Action Name"]()
    //<>{
    //<>    switch ( m_currentState)
    //<>    {
    //<>        [COMMENT] go from left to right through all states 
    //<>        [COMMENT] and add a transition case handler 
    //<>        [COMMENT] if a transition target state is entered
    //<>        [MACRO_BEGIN.][TRIM.]
    //<>        case e[ENTRY.]["State Name"]:
    //<>            exit[ENTRY.]["State Name"]();
    //<>            [COMMENT] [ENTRY]["Actions\\States"] expands to 
    //<>            [COMMENT] the row name of the action
    //<>            enter[ENTRY.]["[ENTRY]["Actions\\States"]"]();
    //<>            m_currentState = e[ENTRY.]["[ENTRY]["Actions\\States"]"];
    //<>            break;
    //<>        [MACRO_END.][TRIM.]
    //<>        
    //<>        default:
    //<>        actionIgnored();
    //<>    }
    //<>}
    //<>
    //<>[MACRO_END][TRIM]
    /** \brief Handle action Play: The user pressed the play button.*/                    //$
    virtual void actionPlay()                                                             //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case eStopped:                                                                //$
                exitStopped();                                                            //$
                enterPlaying();                                                           //$
                m_currentState = ePlaying;                                                //$
                break;                                                                    //$
            case ePaused:                                                                 //$
                exitPaused();                                                             //$
                enterPlaying();                                                           //$
                m_currentState = ePlaying;                                                //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action Stop: The user pressed the stop button.*/                    //$
    virtual void actionStop()                                                             //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case ePlaying:                                                                //$
                exitPlaying();                                                            //$
                enterStopped();                                                           //$
                m_currentState = eStopped;                                                //$
                break;                                                                    //$
            case ePaused:                                                                 //$
                exitPaused();                                                             //$
                enterStopped();                                                           //$
                m_currentState = eStopped;                                                //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action Pause: The user pressed the pause button.*/                  //$
    virtual void actionPause()                                                            //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case ePlaying:                                                                //$
                exitPlaying();                                                            //$
                enterPaused();                                                            //$
                m_currentState = ePaused;                                                 //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action Open: The user opened the player.*/                          //$
    virtual void actionOpen()                                                             //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case eStopped:                                                                //$
                exitStopped();                                                            //$
                enterOpen();                                                              //$
                m_currentState = eOpen;                                                   //$
                break;                                                                    //$
            case ePlaying:                                                                //$
                exitPlaying();                                                            //$
                enterOpen();                                                              //$
                m_currentState = eOpen;                                                   //$
                break;                                                                    //$
            case ePaused:                                                                 //$
                exitPaused();                                                             //$
                enterOpen();                                                              //$
                m_currentState = eOpen;                                                   //$
                break;                                                                    //$
            case eForwarding:                                                             //$
                exitForwarding();                                                         //$
                enterOpen();                                                              //$
                m_currentState = eOpen;                                                   //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action Close: The user closed the player.*/                         //$
    virtual void actionClose()                                                            //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case eOpen:                                                                   //$
                exitOpen();                                                               //$
                enterStopped();                                                           //$
                m_currentState = eStopped;                                                //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action PressForward: The user presses the forward button.*/         //$
    virtual void actionPressForward()                                                     //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case ePlaying:                                                                //$
                exitPlaying();                                                            //$
                enterForwarding();                                                        //$
                m_currentState = eForwarding;                                             //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action ReleaseForward: The user released the forward button.*/      //$
    virtual void actionReleaseForward()                                                   //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case eForwarding:                                                             //$
                exitForwarding();                                                         //$
                enterPlaying();                                                           //$
                m_currentState = ePlaying;                                                //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$
    /** \brief Handle action EndOfMediaReached: The playing logic detected that the end of the cd is reached.*///$
    virtual void actionEndOfMediaReached()                                                //$
    {                                                                                     //$
        switch ( m_currentState)                                                          //$
        {                                                                                 //$
            case ePlaying:                                                                //$
                exitPlaying();                                                            //$
                enterStopped();                                                           //$
                m_currentState = eStopped;                                                //$
                break;                                                                    //$
            case eForwarding:                                                             //$
                exitForwarding();                                                         //$
                enterStopped();                                                           //$
                m_currentState = eStopped;                                                //$
                break;                                                                    //$
                                                                                          //$
            default:                                                                      //$
            actionIgnored();                                                              //$
        }                                                                                 //$
    }                                                                                     //$
                                                                                          //$

    //<>[COMMENT] List enter and exit for all states
    //<>[MACRO_BEGIN][TRIM]
    //<>/** \brief Called when state [ENTRY]["State Name"] is entered.*/
    //<>virtual void enter[ENTRY]["State Name"]() {}
    //<>/** \brief Called when state [ENTRY]["State Name"] is exited.*/
    //<>virtual void exit[ENTRY]["State Name"]() {}
    //<>                                                  
    //<>[MACRO_END][TRIM]
    /** \brief Called when state Stopped is entered.*/                                    //$
    virtual void enterStopped() {}                                                        //$
    /** \brief Called when state Stopped is exited.*/                                     //$
    virtual void exitStopped() {}                                                         //$
                                                                                          //$
    /** \brief Called when state Playing is entered.*/                                    //$
    virtual void enterPlaying() {}                                                        //$
    /** \brief Called when state Playing is exited.*/                                     //$
    virtual void exitPlaying() {}                                                         //$
                                                                                          //$
    /** \brief Called when state Paused is entered.*/                                     //$
    virtual void enterPaused() {}                                                         //$
    /** \brief Called when state Paused is exited.*/                                      //$
    virtual void exitPaused() {}                                                          //$
                                                                                          //$
    /** \brief Called when state Open is entered.*/                                       //$
    virtual void enterOpen() {}                                                           //$
    /** \brief Called when state Open is exited.*/                                        //$
    virtual void exitOpen() {}                                                            //$
                                                                                          //$
    /** \brief Called when state Forwarding is entered.*/                                 //$
    virtual void enterForwarding() {}                                                     //$
    /** \brief Called when state Forwarding is exited.*/                                  //$
    virtual void exitForwarding() {}                                                      //$
                                                                                          //$
    
    /** \brief Called when an action does not trigger a transition.*/
    virtual void actionIgnored() {}
    
    /** \brief Returns the current state.*/
    EState getCurrentState()
    {
        return m_currentState;
    }
private:
    /** \brief holds the current state of the machine.*/
    EState m_currentState; 
};

#endif /* INCLUDED_CDPLAYERSTATEMACHINEINLINE_H_5742456 */
