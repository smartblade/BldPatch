#ifndef Py_PYTHONRUN_H
#define Py_PYTHONRUN_H
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
The Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI or Corporation for National Research Initiatives or
CNRI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

While CWI is the initial source for this software, a modified version
is made available by the Corporation for National Research Initiatives
(CNRI) at the Internet address ftp://ftp.python.org.

STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

******************************************************************/

/* Interfaces to parse and execute pieces of python code */

DL_IMPORT(void) Py_SetProgramName Py_PROTO((char *));
DL_IMPORT(char *) Py_GetProgramName Py_PROTO((void));

DL_IMPORT(void) Py_SetPythonHome Py_PROTO((char *));
DL_IMPORT(char *) Py_GetPythonHome Py_PROTO((void));

DL_IMPORT(void) Py_Initialize Py_PROTO((void));
DL_IMPORT(void) Py_Finalize Py_PROTO((void));
DL_IMPORT(int) Py_IsInitialized Py_PROTO((void));
DL_IMPORT(PyThreadState *) Py_NewInterpreter Py_PROTO((void));
DL_IMPORT(void) Py_EndInterpreter Py_PROTO((PyThreadState *));

DL_IMPORT(int) PyRun_AnyFile Py_PROTO((FILE *, char *));

DL_IMPORT(int) PyRun_SimpleString Py_PROTO((char *));
DL_IMPORT(int) PyRun_InteractiveString Py_PROTO((char *));
DL_IMPORT(int) PyRun_SimpleFile Py_PROTO((FILE *, char *));
DL_IMPORT(int) PyRun_InteractiveOne Py_PROTO((FILE *, char *));
DL_IMPORT(int) PyRun_InteractiveLoop Py_PROTO((FILE *, char *));

DL_IMPORT(struct _node *) PyParser_SimpleParseString Py_PROTO((char *, int));
DL_IMPORT(struct _node *) PyParser_SimpleParseFile Py_PROTO((FILE *, char *, int));

DL_IMPORT(PyObject *) PyRun_String Py_PROTO((char *, int, PyObject *, PyObject *));
DL_IMPORT(PyObject *) PyRun_File Py_PROTO((FILE *, char *, int, PyObject *, PyObject *));

DL_IMPORT(PyObject *) Py_CompileString Py_PROTO((char *, char *, int));

DL_IMPORT(void) PyErr_Print Py_PROTO((void));
DL_IMPORT(void) PyErr_PrintEx Py_PROTO((int));

DL_IMPORT(int) Py_AtExit Py_PROTO((void (*func) Py_PROTO((void))));

DL_IMPORT(void) Py_Exit Py_PROTO((int));

DL_IMPORT(int) Py_FdIsInteractive Py_PROTO((FILE *, char *));

/* In getpath.c */
DL_IMPORT(char *) Py_GetProgramFullPath Py_PROTO((void));
DL_IMPORT(char *) Py_GetPrefix Py_PROTO((void));
DL_IMPORT(char *) Py_GetExecPrefix Py_PROTO((void));
DL_IMPORT(char *) Py_GetPath Py_PROTO((void));

/* In their own files */
DL_IMPORT(const char *) Py_GetVersion Py_PROTO((void));
DL_IMPORT(const char *) Py_GetPlatform Py_PROTO((void));
DL_IMPORT(const char *) Py_GetCopyright Py_PROTO((void));
DL_IMPORT(const char *) Py_GetCompiler Py_PROTO((void));
DL_IMPORT(const char *) Py_GetBuildInfo Py_PROTO((void));

/* Internal -- various one-time initializations */

DL_IMPORT(PyObject *) _PyBuiltin_Init_1 Py_PROTO((void));
DL_IMPORT(void) _PyBuiltin_Init_2 Py_PROTO((PyObject *));
DL_IMPORT(PyObject *) _PySys_Init Py_PROTO((void));
DL_IMPORT(void) _PyImport_Init Py_PROTO((void));

/* Various internal finalizers */
DL_IMPORT(void) _PyImport_Fini Py_PROTO((void));
DL_IMPORT(void) _PyBuiltin_Fini_1 Py_PROTO((void));
DL_IMPORT(void) _PyBuiltin_Fini_2 Py_PROTO((void));
DL_IMPORT(void) PyMethod_Fini Py_PROTO((void));
DL_IMPORT(void) PyFrame_Fini Py_PROTO((void));
DL_IMPORT(void) PyCFunction_Fini Py_PROTO((void));
DL_IMPORT(void) PyTuple_Fini Py_PROTO((void));
DL_IMPORT(void) PyString_Fini Py_PROTO((void));
DL_IMPORT(void) PyInt_Fini Py_PROTO((void));
DL_IMPORT(void) PyFloat_Fini Py_PROTO((void));
DL_IMPORT(void) PyOS_FiniInterrupts Py_PROTO((void));

/* Stuff with no proper home (yet) */
DL_IMPORT(char *) PyOS_Readline Py_PROTO((char *));
extern DL_IMPORT(int) (*PyOS_InputHook) Py_PROTO((void));
extern DL_IMPORT(char) *(*PyOS_ReadlineFunctionPointer) Py_PROTO((char *));

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYTHONRUN_H */
