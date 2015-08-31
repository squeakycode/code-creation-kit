#include <string>

class CDataClass
{
public:
    //declaration
    [ENTRY]["Type"] m_[ENTRY]["Name"]; //[ENTRY]["Description"][ANY]
    
    //constructor
    CDataClass()
        [MACRO_BEGIN][BEGIN][IF][FIRST_TIME]: [OR], [END]m_[ENTRY]["Name"]([ENTRY]["Default"])[MACRO_END]
    {
    }
};
