#include "ConfigSections.h"


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 rOpenGL.dll
* Entry point:            0x1000CEAB
* VC++ mangling:          ?Read@B_VideoModeSection@@QAEHXZ
*/
#ifndef BLD_NATIVE
int B_VideoModeSection::Read()
{
    return true;
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/