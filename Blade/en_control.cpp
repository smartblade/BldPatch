
#include "en_control.h"

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x00595124
* VC++ mangling:          ?SetTurnSpeed@EnControl@@QAEHPBDN@Z
*/
#ifdef BLD_NATIVE
int EnControl::SetTurnSpeed(const char *race_name, double new_speed)
{
    return 0;
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x00595166
* VC++ mangling:          ?SetAnimationFactor@EnControl@@QAEHPBDN@Z
*/
#ifdef BLD_NATIVE
int EnControl::SetAnimationFactor(const char *mov, double new_speed_factor)
{
    return 0;
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x005954F7
* VC++ mangling:          ?AddWatchAnim@EnControl@@QAEHPBD@Z
*/
#ifdef BLD_NATIVE
int EnControl::AddWatchAnim(const char *anim)
{
    return 0;
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x0059650C
* VC++ mangling:          ?StartRecordingDemo@EnControl@@QAEHXZ
*/

#ifdef BLD_NATIVE

int EnControl::StartRecordingDemo()
{
    return 0;
}

#endif


/*
* Module:                 Blade.exe
* Entry point:            0x0059654C
* VC++ mangling:          ?EndRecordingDemo@EnControl@@QAEHPBD@Z
*/

#ifdef BLD_NATIVE

int EnControl::EndRecordingDemo(const char *name)
{
    return 0;
}

#endif


/*
* Module:                 Blade.exe
* Entry point:            0x005967DD
* VC++ mangling:          ?PlayDemo@EnControl@@QAEHPBD@Z
*/

#ifdef BLD_NATIVE

int EnControl::PlayDemo(const char *name)
{
    return 0;
}

#endif
