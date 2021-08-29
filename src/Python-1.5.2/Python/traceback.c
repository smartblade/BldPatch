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

/* Traceback implementation */

#include "Python.h"

#include "compile.h"
#include "frameobject.h"
#include "structmember.h"
#include "osdefs.h"

typedef struct _tracebackobject {
	PyObject_HEAD
	struct _tracebackobject *tb_next;
	PyFrameObject *tb_frame;
	int tb_lasti;
	int tb_lineno;
} tracebackobject;

#define OFF(x) offsetof(tracebackobject, x)

static struct memberlist tb_memberlist[] = {
	{"tb_next",	T_OBJECT,	OFF(tb_next)},
	{"tb_frame",	T_OBJECT,	OFF(tb_frame)},
	{"tb_lasti",	T_INT,		OFF(tb_lasti)},
	{"tb_lineno",	T_INT,		OFF(tb_lineno)},
	{NULL}	/* Sentinel */
};

static PyObject *
tb_getattr(tb, name)
	tracebackobject *tb;
	char *name;
{
	return PyMember_Get((char *)tb, tb_memberlist, name);
}

static void
tb_dealloc(tb)
	tracebackobject *tb;
{
	Py_XDECREF(tb->tb_next);
	Py_XDECREF(tb->tb_frame);
	PyMem_DEL(tb);
}

#define Tracebacktype PyTraceBack_Type
#define is_tracebackobject PyTraceBack_Check

PyTypeObject Tracebacktype = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"traceback",
	sizeof(tracebackobject),
	0,
	(destructor)tb_dealloc, /*tp_dealloc*/
	0,		/*tp_print*/
	(getattrfunc)tb_getattr, /*tp_getattr*/
	0,		/*tp_setattr*/
	0,		/*tp_compare*/
	0,		/*tp_repr*/
	0,		/*tp_as_number*/
	0,		/*tp_as_sequence*/
	0,		/*tp_as_mapping*/
};

static tracebackobject *
newtracebackobject(next, frame, lasti, lineno)
	tracebackobject *next;
	PyFrameObject *frame;
	int lasti, lineno;
{
	tracebackobject *tb;
	if ((next != NULL && !is_tracebackobject(next)) ||
			frame == NULL || !PyFrame_Check(frame)) {
		PyErr_BadInternalCall();
		return NULL;
	}
	tb = PyObject_NEW(tracebackobject, &Tracebacktype);
	if (tb != NULL) {
		Py_XINCREF(next);
		tb->tb_next = next;
		Py_XINCREF(frame);
		tb->tb_frame = frame;
		tb->tb_lasti = lasti;
		tb->tb_lineno = lineno;
	}
	return tb;
}

int
PyTraceBack_Here(frame)
	PyFrameObject *frame;
{
	PyThreadState *tstate = frame->f_tstate;
	tracebackobject *oldtb = (tracebackobject *) tstate->curexc_traceback;
	tracebackobject *tb = newtracebackobject(oldtb,
				frame, frame->f_lasti, frame->f_lineno);
	if (tb == NULL)
		return -1;
	tstate->curexc_traceback = (PyObject *)tb;
	Py_XDECREF(oldtb);
	return 0;
}

static int
tb_displayline(f, filename, lineno, name)
	PyObject *f;
	char *filename;
	int lineno;
	char *name;
{
	int err = 0;
	FILE *xfp;
	char linebuf[1000];
	int i;
	if (filename == NULL || name == NULL)
		return -1;
#ifdef MPW
	/* This is needed by MPW's File and Line commands */
#define FMT "  File \"%.900s\"; line %d # in %s\n"
#else
	/* This is needed by Emacs' compile command */
#define FMT "  File \"%.900s\", line %d, in %s\n"
#endif
	xfp = fopen(filename, "r");
	if (xfp == NULL) {
		/* Search tail of filename in sys.path before giving up */
		PyObject *path;
		char *tail = strrchr(filename, SEP);
		if (tail == NULL)
			tail = filename;
		else
			tail++;
		path = PySys_GetObject("path");
		if (path != NULL && PyList_Check(path)) {
			int npath = PyList_Size(path);
			int taillen = strlen(tail);
			char namebuf[MAXPATHLEN+1];
			for (i = 0; i < npath; i++) {
				PyObject *v = PyList_GetItem(path, i);
				if (v == NULL) {
					PyErr_Clear();
					break;
				}
				if (PyString_Check(v)) {
					int len;
					len = PyString_Size(v);
					if (len + 1 + taillen >= MAXPATHLEN)
						continue; /* Too long */
					strcpy(namebuf, PyString_AsString(v));
					if ((int)strlen(namebuf) != len)
						continue; /* v contains '\0' */
					if (len > 0 && namebuf[len-1] != SEP)
						namebuf[len++] = SEP;
					strcpy(namebuf+len, tail);
					xfp = fopen(namebuf, "r");
					if (xfp != NULL) {
						filename = namebuf;
						break;
					}
				}
			}
		}
	}
	sprintf(linebuf, FMT, filename, lineno, name);
	err = PyFile_WriteString(linebuf, f);
	if (xfp == NULL || err != 0)
		return err;
	for (i = 0; i < lineno; i++) {
		if (fgets(linebuf, sizeof linebuf, xfp) == NULL)
			break;
	}
	if (i == lineno) {
		char *p = linebuf;
		while (*p == ' ' || *p == '\t' || *p == '\014')
			p++;
		err = PyFile_WriteString("    ", f);
		if (err == 0) {
			err = PyFile_WriteString(p, f);
			if (err == 0 && strchr(p, '\n') == NULL)
				err = PyFile_WriteString("\n", f);
		}
	}
	fclose(xfp);
	return err;
}

static int
tb_printinternal(tb, f, limit)
	tracebackobject *tb;
	PyObject *f;
	int limit;
{
	int err = 0;
	int depth = 0;
	tracebackobject *tb1 = tb;
	while (tb1 != NULL) {
		depth++;
		tb1 = tb1->tb_next;
	}
	while (tb != NULL && err == 0) {
		if (depth <= limit) {
			if (Py_OptimizeFlag)
				tb->tb_lineno = PyCode_Addr2Line(
					tb->tb_frame->f_code, tb->tb_lasti);
			err = tb_displayline(f,
			    PyString_AsString(
				    tb->tb_frame->f_code->co_filename),
			    tb->tb_lineno,
			    PyString_AsString(tb->tb_frame->f_code->co_name));
		}
		depth--;
		tb = tb->tb_next;
		if (err == 0)
			err = PyErr_CheckSignals();
	}
	return err;
}

int
PyTraceBack_Print(v, f)
	PyObject *v;
	PyObject *f;
{
	int err;
	PyObject *limitv;
	int limit = 1000;
	if (v == NULL)
		return 0;
	if (!is_tracebackobject(v)) {
		PyErr_BadInternalCall();
		return -1;
	}
	limitv = PySys_GetObject("tracebacklimit");
	if (limitv && PyInt_Check(limitv)) {
		limit = PyInt_AsLong(limitv);
		if (limit <= 0)
			return 0;
	}
	err = PyFile_WriteString("Traceback (innermost last):\n", f);
	if (!err)
		err = tb_printinternal((tracebackobject *)v, f, limit);
	return err;
}
