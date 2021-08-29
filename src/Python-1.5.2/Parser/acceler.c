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

/* Parser accelerator module */

/* The parser as originally conceived had disappointing performance.
   This module does some precomputation that speeds up the selection
   of a DFA based upon a token, turning a search through an array
   into a simple indexing operation.  The parser now cannot work
   without the accelerators installed.  Note that the accelerators
   are installed dynamically when the parser is initialized, they
   are not part of the static data structure written on graminit.[ch]
   by the parser generator. */

#include "pgenheaders.h"
#include "grammar.h"
#include "node.h"
#include "token.h"
#include "parser.h"

/* Forward references */
static void fixdfa Py_PROTO((grammar *, dfa *));
static void fixstate Py_PROTO((grammar *, state *));

void
PyGrammar_AddAccelerators(g)
	grammar *g;
{
	dfa *d;
	int i;
#ifdef Py_DEBUG
	fprintf(stderr, "Adding parser accelerators ...\n");
#endif
	d = g->g_dfa;
	for (i = g->g_ndfas; --i >= 0; d++)
		fixdfa(g, d);
	g->g_accel = 1;
#ifdef Py_DEBUG
	fprintf(stderr, "Done.\n");
#endif
}

void
PyGrammar_RemoveAccelerators(g)
	grammar *g;
{
	dfa *d;
	int i;
	g->g_accel = 0;
	d = g->g_dfa;
	for (i = g->g_ndfas; --i >= 0; d++) {
		state *s;
		int j;
		s = d->d_state;
		for (j = 0; j < d->d_nstates; j++, s++) {
			if (s->s_accel)
				PyMem_DEL(s->s_accel);
			s->s_accel = NULL;
		}
	}
}

static void
fixdfa(g, d)
	grammar *g;
	dfa *d;
{
	state *s;
	int j;
	s = d->d_state;
	for (j = 0; j < d->d_nstates; j++, s++)
		fixstate(g, s);
}

static void
fixstate(g, s)
	grammar *g;
	state *s;
{
	arc *a;
	int k;
	int *accel;
	int nl = g->g_ll.ll_nlabels;
	s->s_accept = 0;
	accel = PyMem_NEW(int, nl);
	for (k = 0; k < nl; k++)
		accel[k] = -1;
	a = s->s_arc;
	for (k = s->s_narcs; --k >= 0; a++) {
		int lbl = a->a_lbl;
		label *l = &g->g_ll.ll_label[lbl];
		int type = l->lb_type;
		if (a->a_arrow >= (1 << 7)) {
			printf("XXX too many states!\n");
			continue;
		}
		if (ISNONTERMINAL(type)) {
			dfa *d1 = PyGrammar_FindDFA(g, type);
			int ibit;
			if (type - NT_OFFSET >= (1 << 7)) {
				printf("XXX too high nonterminal number!\n");
				continue;
			}
			for (ibit = 0; ibit < g->g_ll.ll_nlabels; ibit++) {
				if (testbit(d1->d_first, ibit)) {
#ifdef applec
#define MPW_881_BUG			/* Undefine if bug below is fixed */
#endif
#ifdef MPW_881_BUG
					/* In 881 mode MPW 3.1 has a code
					   generation bug which seems to
					   set the upper bits; fix this by
					   explicitly masking them off */
					int temp;
#endif
					if (accel[ibit] != -1)
						printf("XXX ambiguity!\n");
#ifdef MPW_881_BUG
					temp = 0xFFFF &
						(a->a_arrow | (1 << 7) |
						 ((type - NT_OFFSET) << 8));
					accel[ibit] = temp;
#else
					accel[ibit] = a->a_arrow | (1 << 7) |
						((type - NT_OFFSET) << 8);
#endif
				}
			}
		}
		else if (lbl == EMPTY)
			s->s_accept = 1;
		else if (lbl >= 0 && lbl < nl)
			accel[lbl] = a->a_arrow;
	}
	while (nl > 0 && accel[nl-1] == -1)
		nl--;
	for (k = 0; k < nl && accel[k] == -1;)
		k++;
	if (k < nl) {
		int i;
		s->s_accel = PyMem_NEW(int, nl-k);
		if (s->s_accel == NULL) {
			fprintf(stderr, "no mem to add parser accelerators\n");
			exit(1);
		}
		s->s_lower = k;
		s->s_upper = nl;
		for (i = 0; k < nl; i++, k++)
			s->s_accel[i] = accel[k];
	}
	PyMem_DEL(accel);
}
