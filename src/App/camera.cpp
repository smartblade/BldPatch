#include <App/camera.h>


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/

/*
* Module:                 Blade.exe
* Entry point:            0x00418E69
* VC++ mangling:          ?SetTravelingView@B_Camera@@QAEHHH@Z
*/
#ifndef BLD_NATIVE
int B_Camera::SetTravelingView(int s_type, int t_type)
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
* Entry point:            0x004195CE
* VC++ mangling:          ?SetPersonView@B_Camera@@QAEHPBD@Z
*/
#ifndef BLD_NATIVE
int B_Camera::SetPersonView(const char *person_name)
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
* Entry point:            0x0041A86B
* VC++ mangling:          ?Read3dsMaxAnimation@B_Camera@@QAEHPBDHH@Z
*/
#ifndef BLD_NATIVE
int B_Camera::Read3dsMaxAnimation(
    const char *cam_file_name, int i_unknown, int num_frames)
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
* Entry point:            0x0041AD83
* VC++ mangling:          ?DeleteEvent@B_Camera@@QAEHH@Z
*/
#ifndef BLD_NATIVE
int B_Camera::DeleteEvent(int frame)
{
    return 0;
}
#endif

/*
* Module:                 Blade.exe
* Entry point:            0x0041AE6D
* VC++ mangling:          ?AddEvent@B_Camera@@QAEXHPAUPyObject@@@Z
*/
#ifndef BLD_NATIVE
void B_Camera::AddEvent(int frame, PyObject *func)
{
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
* Entry point:            0x004201E8
* VC++ mangling:          ?AddSourceNode@B_Camera@@QAEHNABVB_Vector@@@Z
*/

int B_Camera::AddSourceNode(double time, const B_Vector &position)
{
    return source.AddNode(time, position);
}


/*
................................................................................
................................................................................
................................................................................
................................................................................
*/
