
#include <bld_system.h>
#define BUILD_LIB
#include <blade_ext.h>

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x004431DE
*/
#ifdef BLD_NATIVE
int WorldToMBW(const char *world)
{
    int (*bld_proc)(const char *world) = NULL;
    return bld_proc(world);
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/
