#ifndef Py_CEVAL_H
#define Py_CEVAL_H
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

/* Interface to random parts in ceval.c */

DL_IMPORT(PyObject *) PyEval_CallObjectWithKeywords
	Py_PROTO((PyObject *, PyObject *, PyObject *));

/* DLL-level Backwards compatibility: */
#undef PyEval_CallObject
DL_IMPORT(PyObject *) PyEval_CallObject Py_PROTO((PyObject *, PyObject *));

/* Inline this */
#define PyEval_CallObject(func,arg) \
        PyEval_CallObjectWithKeywords(func, arg, (PyObject *)NULL)

#ifdef HAVE_STDARG_PROTOTYPES
DL_IMPORT(PyObject *) PyEval_CallFunction Py_PROTO((PyObject *obj, char *format, ...));
DL_IMPORT(PyObject *) PyEval_CallMethod Py_PROTO((PyObject *obj,
				      char *methodname, char *format, ...));
#else
/* Better to have no prototypes at all for varargs functions in this case */
DL_IMPORT(PyObject *) PyEval_CallFunction();
DL_IMPORT(PyObject *) PyEval_CallMethod();
#endif

DL_IMPORT(PyObject *) PyEval_GetBuiltins Py_PROTO((void));
DL_IMPORT(PyObject *) PyEval_GetGlobals Py_PROTO((void));
DL_IMPORT(PyObject *) PyEval_GetLocals Py_PROTO((void));
DL_IMPORT(PyObject *) PyEval_GetOwner Py_PROTO((void));
DL_IMPORT(PyObject *) PyEval_GetFrame Py_PROTO((void));
DL_IMPORT(int) PyEval_GetRestricted Py_PROTO((void));

DL_IMPORT(int) Py_FlushLine Py_PROTO((void));

DL_IMPORT(int) Py_AddPendingCall Py_PROTO((int (*func) Py_PROTO((ANY *)), ANY *arg));
DL_IMPORT(int) Py_MakePendingCalls Py_PROTO((void));


/* Interface for threads.

   A module that plans to do a blocking system call (or something else
   that lasts a long time and doesn't touch Python data) can allow other
   threads to run as follows:

	...preparations here...
	Py_BEGIN_ALLOW_THREADS
	...blocking system call here...
	Py_END_ALLOW_THREADS
	...interpret result here...

   The Py_BEGIN_ALLOW_THREADS/Py_END_ALLOW_THREADS pair expands to a
   {}-surrounded block.
   To leave the block in the middle (e.g., with return), you must insert
   a line containing RET_SAVE before the return, e.g.

	if (...premature_exit...) {
		Py_BLOCK_THREADS
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

   An alternative is:

	Py_BLOCK_THREADS
	if (...premature_exit...) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	Py_UNBLOCK_THREADS

   For convenience, that the value of 'errno' is restored across
   Py_END_ALLOW_THREADS and RET_SAVE.

   WARNING: NEVER NEST CALLS TO Py_BEGIN_ALLOW_THREADS AND
   Py_END_ALLOW_THREADS!!!

   The function PyEval_InitThreads() should be called only from
   initthread() in "threadmodule.c".

   Note that not yet all candidates have been converted to use this
   mechanism!
*/

extern DL_IMPORT(PyThreadState *) PyEval_SaveThread Py_PROTO((void));
extern DL_IMPORT(void) PyEval_RestoreThread Py_PROTO((PyThreadState *));

#ifdef WITH_THREAD

extern DL_IMPORT(void) PyEval_InitThreads Py_PROTO((void));
extern DL_IMPORT(void) PyEval_AcquireLock Py_PROTO((void));
extern DL_IMPORT(void) PyEval_ReleaseLock Py_PROTO((void));
extern DL_IMPORT(void) PyEval_AcquireThread Py_PROTO((PyThreadState *tstate));
extern DL_IMPORT(void) PyEval_ReleaseThread Py_PROTO((PyThreadState *tstate));

#define Py_BEGIN_ALLOW_THREADS { \
			PyThreadState *_save; \
			_save = PyEval_SaveThread();
#define Py_BLOCK_THREADS	PyEval_RestoreThread(_save);
#define Py_UNBLOCK_THREADS	_save = PyEval_SaveThread();
#define Py_END_ALLOW_THREADS	PyEval_RestoreThread(_save); \
		 }

#else /* !WITH_THREAD */

#define Py_BEGIN_ALLOW_THREADS {
#define Py_BLOCK_THREADS
#define Py_UNBLOCK_THREADS
#define Py_END_ALLOW_THREADS }

#endif /* !WITH_THREAD */

#ifdef __cplusplus
}
#endif
#endif /* !Py_CEVAL_H */
