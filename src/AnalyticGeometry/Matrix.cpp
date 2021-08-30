#include <AnalyticGeometry/Matrix.h>

#include <bld_system.h>
#include <AnalyticGeometry/Vector.h>


/*
* Module:                 Blade.exe
* Entry point:            0x00404BE0
* VC++ mangling:          ??0B_Matrix@@QAE@XZ
*/
#ifdef BLD_NATIVE
B_Matrix::B_Matrix()
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
* Entry point:            0x00404F2F
* VC++ mangling:          ??0B_Matrix@@QAE@ABVB_Vector@@@Z
*/
#ifdef BLD_NATIVE
B_Matrix::B_Matrix(const B_Vector &v)
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
* Entry point:            0x0040571F
* VC++ mangling:          ?TranslationVector@B_Matrix@@QBE?AVB_Vector@@XZ
*/
#ifdef BLD_NATIVE
B_Vector B_Matrix::TranslationVector() const
{
    return B_Vector();
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
* Entry point:            0x406903
* VC++ mangling:          ??D@YA?AVB_Vector@@ABV0@ABVB_Matrix@@@Z
*/
#ifdef BLD_NATIVE
B_Vector operator *(const B_Vector& vector, const B_Matrix &matrix)
{
    return vector;
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
* Entry point:            0x00406AF1
* VC++ mangling:          ?Rotate@@YA?AVB_Vector@@ABV1@ABVB_Matrix@@@Z
*/
#ifdef BLD_NATIVE
B_Vector Rotate(const B_Vector &v, const B_Matrix &rotateMatrix)
{
    return v;
}
#endif

/*
................................................................................
................................................................................
................................................................................
................................................................................
*/