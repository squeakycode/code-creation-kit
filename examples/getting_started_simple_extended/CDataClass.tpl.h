#include <string>

class [ENTRY]["Class Name"]
{
public:
    //declaration
    [ENTRY.]["[ENTRY]["Type"]"] m_[ENTRY]["Name"]; //[ENTRY]["Description"][ANY]
    
    //constructor
    [ENTRY]["Class Name"]()
        [BEGIN][BEGIN][IF][FIRST_TIME]: [OR], [END]m_[ENTRY]["Name"]([ENTRY]["Default"])[END]
    {
    }
    
    unsigned int Clip()
    {
        unsigned int clipCounter = 0;

        //clip max
[MACRO_BEGIN]        if ( m_[ENTRY]["Name"] > [ENTRY]["Max"] )
        {
            m_[ENTRY]["Name"] = [ENTRY]["Max"];
            ++clipCounter;        
        }
[MACRO_END]
 
         //clip min
        [MACRO_BEGIN][TRIM]
        if ( m_[ENTRY]["Name"] < [ENTRY]["Min"] )
        {
            m_[ENTRY]["Name"] = [ENTRY]["Min"];
            ++clipCounter;        
        }
        [MACRO_END][TRIM]

        return clipCounter;
    }
};
