/*****************************************************************\
*       32-bit or 64-bit BBC BASIC Interpreter                    *
*       (C) 2017-2022  R.T.Russell  http://www.rtrussell.co.uk/   *
*                                                                 *
*       The name 'BBC BASIC' is the property of the British       *
*       Broadcasting Corporation and used with their permission   *
*                                                                 *
*       bbexec2.c: Variable assignment and statement execution    *
*                  Pico version to minimise stack use             *
*       Version 1.28a, 11-Jan-2022                                *
\*****************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "BBC.h"

// Routines in bbmain:
int range1 (char);		// Test char for valid in a variable name
signed char nxt (void);	// Skip spaces, handle line continuation
void check (void);		// Check for running out of memory
void error (int, const char*);	// Process an error
void outchr (unsigned char);	// Output a character
void text (const char *);	// Output NUL-terminated string
unsigned short report (void);	// Build an error string in accs
void crlf (void);		// Output a newline
char *allocs (STR *, int);	// Reallocate a string
char *alloct (int);		// Allocate temporary string space
heapptr *pushs (VAR);		// Push string on stack
char *moves (STR *, int);	// Move string into temporary buffer
void braket (void);		// Check for closing parenthesis
void comma (void);		// Check for a comma
void clear (void);		// Clear dynamic variables etc.
void clrtrp (void);		// Clear ON event handlers
signed char *findl (unsigned int);	// Find a specified line number or label
int arrlen (void **);		// Count elements in an array
void *getvar (unsigned char*);	// Get a variable's pointer and type
void *getdim (unsigned char*);
void *getdef (unsigned char*);
void *getput (unsigned char*);
void *putvar (void *, unsigned char*);
void *create (unsigned char **, unsigned char *);
void *putdef (void *);
signed char *gettop (signed char *, unsigned short *);
signed char *search (signed char *, signed char);
unsigned short setlin (signed char *, char **);

// Routines in bbeval:
VAR expr (void);		// Evaluate an expression
VAR exprn (void);		// Evaluate a numeric expression
VAR exprs (void);		// Evaluate a string expression
long long itemi (void);	// Evaluate an integer item
long long expri (void);	// Evaluate an integer expression
void fixs (VAR);		// Copy to accs and terminate with CR
VAR cons (void);		// Get a constant (quoted) string
VAR val (void);		// Get a numeric constant from accs
int str (VAR, char *, int);	// Convert number to decimal string
int strhex (VAR, char*, int);	// Convert number to hexadecimal string
void *channel (void);		// Get a file channel number
int expra (void *, int, unsigned char); // Evaluate an array expression
VAR loadn (void *, unsigned char); // Load a numeric from memory
VAR loads (void *, unsigned char); // Load a string from memory
VAR math (VAR, signed char, VAR);  // Perform arithmetic

// Routines in bbcmos:
heapptr xtrap (void);		// Handle an event interrupt etc.
void oswrch (unsigned char);	// Output a byte
void oswait (int);		// Pause for a specified time
void osline (char *);		// Get a line of console input
void putime (int);		// Store centisecond ticks
void mouse (int*, int*, int*);	// Get mouse state
void mouseon (int);		// Set mouse cursor (pointer)
void mouseoff (void);		// Hide mouse cursor
void mouseto (int, int);	// Move mouse cursor
void *sysadr (char *);		// Get the address of an API function
size_t guicall (void *, PARM *);	// Call function in GUI thread context
heapptr oshwm (void *, int);	// Allocate memory above HIMEM
int oscall (int);		// Call an emulated OS function
void envel (signed char *);	// ENVELOPE statement
void quiet (void);		// Disable sound generation
void sound (short, signed char, unsigned char, unsigned char); // SOUND statement
void *osopen (int, char *);	// Open a file
unsigned char osbget (void *, int*); // Read a byte from a file
void osbput (void *, unsigned char); // Write a byte to a file
void setptr (void *, long long);	// Set the file pointer
long long getext (void *);	// Get file length
void osshut (void *);		// Close file(s)
void osload (char*, void *, int); // Load a file to memory

// Routines in bbasmb:
void assemble (void);

// Routines in bbccli:
void oscli (char*);		// OSCLI

// Routines in bbcvdu:
long long apicall_ (void *, PARM *);
double fltcall_ (void *, PARM *);

void spaces (int num)
{
	while (num-- > 0)
		outchr (' ');
}

static void dotab (void)
{
	int x = expri ();
	if (*esi == ',')
	    {
		int y;
		esi++;
		y = expri ();
		braket ();
		oswrch (31);
		oswrch (x);
		oswrch (y);
		return;
	    }
	braket ();
	if (vcount == x)
		return;
	if (vcount > x)
		crlf ();
	spaces (x - vcount);
}

static int format (signed char al)
{
	switch (al)
	    {
		case 0x27:
			crlf ();
			return 1;

		case TTAB:
			dotab ();
			return 1;

		case TSPC:
			spaces (itemi ());
			return 1;
	    }
return 0;
}

static void ptext (char *p, int l)
{
	int i;
	for (i = 0; i < l; i++)
		outchr (*p++);
}

static void equals (void)
{
	if (nxt () != '=')
		error (4, NULL); // 'Mistake'
	esi++;
}

// Store numeric variable:
void storen (VAR v, void *ptr, unsigned char type)
{
	int n;
	switch (type)
	    {
		case 1:
			if (v.i.t)
				v.i.n = v.f;
			n = v.i.n;
			if (v.i.n != n)
				error (20, NULL); // 'Number too big'
			*(unsigned char*)ptr = n;
			break;

		case 4:
			if (v.i.t)
				v.i.n = v.f;
			n = v.i.n;
			if (v.i.n != n)
				error (20, NULL); // 'Number too big'
			ISTORE(ptr, n);
			break;

		case 5:
			{
			long long l;
			int e;

			if (v.i.t == 0)
				v.f = v.i.n;
			v.d.d = v.f;

			l = ((v.i.n & 0x7FFFFFFFFFFFFFFFL) + 0x100000) >> 21;
			e = (l >> 31) - 895;
			if (v.d.d < 0) l |= 0x80000000; else l &= 0x7FFFFFFF;
			if (e > 255)
				error (20, NULL); // 'Number too big'
			if (e <= 0)
			    {
				ISTORE(ptr, 0);
				* ((char *)ptr+4) = 0;
			    }
			else
			    {
				ISTORE(ptr, l);
				* ((char *)ptr+4) = e;
			    }
			}
			break;

		case 8:
			{
			if (v.i.t == 0)
				v.f = v.i.n;
			v.d.d = v.f;
			// *(int *)ptr = (int) v.s.p;
			// *(int *)((char *)ptr + 4) = v.s.l;
			memcpy (ptr, &v.s.p, 8); // may be unaligned
			}
			break;

		case 10:
			// *(int *)ptr = (int) v.s.p;
			// *(int *)((char *)ptr + 4) = v.s.l;
			// *(short *)((char *)ptr + 8) = v.s.t;
			memcpy (ptr, &v.s.p, 10); // may be unaligned
			break;

		case 40:
			if (v.i.t)
			    {
				long long t = v.f;
				if (t != truncl(v.f))
					error (20, NULL); // 'Number too big'
				v.i.n = t;
			    }
			// *(int *)ptr = (int) v.s.p;
			// *(int *)((char *)ptr + 4) = v.s.l;
			memcpy (ptr, &v.s.p, 8); // may be unaligned
			break;

		case 36:
			VSTORE(ptr, (void *) (size_t) v.i.n);
			break;

		default:
			error (6, NULL); // 'Type mismatch'
	    }
}

// Store string variable:
void stores (VAR v, void *ptr, unsigned char type)
{
	if (ptr == NULL)
		return;
	switch (type)
	    {
		case 128:
			memmove (ptr, v.s.p + zero, v.s.l);
			*((char *)ptr + v.s.l) = 0x0D;
			break;

		case 130:
			memmove (ptr, v.s.p + zero, v.s.l);
			*((char *)ptr + v.s.l) = 0;
			break;

		case 136:
			if ((v.s.l != 0) && (v.s.l == tmps.l) && (v.s.p == tmps.p))
			    {
				// Source string is pointed to by the tmps descriptor so simply
				// swap the descriptors rather than copying the string contents:
				STR swap = tmps;
				memcpy(&tmps, ptr, sizeof(STR));
				memcpy(ptr, &swap, sizeof(STR));
				break;
			    }
			memmove (allocs ((STR *)ptr, v.s.l), v.s.p + zero, v.s.l);
			break;

		default:
			error (6, NULL); // 'Type mismatch'
	    }
}

// Modify numeric variable:
void modify (VAR v, void *ptr, unsigned char type, signed char op)
{
	if (op == '=')
	    {
		storen (v, ptr, type);
		return;
	    }
	storen (math (loadn (ptr, type), op, v), ptr, type);
} 

// Modify string variable:
void modifs (VAR v, void *ptr, unsigned char type, signed char op)
{
	VAR s;
	char *tmp;
	if (op == '=')
	    {
		stores (v, ptr, type);
		return;
	    }
	if (op != '+')
		error (6, NULL); // 'Type mismatch'

	s = loads (ptr, type);
	tmp = moves ((STR *) &v, s.s.l);
	memmove (tmp, s.s.p + zero, s.s.l);
	s.s.p = tmp - (char *) zero;
	s.s.l += v.s.l;
	stores (s, ptr, type);
} 

// Assign to a numeric variable (supports compound assignment operators):
static void assign (void *ptr, unsigned char type)
{
	signed char op = nxt ();
	esi++;
	if (op != '=')
	    {
		if ((op == '+') || (op == '-') || (op == '*') || (op == '/') ||
			((op >= TAND) & (op <= TOR)))
			equals ();
		else
			error (4, NULL); // Mistake
	    }
	modify (exprn (), ptr, type, op);
}

// Assign to a string variable (supports compound += operator):
static void assigns (void *ptr, unsigned char type)
{
	signed char op = nxt ();
	esi++;
	if (op != '=')
	    {
		if (op == '+')
			equals ();
		else
			error (4, NULL); // Mistake
	    }
	modifs (exprs (), ptr, type, op);
}

// Test for statement terminator:
static int termq (void)
{
	signed char al = nxt ();
	return (al == TELSE) || (al == ':') || (al == 0x0D);
}

// Skip a quoted (constant) string:
static void quote (void)
{
	signed char al;
	while ((al = *esi++) != '"')
	    {
		if (al == 0x0D)
			error (9, NULL); // 'Missing "'
	    }
}

// Skip until end of statement:
static void skip (void)
{
	while (!termq ())
	    {
		signed char al = *esi++;
		if (al == TLINO) esi += 3;
		if (al == '"') quote ();
	    }
}

// Search forward for a token, which may be anywhere in a line
// Handle nested inner structures (nest token ignored after EXIT)
static void wsurch (signed char token, signed char nest, int level)
{
	while (1)
	    {
		signed char al = *esi++;
		if (al == '"')
			quote ();
		else if (al == TREM)
			esi = memchr (esi, 0x0D, 255);
		else if (al == TEXIT)
		    {
			nxt ();
			esi++;
		    }
		else if ((al == token) && (--level == 0))
			return; // found
		else if (al == nest)
			level++;
		else if (al == 0x0D)
		    {
			if (*esi == 0)
				return; // not found
			esi += 3;
			if (*esi == TDATA)
				esi = memchr (esi, 0x0D, 255);
		    }
	    }
}

// Search forward for one of two tokens at the start of a line.
// Handle nesting of structures ('nest' looked for at end of line,
// 'unnest' looked for at start of line).
// The initial level should normally be set to zero.
static signed char *nsurch (signed char tok1, signed char tok2,
			signed char nest, signed char unnest, int level)
{
	while (1)
	    {
		int n = (int)*(unsigned char*)esi;
		if (n == 0)
			return NULL; // Not found
		if ((level == 0) && ((tok1 == *(esi+3)) || (tok2 == *(esi+3))))
			return esi;
		if (unnest == *(esi+3))
			level--;
		if (level < 0)
			return esi;
		esi += n;
		if ((n > 4) && (nest == *(esi-2)))
			level++;
	    }
}

// Get a (possibly quoted) string to string accumulator:
static int fetchs (char **psrc)
{
	VAR v;
	char *src = *psrc;
	char *dst = accs;
	while (*src == ' ') src++;
	if (*src == '"')
	    {
		signed char *oldesi = esi;
		esi = (signed char*) src + 1;
		v = cons ();
		nxt ();
		fixs (v);
		*psrc = (char *) esi;
		esi = oldesi;
		return v.s.l;
	    }
	while ((*src != ',') && (*src != 0x0D))
		*dst++ = *src++;
	*psrc = src;
	*dst = 0x0D;
	return dst - accs;
}

// Test for being inside a function or procedure:
static void isloc (void)
{
	int eax = *(int *)esp;
	if ((eax != FNCHK) && (eax != PROCHK) && (eax != LOCCHK) &&
	    (eax != RETCHK) && (eax != DIMCHK) && (eax != LDCHK) && (eax != ONCHK))
		error (12, NULL); // 'Not in a FN or PROC'
}

// Send a PLOT command:
static void plot (int n, int x, int y)
{
	oswrch (25);
	oswrch (n);
	oswrch (x);
	oswrch (x >> 8);
	oswrch (y);
	oswrch (y >> 8);
}

// Create a 'secret' variable name based on code pointer, for use by PRIVATE:
static char *secret (char *p, unsigned char type)
{
	unsigned int i, ebp = 54 , eax = esi - (signed char *) zero;
	eax = (eax << 1) | (eax > ((void *) esp - zero)); // library bit in LSB
	for (i = 0; i < 4; i++)
	    {
		unsigned char al = 'A' + (eax % ebp);
		if (al > 'Z') al += '_' - '[';
		if (al > 'z') al -= '{' - '0';
		*p++ = al;
		eax = eax / ebp;
		ebp = 64;
	    }
	*p++ = '@';
	switch (type)
	    {
		case STYPE:
			*p++ = '{';
			*p++ = '}';
			break;

		case 40:
			*p++ = '%';
		case 4:
			*p++ = '%';
			break;

		case 8:
			*p++ = '#';
			break;

		case 1:
			*p++ = '&';
			break;

		case 128:
		case 130:
		case 136:
			*p++ = '$';
	    }
	return p;
}

// Free scalar strings and string arrays contained in a structure:
static void undims (void *ebp, void *ebx)
{
	VAR v;
	v.s.t = -1;
	v.s.l = 0;
	v.s.p = 0;

	ebp += 4; // skip length field
	while (1)
	    {
		void *edx = ebp; // pointer to link
		ebp += 4; // bump past link
		ebp = memchr (ebp, 0, 255) + 1;// scan for NUL
		char al = *((char *)ebp - 2);	// type character
		if (al == '$')			// scalar string
		    {
			ebp = ebx + ILOAD(ebp);
			stores (v, ebp, 0x88);	// free string
		    }
		else if ((al == '(') && (*((char *)ebp - 3) == '$'))	// string array
		    {
			void *edi = ebx + ILOAD(ebp); // string pointer
			ebp += 4; // skip data offset
			int eax = arrlen (&ebp);
			while (eax--)
			    {
				stores (v, edi, 0x88);
				edi += 8; // string descriptor GCC extension: sizeof(void) = 1
			    }
		    }
		else if (al == '{')		// sub-structure
		    {
			undims (VLOAD(ebp), ebx + ILOAD(ebp + sizeof(void *)));
		    }
		ebp = edx + ILOAD(edx); // follow relative link (GCC extension)
		if (ebp == edx)
			break;
	    }
}

// Release stack used by LOCAL structure or array or structure-array
// Also free any strings contained in the structure or array
static void undim (void)
{
	unsigned char type;
	void *data, *desc, *varptr;
	int size;
	VAR v = {0};

	esp++;	// bump past DIMCHK
	type = (unsigned char)(int) *esp++;
	varptr = *(void **)esp; esp += STRIDE;
	desc = *(void **)esp; esp += STRIDE;
	data = *(void **)esp; esp += STRIDE;
	size = (int) *esp++;
	if (varptr != NULL)
	    {
		USTORE(varptr , 1); // restore LOCAL marker
		if (type == STYPE) // scalar structure
		    {
			undims (desc, data);
		    }
		if (type == 0xC8) // string array
		    {
			int n = arrlen (&desc);
			while (n--)
			    {
				stores (v, desc, 0x88);
				desc += 8; // GCC extension sizeof(void) = 1
			    }
		    }
		if (type == (STYPE + 0x40)) // structure array
		    {
			int n = arrlen (&desc);
			while (n--)
			    {
				undims (VLOAD(desc), VLOAD(desc + sizeof(void *)));
				desc += 2 * sizeof(size_t);
			    }				
		    }
	    }
	esp += size >> 2;
}

// Save a LOCAL variable or FN/PROC formal variable to stack:
static void savloc (void *ptr, unsigned char type)
{
	VAR v;
	if (type & BIT6)
		type = ATYPE; // array pointer

	if (type == STYPE)
	    {
		esp -= STRIDE * 2;
		*((void **)esp + 1) = VLOAD(ptr + sizeof(void*)); // struct data ptr
		*(void **)esp       = VLOAD(ptr); // struct format ptr
	    }
	else if (type < 128)
	    {
		v = loadn (ptr, type);
		*--esp = (int)v.s.t;
		*--esp = v.s.l;
		*--esp = v.s.p; // Assumed 32-bits
	    }
	else if (type == 136) 
	    {
		// can't use long long because of ARM alignment requirements
		*(int *)--esp = ILOAD((char *)ptr + 4); // string length
		*(int *)--esp = ILOAD(ptr); // string pointer
	    }
	else
	    {
		v = loads (ptr, type);
		int n = (v.s.l + 3) & -4;
		if (n > ((char *)esp - (char *)zero - pfree - STACK_NEEDED))
			error (0, NULL); // No room
		esp -= n >> 2;
		memcpy ((char *) esp, v.s.p + zero, v.s.l);
		*--esp = v.s.l; // length
	    }
	esp -= STRIDE;
	*(void **)esp = ptr;
	*--esp = (int)type;
	*--esp = LOCCHK;
	check ();
}

// Restore LOCALs or formal parameters from stack:
static void resloc (void)
{
	unsigned char type;
	void *ptr;
	do
	    {
		esp++; // discard LOCCHK
		type = (int) *esp++;
		ptr = *(void **)esp;
		esp += STRIDE;

		if (type == STYPE)
		    {
			if (ptr != NULL)
			    {
				VSTORE(ptr, *(void **)esp); // struct format ptr
				VSTORE(ptr + sizeof(void*), *((void **)esp + 1)); // struct data ptr
			    }
			esp += STRIDE * 2;
		    }
		else if (type < 128)
		    {
			if (ptr != NULL)
				storen (NLOAD(esp), ptr, type);
			esp += 3;
		    }
		else if (type == 136)
		    {
			VAR v;
			v.s.l = 0;
			v.s.p = 0;
			if (ptr != NULL)
			    {
				stores (v, ptr, type); // free local string
				// can't use long long because of ARM alignment requirements
				ISTORE(ptr, *(int *)esp++); // string pointer
				ISTORE((char *)ptr + 4, *(int *)esp++); // string length
			    }
			else
				esp += 2;
		    }
		else
		    {
			VAR v;
			v.s.l = (int) *esp++;
			v.s.p = (void *) esp - zero;
			if (ptr != NULL)
				stores (v, ptr, type);
			esp += (v.s.l + 3) >> 2;
		    }
	    }
	while (*(int *)esp == LOCCHK);
}

// Restore variables from stack, used by both 'unret' and 'argue':
static void unstack (int count)
{
	while (count--)
	    {
		unsigned char type = (int) *esp++;
		void *ptr = *(void **)esp;
		esp += STRIDE;

		if (type & BIT6)
		    {
			VSTORE(ptr, *(void **)esp); // array pointer
			esp += STRIDE;
		    }
		else if (type & BIT4)
		    {
			VSTORE(ptr, *(void **)esp); // struct format ptr
			VSTORE(ptr + sizeof(void*), *((void **)esp + 1)); // struct data ptr
			esp += STRIDE * 2;
		    }
		else if (type < 128)
		    {
			storen (NLOAD(esp), ptr, type); // numeric
			esp += 3;
		    }
		else
		    {
			VAR v;
			v.s.l = *(int *)esp++;
			v.s.p = (void *) esp - zero;
			stores (v, ptr, type);
			esp += (v.s.l + 3) >> 2;
		    }
	    }
}

// Update RETURNed parameters or PRIVATE variables on return fron FN/PROC:
// Do it via the stack to support PROC1(A,B)  DEF PROC1(RETURN B,RETURN A)
static void unret (void)
{
	esp++; 
	int count = *(int *)esp++;
	if (count == 0)
		return;
	heapptr *edi = esp;
	int n = count;

	while (n--)
	    {
		unsigned char srctype = (int) *edi++; // formal/private type
		void *srcptr = *(void **)edi;
		edi += STRIDE;
		unsigned char dsttype = (int) *edi++; // actual/secret type
		void *dstptr = *(void **)edi;
		edi += STRIDE;

		if (srctype & BIT6)
		    {
			esp -= STRIDE;
			*(void **)esp = VLOAD(srcptr); // array pointer
		    }
		else if (srctype & BIT4)
		    {
			esp -= STRIDE * 2;
			*((void **)esp + 1) = VLOAD(srcptr + sizeof(void*)); // struct data ptr
			*(void **)esp       = VLOAD(srcptr); // struct format ptr
		    }
		else if (srctype < 128)
		    {
			VAR v = loadn (srcptr, srctype);
			*--esp = (int)v.s.t;
			*--esp = v.s.l;
			*--esp = v.s.p;
		    }
		else
		    {
			VAR v = loads (srcptr, srctype);
			pushs (v);
			*--esp = v.s.l;
		    }

		esp -= STRIDE;
		*(void **)esp = dstptr;
		*--esp = (int) dsttype;
	    }

	unstack (count);
	esp = edi;
}

// Transfer actual parameters to formal parameters via the stack:
static signed char* argue (signed char *ebx, heapptr *edi, int flag)
{
	int i, count = 0;
	void *ptr;
	signed char *tmp;
	signed char delim;
	unsigned char type;

	do
	    {
		unsigned char otype;
		void *optr;

		esi++;
		ebx++;
		if ((nxt () == TRETURN) || flag)
		    {
			if (*esi == TRETURN)
				esi++;
			nxt ();
			ptr = getput (&type);
			if (type & BIT6)
				type = ATYPE;
			*edi++ = (int)type; // formal/private type
			*(void **)edi = ptr; // formal/private ptr
			edi += STRIDE;

			tmp = esi; esi = ebx; ebx = tmp; // swap
			nxt ();
			optr = getput (&otype);
			if (otype & BIT6)
			    {
				if (ILOAD(optr) == 1)
					error (14, NULL); // Don't allow a LOCAL array
				otype = ATYPE;
 			    }
			*edi++ = (int)otype; // actual/secret type
			*(void **)edi = optr; // actual/secret pointer
			edi += STRIDE;

			if ((type ^ otype) & BIT7)
				error (6, NULL); // 'Type mismatch'

			if (type == STYPE)
			    {
				if (ILOAD(optr) == 1)
					error (56, NULL); // Don't allow a LOCAL structure
				esp -= STRIDE * 2;
				*((void **)esp + 1) = VLOAD(optr + sizeof(void*)); // struct data ptr
				*(void **)esp       = VLOAD(optr); // struct format ptr
			    }
			else if (type < 128)
			    {
				VAR v = loadn (optr, otype);
				*--esp = (int)v.s.t;
				*--esp = v.s.l;
				*--esp = v.s.p;
			    }
			else
			    {
				VAR v = loads (optr, otype);
				pushs (v);
				*--esp = v.s.l;
			    }
		    }
		else
		    {

			ptr = getput (&type);

			tmp = esi; esi = ebx; ebx = tmp; // swap
			if (type & (BIT4 | BIT6))
			    {
				nxt ();
				optr = getvar (&otype);
				if (optr == NULL)
					error (16, NULL); // 'Syntax error'
				if (otype == 0)
					error (26, NULL); // 'No such variable'
				if (type != otype)
					error (6, NULL); // 'Type mismatch'
				if (type == STYPE)
				    {
					esp -= STRIDE;
					*(void **)esp = VLOAD(optr + sizeof(void*)); // struct data ptr
				    }
				esp -= STRIDE;
				*(void **)esp = VLOAD(optr); // array pointer/struct fmt pointer
			    }
			else if (type < 128)
			    {
				VAR v = exprn ();
				*--esp = (int)v.s.t;
				*--esp = v.s.l;
				*--esp = v.s.p;
			    }
			else
			    {
				VAR v = exprs ();
				pushs (v);
				*--esp = v.s.l;
			    }
		    }

		esp -= STRIDE;
		*(void **)esp = ptr;
		*--esp = (int) type;
		count ++;

		delim = nxt ();
		tmp = esi; esi = ebx; ebx = tmp; // swap
	    }
	while ((nxt () == ',') && (delim == ','));
	if ((*esi == ',') || (delim == ','))
		error (31, NULL); // 'Incorrect arguments'

	tmp = esi; esi = ebx; ebx = tmp; // swap

// Have now finished stacking the parameters.
// Check all the (movable) string formal parameters to confirm that
// a user-defined function actual parameter hasn't moved any of them,
// because that leaves BB4W in an unstable state ('Geoff Webb' bug).
// Then orphan those formal strings so they are preserved in the heap.

	for (i = 0; i < count; i++)
	    {
		edi++; // skip LOCCHK
		unsigned char type = (unsigned char) *(int *)edi++;
		void *ebp = *(void **)edi;
		edi += STRIDE;
		if (type == STYPE)
			edi += STRIDE * 2;
		else if (type < 128)
			edi += 3;
		else if (type == 136)
		    {
			heapptr sptr = *edi++; // string pointer
			edi++; // skip string length
			if (sptr != ULOAD(ebp))
				error (31, NULL); // 'Incorrect arguments'
			USTORE(ebp, 0); // zero string pointer
			USTORE(ebp + 4, 0); //  zero string length (GCC extension)
		    }
		else
			edi += (((int) *edi + 3) >> 2) + 1;
	    }

// Unstack values of actual parameters into formal parameters:

	unstack (count);

	return ebx;
}

// Adjust the format pointers in nested structures to point into stack:
static void fixup (void *edi, int ebx)
{
	edi += 4; // bump past size field (GCC extension)
	while (1)
	    {
		void *edx = edi;
		edi = memchr (edi + 4, 0, 255); // search for NUL
		if (*((char *)edi - 1) == '{')
		    {
			edi++; // GCC extension: sizeof(void) = 1
			void *tmp = VLOAD(edi);
			if (tmp > edi)
			    {
				VSTORE(edi, tmp + ebx);
				fixup (tmp, ebx); // nested structure
			    }
		    }
		edi = edx + ILOAD(edx); // relative link
		if (edi == edx)
			break;
	    }
}

// Build a structure or sub-structure template:
static int structure (void **pedi)
{
	int ecx = 0;
	unsigned char type;
	void *edi = *pedi;

	while (1)
	    {
		signed char al = nxt ();
		void *tmp;
		void *ebx = edi + 4;
		ISTORE(edi, 0);	// zero terminating link

		if (al == '}')
		    {
			void *ptr;

			esi++; // bump past }
			equals ();
			nxt ();
			ptr = getvar (&type);
			if (ptr == NULL)
				error (10, NULL); // 'Bad DIM statement'
			if (type != STYPE)
				error (6, NULL); // 'Type mismatch'
			edi = VLOAD(ptr); // descriptor pointer
			if (((edi > (void *)esp) && ((edi - zero) < himem)) || (edi < (void *)2)) 
				error (10, NULL); // 'Bad DIM statement'
			*pedi = edi;
			return ecx + ILOAD(edi); // add structure size
		    }
		if (!range1 (al))
			error (16, NULL); // 'Syntax error'
		ebx = create ((unsigned char **)&ebx, &type);
		if (type == STYPE) // nested struct ?
		    {
			int eax;
			void *ebp;
			void *edi = ebx + 2 * sizeof(void *); // GCC extension: sizeof(void) = 1
			VSTORE(ebx, edi); // format pointer
			VSTORE(ebx + sizeof(void*), (void *)(size_t)ecx); // GCC extension
			ebp = edi;
			edi += 4;
			eax = structure (&edi);
			ISTORE(ebp, eax);
			ecx += eax;
			if (edi < ebx)
			    {
				VSTORE(ebx, edi); 
				edi = ebx + 2 * sizeof(void *); // GCC extension: sizeof(void) = 1
			    }
			ebx = edi;
		    }
		else
		    {
			ISTORE(ebx, ecx);	// store member offset
			ebx += 4;	// GCC extension: sizeof(void) = 1
			if (*esi == '(') // array member ?
			    {
				unsigned char dims = 0;
				int size = type & TMASK;
				void *desc = ebx + 1;
				do
				    {
					esi++;
					int n = expri () + 1;
					if (n < 1)
						error (10, NULL); // 'Bad DIM statement'
					ISTORE(desc, n); desc += 4;
					size *= n;
					dims++;
				    }
				while (*esi == ',');
				braket ();
				*(unsigned char*)ebx = dims;
				ebx = desc;
				ecx += size;
			    }
			else
				ecx += type & TMASK;
		    }
		tmp = ebx; ebx = edi; edi = tmp; // swap
		al = nxt ();
		esi++;
		if (al == '}')
			break;
		if (al != ',')
			error (16, NULL); // 'Syntax error'
		ISTORE(ebx, edi - ebx); // relative link (signed)
	    }
	*pedi = edi; // format pointer
	return ecx;
}

// Scan for DEF PROC and DEF FN
static void defscan (signed char *edx)
{
	void *ptr;
	unsigned char found;

	while (1)
	    {
		signed char *oldesi;
		edx = search (edx, TDEF);
		if (edx == NULL)
			return;
		oldesi = esi;
		esi = edx + 1; // Byte after DEF
		nxt ();
		if ((*esi == TPROC) || (*esi == TFN))
		    {
			ptr = getdef (&found);
			if (found == 0)
				ptr = putdef (ptr);
			VSTORE(ptr, esi);
		    }
		esi = oldesi;
		edx -= 3; // point to line-length byte
		edx += (int) *(unsigned char *)edx;
	    }
}

// User-defined PROC, ON PROC and FN:
void procfn (signed char flag)
{
	void *ptr;
	signed char *ebx;
	unsigned char found;
	signed char *oldesi;
	heapptr *edi;
	heapptr *resesp;

	esi--;		// Point to TFN or TPROC token
	oldesi = esi;
	ptr = getdef (&found);
	if (ptr == NULL)
		error (16, NULL); // 'Syntax error'
	if ((found == 0) || (ILOAD(ptr) == 0))
	    {
		if (libase)
			defscan (libase + (signed char *) zero);
		defscan (vpage + (signed char *) zero);
		esi = oldesi;
		ptr = getdef (&found);
		if ((found == 0) || (ILOAD(ptr) == 0))
			error (29, NULL); // 'No such FN/PROC'
	    }

	ebx = VLOAD(ptr);	// Get FN/PROC pointer

	esp -= STRIDE; // Reserve space
	resesp = esp;
	if (flag == TFN)
		*--esp = FNCHK;
	else
		*--esp = PROCHK;
	check ();

	if (nxt () == '(')
	    {
		int nret = 0;
		oldesi = esi;
		esi = ebx;
		if (nxt () != '(')
			error (16, NULL); // 'Syntax error'
		do
		    {
			esi++; // skip '(' or ','
			unsigned char type;
			if (nxt () == TRETURN)
			    {
				nret++;
				esi++;
				nxt ();
			    }
			ptr = getput (&type);
			savloc (ptr, type);
		    }
		while (nxt () == ',');
		braket ();
		edi = esp; // pointer to saved parameters

		check ();
		esp -= 2 * (STRIDE + 1) * nret; // make space for RETURN info
		*--esp = nret;
		*--esp = RETCHK;

		esi = ebx;
		ebx = argue (oldesi, esp + 2, 0) + 1; // Transfer arguments
		braket();

// If any of the dummy arguments are the same as passed-by-reference
// variables, then they must not be restored on exit (they would
// overwrite the wanted returned values), therefore search the saved
// values on the stack and if a match is found set the 'pointer' to
// zero.  On exit from the FN/PROC this will prevent the dummies
// being restored.  Here edi points to first saved dummy argument

		while ((nret != 0) && (*edi++ == LOCCHK))
		    {
			int i;
			heapptr *ebp = esp + 4 + STRIDE;
			unsigned char type = (unsigned char) *(int *)edi++;
			for (i = 0; i < nret; i++)
			    {
				if (*(void **)ebp == *(void **)edi)
				    {
					*(void **)edi = NULL;
					if (type == 136)
						allocs ((STR *)(edi+STRIDE), 0);
			  	    }
				ebp += 2 * (STRIDE + 1);
			    }
			edi += STRIDE;
			if (type == STYPE)
				edi += STRIDE * 2;
			else if (type < 128)
				edi += 3;
			else if (type == 136)
				edi += 2;
			else
				edi += ((*((int *)edi) + 3) >> 2) + 1;
		    }
	    }

	if (flag == TON)
		skip ();
	*(void **)resesp = esi;
	esi = ebx;
}

// Process end-of-line:
void newlin (void)
{
	unsigned char al = *esi;	// line length
	if (al == 0) return;
	esi += 3;
	curlin = esi - (signed char *) zero;
	if (tracen)
	    {
		unsigned short lino = SLOAD(esi - 2);
		if ((lino != 0) && (lino < tracen))
		    {
			outchr ('[');
			if (lino)
			    {
				sprintf (accs, "%d", lino);
				text (accs);
			    }
			outchr (']');
			outchr (' ');
		    }
	    }
}

static signed char al_token;
static void *tmpesi;
static bool bFlgChk;

static void xeq_COLON (void) //  ':':
    {
    bFlgChk = false;
    }

// static void xeq_TENDCASE (void)
static void xeq_TENDIF (void)
    {
    }

static void xeq_TSTOP (void)
    {
    unsigned short lino = setlin (esi, NULL);
    text ("\r\nSTOP");
    if (lino)
        {
        sprintf (accs, " at line %d", lino);
        text (accs);
        }
    crlf ();
    error (256, NULL);
    }

static void xeq_TEND (void)
    {
    osshut (0);
    error (256, NULL);
    }

static void xeq_NULL (void)
    {
    error (256, NULL);
    }

/************************************ WHEN *************************************/
/********************************** OTHERWISE **********************************/

// static void xeq_TOTHERWISE (void)
static void xeq_TWHEN (void)
    {
    error (44, NULL); // 'WHEN/OTHERWISE not first'
    }

/************************************ QUIT *************************************/

static void xeq_TQUIT (void)
    {
    int n = 0;
    if (!termq ())
        n = expri ();
    error (~n, NULL);
    }

/************************************* REM *************************************/

// static void xeq_TDEF (void)
// static void xeq_TDATA (void)
// static void xeq_TELSE (void)
static void xeq_TREM (void)
    {
    esi = (signed char*) memchr ((char *) esi, 0x0D, 255);
    }

/************************************ GOTO *************************************/

static void xeq_TGOTO (void)
    {
    int n = itemi ();
    if (!termq ())
        error (16, NULL); // 'Syntax error'
    esi = findl (n);
    if (esi == NULL)
        error (41, NULL); // 'No such line'
    newlin ();
    }

static void xeq_TLINO (void)
    {
    esi = tmpesi;
    xeq_TGOTO ();
    }

/************************************ GOSUB ************************************/

static void xeq_TGOSUB (void)
    {
    int n = itemi ();
    check ();
    esp -= STRIDE;
    *(void **)esp = esi;
    *--esp = GOSCHK;
    if (!termq ())
        error (16, NULL); // 'Syntax error'
    esi = findl (n);
    if (esi == NULL)
        error (41, NULL); // 'No such line'
    newlin ();
    }

/*********************************** RETURN ************************************/

static void xeq_TRETURN (void)
    {
    while (*(int *)esp != GOSCHK)
        {
        int ebx = *(int *)esp;
        if (ebx == LOCCHK)
            resloc ();
        else if (ebx == DIMCHK)
            undim ();
        else if (ebx == RETCHK)
            unret ();
        else if (ebx == FORCHK)
            esp += 9 + 2 * STRIDE;
        else if ((ebx == REPCHK) || (ebx == WHICHK))
            esp += 1 + STRIDE;
        else if (ebx == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }
        else if (ebx == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else if (ebx == CALCHK)
            {
            esp++;
            void *ebx = *(void **)esp;
            esp += STRIDE;
            VSTORE(ebx, NULL); // remove called module
#ifdef PICO
            libtop = ebx;
#endif
            }
        else
            error (38, NULL); // 'Not in a subroutine'
        }
    esp++;
    if (termq ())
        {
        esi = *(void **)esp;
        esp += STRIDE;
        return;
        }
    esp++;
        {
        int n = itemi ();
        esi = findl (n);
        if (esi == NULL)
            error (41, NULL); // 'No such line'
        esi += 3; // n.b. in case very first line!
        }
    }

/************************************ PROC *************************************/

static void xeq_TPROC (void)
    {
    if (*(esi-1) == ' ')
        error (30, NULL); // 'Bad call'
    procfn (TPROC);
    }

/************************************ LOCAL ************************************/

static void xeq_TLOCAL (void)
    {
    if (*esi == TERROR)
        esi++; // Ignore
    else if (*esi == TDATA)
        {
        esi++;
        *--esp = datptr;
        *--esp = LDCHK;
        }
    else
        {
        isloc ();
        while (1)
            {
            unsigned char type;
            void *ptr = getput (&type);
            savloc (ptr, type);
            if (type & (BIT4 | BIT6))
                VSTORE(ptr, (void *) 1); // Flag LOCAL array/struct
            else if (type == 128)
                *(char *)ptr = 0x0D; // CR-term string
            else if (type == 130)
                *(char *)ptr = 0; // NUL-term string
            else
                memset (ptr, 0, type & TMASK); // zero var/desc
            if (nxt () != ',')
                break;
            esi++;
            nxt ();
            }
        }
    }

/*********************************** PRIVATE ***********************************/

static void xeq_TPRIVATE (void)
    {
    unsigned char type;
    void *ptr;
    int count = 0;
    signed char *oldesi = esi;
    char *edi = accs;
    isloc ();

    *secret (edi, 4) = 0x0D;
    esi = (signed char *) accs;
    ptr = getput (&type);
    esi = oldesi;
    if (ILOAD(ptr))
        {
        skip ();
        return; // reentrant call, ignore
        }
    ISTORE(ptr, 1);
    *--esp = 0;
    *--esp = 0;
    *--esp = 0;
    esp -= STRIDE;
    *(void **)esp = ptr;
    *--esp = 4;
    *--esp = LOCCHK;

    while (1)
        {
        ptr = getput (&type);
        savloc (ptr, type);
        edi = secret (edi, type);
        count += 1;
        if (nxt () != ',')
            break;
        *edi++ = ',';
        esi++;
        nxt ();
        };
    *edi++ = 0x0D;

    if ((count << 4) > ((char *)esp - (char *)zero - pfree - STACK_NEEDED))
        error (0, NULL); // 'No room'
    esp -= count * 2 * (STRIDE + 1);
    *--esp = count;
    *--esp = RETCHK;
    esi = oldesi - 1;
    // transfer secret [accs] to formal [esi] 
    esi = argue ((signed char *)accs - 1, esp + 2, 1);
    }

/*********************************** ENDPROC ***********************************/

static void xeq_TENDPROC (void)
    {
    while (*(int *)esp != PROCHK)
        {
        int ebx = *(int *)esp;
        if (ebx == LOCCHK)
            resloc ();
        else if (ebx == DIMCHK)
            undim ();
        else if (ebx == RETCHK)
            unret ();
        else if (ebx == FORCHK)
            esp += 9 + 2 * STRIDE;
        else if ((ebx == REPCHK) || (ebx == WHICHK))
            esp += 1 + STRIDE;
        else if (ebx == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }
        else if (ebx == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else
            error (13, NULL); // 'Not in a procedure'
        }
    esp++;
    esi = *(void **)esp;
    esp += STRIDE;
    }

/***********************************  ENDFN  ***********************************/

static VAR xeq_ENDFN (void)    // '='
    {
    VAR v = expr ();
    if (v.s.t == -1)
        v.s.p = moves ((STR *)&v, 0) - (char *) zero;
    while (*(int *)esp != FNCHK)
        {
        int ebx = *(int *)esp;
        if (ebx == LOCCHK)
            resloc ();
        else if (ebx == DIMCHK)
            undim ();
        else if (ebx == RETCHK)
            unret ();
        else if (ebx == FORCHK)
            esp += 9 + 2 * STRIDE;
        else if ((ebx == REPCHK) || (ebx == WHICHK))
            esp += 1 + STRIDE;
        else if (ebx == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }
        else if (ebx == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else
            error (7, NULL); // 'Not in a function'
        }
    esp++;
    esi = *(void **)esp;
    esp += STRIDE;
    curlin = esi - (signed char *) zero;
    return v;
    }

/*********************************** RESTORE ***********************************/

static void xeq_TRESTOR (void)
    {
    if (*esi == TDATA)
        {
        esi++;
        if (*(int *)esp++ != LDCHK)
            error (54, "DATA not LOCAL");
        datptr = *esp++;
        return;
        }

    if (*esi == TERROR)
        {
        esi++;
        if (*(int *)esp++ != ONCHK)
            error (0, "ON ERROR not LOCAL");
        onersp = *(heapptr **)esp;
        esp += STRIDE;
        errtrp = *esp++;
        return;
        }

    if (*esi == TLOCAL)
        {
        esi++;
        while ((*(int *)esp != PROCHK) && (*(int *)esp != FNCHK))
            {
            int ebx = *(int *)esp;
            if (ebx == LOCCHK)
                resloc ();
            else if (ebx == DIMCHK)
                undim ();
            else if (ebx == RETCHK)
                unret ();
            else if (ebx == FORCHK)
                esp += 9 + 2 * STRIDE;
            else if ((ebx == REPCHK) || (ebx == WHICHK))
                esp += 1 + STRIDE;
            else if (ebx == ONCHK)
                {
                esp++;
                onersp = *(heapptr **)esp;
                esp += STRIDE;
                errtrp = *esp++;
                }
            else if (ebx == LDCHK)
                {
                esp++;
                datptr = *esp++;
                }
            else
                error (12, NULL); // 'Not in a FN or PROC'
            }
        return;
        }

    if (*esi == '+')
        {
        int n;
        signed char *edi;

        esi++;
        n = expri ();
        if (n <= 0)
            n = 1;
        edi = memchr (esi, 0x0D, 255) + 1;
        while (--n)
            edi += (int)*(unsigned char *)edi;
        datptr = search (edi, TDATA) - (signed char *) zero;
        }
    else if (!termq ())
        {
        int n = expri ();
        signed char *edi = findl (n);
        if (edi == NULL)
            error (41, NULL); // 'No such line'
        datptr = search (edi, TDATA) - (signed char *) zero;
        }
    else
        datptr = search (vpage + (signed char *) zero, TDATA) -
            (signed char *) zero;

    if ((int) datptr == 0)
        error (42, NULL); // 'Out of DATA'
    }

/*********************************** INSTALL ***********************************/

static void xeq_TINSTALL (void)
    {
    VAR v;
    void *chan;
    int size;
    signed char *edi = libase + (signed char *) zero;
    signed char *newtop;
    if (al_token == TINSTALL)
        {
        v = exprs ();
        fixs (v);
        }
    else
        v.s.l = (char *) memchr (accs, 0x0D, 255) - accs;

    if (edi == (signed char *) zero)
        {
        edi = himem + (signed char *) zero;
        oshwm (edi + 1, 0);
        libase = edi - (signed char *) zero;
        *edi = 0;
        }
    int ll;
    while ((ll = (int)*(unsigned char *)edi) != 0)
        {
        if ((al_token == TINSTALL) && (*(edi+3) == 0) &&
            (0 == memcmp (accs, edi+4, v.s.l+1)))
            return; // already installed
        edi += ll;
        }
    chan = osopen (0, accs);
    if (chan == 0)
        error (214, "File or path not found");
    size = getext (chan);
    osshut (chan);
    oshwm (edi + v.s.l + 5 + size, 0);
    ISTORE(edi, v.s.l + 5);
    memcpy (edi + 4, accs, v.s.l + 1);
    osload (accs, edi + v.s.l + 5, size);
    newtop = gettop (edi, NULL);
    if (newtop == NULL) 
        error (52, NULL); // 'Bad library'
#ifdef PICO
    libtop = newtop;
#endif
    if (al_token == TINSTALL)
        {
        defscan (libase + (signed char *) zero);
        defscan (vpage + (signed char *) zero);
        }
    else
        {
        ISTORE(newtop, 0xF8000005);
        *(newtop + 4) = 0x0D;
        check ();
        esp -= STRIDE;
        *(void **)esp = esi;
        *--esp = GOSCHK;
        esp -= STRIDE;
        *(void **)esp = edi;
        *--esp = CALCHK;
        esi = edi + v.s.l + 4;
        }
    }

/************************************ CALL *************************************/

static void xeq_TCALL (void)
    {
    VAR v = expr ();
    if (v.s.t != -1)
        {
        size_t n = v.i.n;
        void (*func) (int,int,int,int,int,int,char *);
        char *p = buff + 1;
        unsigned char count = 0;

        if (v.i.t != 0)
            n = v.f;

        while (nxt () == ',')
            {
            void *ptr;
            unsigned char type;

            esi++;
            nxt ();
            ptr = getvar (&type);
            if (ptr == NULL)
                error (16, NULL); // 'Syntax error'
            if (type == 0)
                error (26, NULL); // 'No such variable' 
            *p++ = type;
            VSTORE(p, ptr);
            p += sizeof(void *); // GCC extension 
            count += 1;
            };
        *buff = count;

        if ((n >= 0x8000) && (n <= 0xFFFF))
            oscall (n);
        else
            {
            func = (void *) n;
            func (stavar[1], stavar[2], stavar[3],
                stavar[4], stavar[5], stavar[6], buff);
            }
        return;
        }
    else
        fixs (v);
    xeq_TINSTALL ();
    }

/************************************ TRACE ************************************/

static void xeq_TTRACE (void)
    {
    switch (*esi)
        {
        case TON:
            esi++ ;
            tracen = 0xFFFF ;
            break ;
        case TOFF:
            esi++ ;
            tracen = 0 ;
            break ;
        case TSTEP:
            esi++ ;
            flags |= PAUSE ;
            switch (nxt ())
                {
                case TOFF:
                    flags &= ~PAUSE ;
                case TON:
                    esi++ ;
                }
            break ;
        default:
            tracen = expri () ;
        }
    }

/************************************ BPUT *************************************/

static void xeq_TBPUT (void)
    {
    VAR v;
    void *chan = channel ();
    comma ();
    v = expr ();
    if (v.s.t != -1)
        {
        if (v.i.t != 0)
            v.i.n = v.f;
        osbput (chan, v.i.n);
        }
    else
        {
        int i;
        char *p = v.s.p + (char *) zero;
        for (i = 0; i < v.s.l; i++)
            osbput (chan, *p++);
        if (nxt () == ';')
            esi++;
        else
            osbput (chan, 0x0A);
        }
    }

/************************************  PTR  ************************************/

static void xeq_TPTRL (void)
    {
    if (*esi == '(')
        {
        void *ptr;
        unsigned char type;
        esi++;
        nxt ();
        ptr = getvar (&type);
        if (ptr == NULL)
            error (16, NULL); // 'Syntax error'
        if (type == 0)
            error (26, NULL); // 'No such variable'
        braket ();
        equals ();
        if (type == 136)
            USTORE(ptr, expri () - (size_t) zero);
        else if ((type == 36) || (type & 0x40))
            TSTORE(ptr, expri ());
        else if (type == STYPE)
            TSTORE(ptr + sizeof(void*), expri ());
        else
            error (6, NULL); // 'Type mismatch'
        }
    else
        {
        long long n;
        void *chan = channel ();
        equals ();
        n = expri ();
        setptr (chan, n);
        }
    }

/************************************  EXT  ************************************/

static void xeq_TEXTR (void)
    {
    error (255, "Sorry, not implemented");
    }

/************************************ PAGE *************************************/

static void xeq_TPAGEL (void)
    {
    equals ();
    void *n = (void *) (size_t) expri ();
    if ((n + STACK_NEEDED) > (void *) esp)
        error (8, NULL); // 'Address out of range'
    vpage = n - zero;
    }

/************************************ LOMEM ************************************/

static void xeq_TLOMEML (void)
    {
    equals ();
    void *n = (void *) (size_t) expri ();
    if ((n + STACK_NEEDED) > (void *) esp)
        error (8, NULL); // 'Address out of range'
    clear ();
    lomem = n - zero;
    pfree = n - zero;
    }

/************************************ HIMEM ************************************/

static void xeq_THIMEML (void)
    {
    equals ();
    void *n = (void *) (size_t) (expri () & -4); // align
    if ((n < (pfree + zero + STACK_NEEDED)) ||
        (oshwm (n, 1) == 0))
        error (0, NULL); // 'No room'
    if ((void *) esp == himem + zero)
        esp = n;
    himem = n - zero;
    if ((libase != 0) && (himem > libase))
        {
        libase = 0;
        proptr[0] = 0;
        fnptr[0] = 0;
#ifdef PICO
        libtop = n;
#endif
        }
    }

/************************************  RUN  ************************************/

static void xeq_TRUN (void)
    {
    if (!termq ())
        {
        VAR v = exprs ();
        fixs (v);
        osload (accs, vpage + zero, 
            (signed char *)esp - (signed char *)zero - vpage - STACK_NEEDED);
        }
    clrtrp ();
    clear ();
    datptr = search (vpage + (signed char *) zero, TDATA) -	
        (signed char *) zero;
    esi = vpage + (signed char *) zero;
    esp = (heapptr *)((himem & -4) + zero); // align
    newlin ();
    }

/************************************ CHAIN ************************************/

static void xeq_TCHAIN (void)
    {
    VAR v = exprs ();
    fixs (v);
    osload (accs, vpage + zero, 
        (signed char *)esp - (signed char *)zero - vpage - STACK_NEEDED);
    clrtrp ();
    clear ();
    datptr = search (vpage + (signed char *) zero, TDATA) -	
        (signed char *) zero;
    esi = vpage + (signed char *) zero;
    esp = (heapptr *)((himem & -4) + zero); // align
    newlin ();
    }

/************************************ INPUT ************************************/

static void xeq_TINPUT (void)
    {
    if (*esi == '#')
        {
        void *chan = channel ();
        while (nxt () == ',')
            {
            void *ptr;
            unsigned char type;
            esi++;
            nxt ();
            ptr = getput (&type);
            if (type & BIT6)
                error (14, NULL); // 'Bad use of array'
            if (type & BIT4)
                error (56, NULL); // 'Bad use of structure'
            if (type < 128)
                {
                VAR v;
                char *p = pfree + (char *) zero;
                int i, size = 5;
                if (liston & BIT0) size = 8;
                if (liston & BIT1) size = 10;
                for (i = 0; i < size; i++)
                    *p++ = osbget (chan, NULL);
                v = loadn (pfree + zero, size);
                storen (v, ptr, type);
                }
            else
                {
                VAR v;
                char al, *p;
                int eof;
                v.s.t = -1;
                v.s.l = 0;
                v.s.p = 0;
                stores (v, ptr, type); // May affect pfree
                p = pfree + 3 + (char *) zero;
                v.s.p = p - (char *) zero;
                while (((al = osbget (chan, &eof)) != 0x0D) &&
                    (eof == 0))
                    {
                    *p++ = al;
                    if ((p + STACK_NEEDED) > (char *)esp)
                        error (0, NULL); // 'No room'
                    }
                v.s.l = p - (char *) zero - v.s.p;
                stores (v, ptr, type);
                }
            }
        return;
        }
        {
        unsigned char flag = 0;
        char *bufptr = buff;
        *bufptr = 0x0D;

        if (*esi == TLINE)
            {
            esi++;
            flag = BIT7;
            nxt ();
            }

        while (1)
            {
            VAR v;
            signed char al;

            if (termq ())
                return;

            al = *esi++;
            if ((al == ',') || (al == ';'))
                flag ^= BIT0;
            else if (al == '"')
                {
                v = cons ();
                ptext (v.s.p + (char *) zero, v.s.l);
                flag |= BIT0;
                }
            else if (format (al))
                flag |= BIT0;
            else
                {
                void *ptr;
                unsigned char type;
                esi--;
                ptr = getput (&type);
                if (type & BIT6)
                    error (14, NULL); // 'Bad use of array'
                if (type & BIT4)
                    error (56, NULL); // 'Bad use of structure'
                if (*bufptr == 0x0D)
                    {
                    if (!(flag & BIT0))
                        {
                        outchr ('?');
                        outchr (' ');
                        }
                    osline (buff);
                    crlf ();
                    bufptr = buff;
                    }
                if (flag & BIT7)
                    {
                    v.s.l = (char *) memchr (bufptr, 0x0D, 256) - bufptr;
                    memcpy (accs, bufptr, v.s.l + 1);
                    bufptr += v.s.l;
                    }
                else
                    {
                    v.s.l = fetchs (&bufptr);
                    if ((*bufptr == ',') || (*bufptr == ';'))
                        bufptr++;
                    }
                v.s.p = accs - (char *) zero;
                if (type >= 128)
                    stores (v, ptr, type);
                else
                    storen (val (), ptr, type);
                flag ^= BIT0;
                }
            }
        }
    }

/************************************ READ *************************************/

static void xeq_TREAD (void)
    {
    if (*esi != '#')
        {
        unsigned char type;
        void *ptr;
        signed char *edx = datptr + (signed char *) zero;
        if (edx == NULL)
            error (42, NULL); // 'Out of DATA'
        while (1)
            {
            signed char al = *edx++;
            if ((al != TDATA) && (al != ','))
                {
                if (al != 0x0D)
                    {
                    if (al != ':')
                        {
                        curlin = edx - 1 - (signed char *) zero;
                        error (16, NULL);
                        }
                    edx = memchr (edx, 0x0D, 255) + 1;
                    }
                edx = search (edx, TDATA);
                if (edx == NULL)
                    error (42, NULL); // 'Out of DATA'
                edx++;
                }
            ptr = getput (&type);
            if (type & BIT6)
                error (14, NULL); // 'Bad use of array'
            if (type & BIT4)
                error (56, NULL); // 'Bad use of structure'
            if (type < 128)
                {
                VAR v;
                signed char *oldesi = esi;
                esi = edx;
                curlin = esi - (signed char *) zero;
                v = exprn ();
                edx = esi;
                esi = oldesi;
                storen (v, ptr, type);
                }
            else
                {
                VAR v;
                v.s.l = fetchs ((char **) &edx);
                v.s.p = accs - (char *) zero;
                stores (v, ptr, type);
                }
            datptr = edx - (signed char *) zero;
            if (nxt () != ',')
                return;
            esi++;
            nxt ();
            curlin = esi - (signed char *) zero;
            }
        return;
        }
    xeq_TINPUT ();
    }

/************************************ PRINT ************************************/

static void xeq_TPRINT (void)
    {
    if (*esi == '#')
        {
        void *chan = channel ();
        while (nxt () == ',')
            {
            VAR v; 
            esi++;
            v = expr ();
            if (v.s.t != -1)
                {
                char *p = pfree + (char *) zero;
                int i, size = 5;
                if (liston & BIT0) size = 8;
                if (liston & BIT1) size = 10;
                storen (v, p, size);
                for (i = 0; i < size; i++)
                    osbput (chan, *p++);
                }
            else
                {
                int i;
                char *p = v.s.p + (char *) zero;
                for (i = 0; i < v.s.l; i++)
                    osbput (chan, *p++);
                osbput (chan, 0x0D);
                }
            }
        return;
        }
        {
        signed char al;
        VAR v;
        unsigned char mode = 0;
        int field = stavar[0] & 0xFF;

        while (1)
            {
            al = nxt ();
            if ((al == ':') || (al == 0x0D) || (al == TELSE))
                {
                if ((mode & BIT0) == 0) crlf ();
                break;
                }
            mode &= ~BIT0;
            esi++;
            if (al == '~')
                mode = 2;
            else if (al == ';')
                {
                mode = 1;
                field = 0;
                }
            else if (al == ',')
                {
                mode &= ~2;
                field = stavar[0] & 0xFF;
                if (field)
                    while (vcount % field)
                        outchr (' ');
                }
            else if (!format(al))
                {
                esi--;
                v = expr ();
                if (v.s.t == -1)
                    {
                    ptext (v.s.p + (char *) zero, v.s.l);
                    }
                else
                    {
                    if (mode & BIT1)
                        strhex (v, accs, field);
                    else
                        str (v, accs, (stavar[0] & 0xFFFF00) + field);
                    text (accs);
                    }
                }
            }
        }
    }

/*********************************** WIDTH *************************************/

static void xeq_TWIDTH (void)
    {
    vwidth = expri ();
    }

/*********************************** CLOSE *************************************/

static void xeq_TCLOSE (void)
    {
    osshut (channel ());
    }

/***********************************  OFF  *************************************/

static void xeq_TOFF (void)
    {
	signed char al;
    oswrch (23);
    oswrch (1);
    for (al = 0; al < 8; al++)
        oswrch(0);
    }

/*********************************** ERROR *************************************/

static void xeq_TERROR (void)
    {
    VAR v;
    int n = expri ();
    comma ();
    v = exprs ();
    memcpy (buff, v.s.p + zero, v.s.l);
    *(buff + v.s.l) = 0;
    error (n, buff);
    }

/*********************************** CLEAR *************************************/

static void xeq_TCLEAR (void)
    {
    clear ();
    datptr = search (vpage + (signed char *) zero, TDATA) -
        (signed char *) zero;
    }

/***********************************  VDU  *************************************/

static void xeq_TVDU (void)
    {
    do
        {
        int n = expri ();
        oswrch (n);
        if (*esi == ',')
            esi++;
        else if (*esi == ';')
            {
            esi++;
            oswrch (n >> 8);
            }
        else if (*esi == '|')
            {
            int i = 9;
            esi++;
            while (i--) oswrch (0);
            }
        }
    while (!termq ());
    }

/***********************************  CLS  *************************************/

static void xeq_TCLS (void)
    {
    oswrch (12);
    vcount = 0;
    }

/***********************************  CLG  *************************************/

static void xeq_TCLG (void)
    {
    oswrch (16);
    }

/***********************************  MODE  ************************************/

static void xeq_TMODE (void)
    {
    int n = expri ();
    oswrch (22);
    oswrch (n);
    vcount = 0;
    }

/*********************************** COLOUR ************************************/

static void xeq_TCOLOUR (void)
    {
    int r, g, b, n = expri ();
    if (*esi != ',')
        {
        oswrch (17);
        oswrch (n);
        return;
        }
    esi ++;
    r = expri ();
    comma ();
    g = expri ();
    comma ();
    b = expri ();
    oswrch (19);
    oswrch (n);
    oswrch (16);
    oswrch (r);
    oswrch (g);
    oswrch (b);
    }

/***********************************  GCOL  ************************************/

static void xeq_TGCOL (void)
    {
    int m = 0, n = expri ();
    if (*esi == ',')
        {
        esi++;
        m = n;
        n = expri ();
        }
    oswrch (18);
    oswrch (m);
    oswrch (n);
    }

/**************************  TINT (not implemented)  ***************************/

static void xeq_TTINT (void)
    {
    expri ();
    }

/***********************************  MOVE  ************************************/

static void xeq_TMOVE (void)
    {
    int c = 4, x, y;
    if (*esi == TBY)
        {
        esi++;
        c = 0;
        }
    x = expri ();
    comma ();
    y = expri ();
    plot (c, x, y);
    }

/***********************************  DRAW  ************************************/

static void xeq_TDRAW (void)
    {
    int c = 5, x, y;
    if (*esi == TBY)
        {
        esi++;
        c = 1;
        }
    x = expri ();
    comma ();
    y = expri ();
    plot (c, x, y);
    }

/***********************************  FILL  ************************************/

static void xeq_TFILL (void)
    {
    int c = 133, x, y;
    if (*esi == TBY)
        {
        esi++;
        c = 129;
        }
    x = expri ();
    comma ();
    y = expri ();
    plot (c, x, y);
    }

/***********************************  LINE  ************************************/

static void xeq_TLINE (void)
    {
    int x1, y1, x2, y2;
    x1 = expri ();
    comma ();
    y1 = expri ();
    comma ();
    x2 = expri ();
    comma ();
    y2 = expri ();
    plot (4, x1, y1);
    plot (5, x2, y2);
    }

/*********************************** ORIGIN ************************************/

static void xeq_TORIGIN (void)
    {
    int x, y;
    x = expri ();
    comma ();
    y = expri ();
    oswrch (29);
    oswrch (x);
    oswrch (x >> 8);
    oswrch (y);
    oswrch (y >> 8);
    }

/***********************************  PLOT  ************************************/

static void xeq_TPLOT (void)
    {
    int n, x, y;
    if (*esi == TBY)
        {
        esi++;
        n = 65;
        }
    else
        {
        n = expri ();
        comma ();
        }
    x = expri ();
    if (*esi == ',')
        {
        esi++;
        y = expri ();
        }
    else
        {
        y = x;
        x = n;
        n = 69;
        }
    plot (n, x, y);
    }

/************************************ CIRCLE ***********************************/

static void xeq_TCIRCLE (void)
    {
    int n = 145, x, y, r;
    if (*esi == TFILL)
        {
        esi++;
        n = 153;
        }
    x = expri ();
    comma ();
    y = expri ();
    comma ();
    r = expri ();
    plot (4, x, y);
    plot (n, r, 0);
    }

/*********************************** ELLIPSE ***********************************/

static void xeq_TELLIPSE (void)
    {
    int n = 193, x, y, a, b;
    if (*esi == TFILL)
        {
        esi++;
        n = 201;
        }
    x = expri ();
    comma ();
    y = expri ();
    comma ();
    a = expri ();
    comma ();
    b = expri ();
    plot (4, x, y);
    plot (0, a, 0);
    plot (n, 0, b);
    }

/********************************** RECTANGLE **********************************/

static void xeq_TRECT (void)
    {
    int n = 191, x, y, w, h;
    if (*esi == TFILL)
        {
        esi++;
        n = 189;
        }
    else if (*esi == TSWAP)
        {
        esi++;
        n = 253;
        }
    x = expri ();
    comma ();
    y = expri ();
    comma ();
    w = expri ();
    if (*esi == ',')
        {
        esi++;
        h = expri ();
        }
    else
        h = w;

    if (*esi == TTO)
        {
        esi++;
        plot (4, x, y);
        plot (0, w, h);
        x = expri ();
        comma ();
        y = expri ();
        plot (n, x, y);
        }
    else if (n == 191) // RECTANGLE
        {
        plot (4, x, y);
        plot (9, w, 0);
        plot (9, 0, h);
        plot (9, -w, 0);
        plot (9, 0, -h);
        }
    else if (n == 189) // RECTANGLE FILL
        {
        plot (4, x, y);
        plot (97, w, h);
        }
    else
        error (16, NULL); // 'Syntax error'
    }

/*************************************  IF  ************************************/

static void xeq_TIF (void)
    {
    long long n = expri ();
    if (n)
        {
        if (*esi == TTHEN)
            while (*++esi == ';');
        bFlgChk = false;
        return;
        }

    while (1)
        {
        signed char al = *esi;
        if ((al == TREM) || (al == 0x0D))
            break;
        esi++;

        if (al == '"')
            quote ();

        if (al == TELSE)
            {
            bFlgChk = false; // To prevent event interrupt
            return;
            }

        if (al == TTHEN)
            {
            while (*esi == ';') esi++;
            if (*esi == 0x0D)
                {
                esi++;
                esi = nsurch (TELSE, 0, TTHEN, TENDIF, 0);
                if (esi == NULL)
                    error (49, NULL); // 'Missing ENDIF'
                esi += 4;
                break;
                }
            }
        }
    }


/*************************************  ON  ************************************/

static void xeq_TON (void)
    {
	signed char al;
    int n, nest ;
    void *edx ;
    heapptr *ebx ;

    al = *esi++ ;
    switch (al)
        {
        case TERROR:
            if (nxt () == TLOCAL)
                {
                esi++ ;
                *--esp = errtrp ;
                esp -= STRIDE ;
                *(heapptr **)esp = onersp ;
                *--esp = ONCHK ;
                edx = esp ;
                }
            else
                edx = NULL ;

            if (nxt () == TOFF)
                {
                esi++ ;
                errtrp = 0 ;
                onersp = NULL ;
                }
            else
                {
                errtrp = esi - (signed char *) zero ;
                onersp = edx ;
                esi = memchr (esi, 0x0D, 255) ;
                }
            break ;

        case TTIMER:
            ebx = &timtrp ;
            goto onevt ;
        case TCLOSE:
            ebx = &clotrp ;
            goto onevt ;
        case TMOVE:
            ebx = &siztrp ;
            goto onevt ;
        case TSYS:
            ebx = &systrp ;
            goto onevt ;
        case TMOUSE:
            ebx = &moutrp ;
        onevt:
            if (nxt () == TLOCAL)
                {
                isloc () ;
                esi++ ;
                savloc (ebx, 4) ;
                }
            if (nxt () == TOFF)
                {
                esi++ ;
                *ebx = 0 ;
                }
            else
                {
                *ebx = esi - (signed char *) zero ;
                esi = memchr (esi, 0x0D, 255) ;
                }
            break ;

        default:
            esi-- ;
            if (termq ())
                {
                oswrch (23) ;
                oswrch (1) ;
                oswrch (1) ;
                for (al = 0; al < 7; al++)
                    oswrch(0) ;
                break ;
                }
            n = expri () - 1 ;
            nest = 0 ;
            al = *esi++ ;
            if ((al != TGOTO) && (al != TGOSUB) && (al != TPROC))
                error (39, NULL) ; // 'ON syntax'
            while (n)
                {
                switch (*esi++)
                    {
                    case '(':
                        nest++ ;
                        break ;
                    case ')':
                        nest-- ;
                        break ;
                    case '"':
                        quote () ;
                        break ;
                    case TLINO:
                        esi += 3 ;
                        break ;
                    case ',':
                        if (nest == 0)
                            n-- ;
                    }
                if (termq ())
                    {
                    if (nxt () == TELSE)
                        {
                        esi++ ;
                        bFlgChk = false;
                        return;
                        }
                    error (40, NULL) ; // 'ON range'
                    }
                }
            if (al == TPROC)
                {
                if (nxt () == TPROC)
                    esi++ ;
                procfn (TON) ;
                break ;
                }
            n = itemi () ;
            skip () ;
            if (al == TGOSUB)
                {
                check () ;
                esp -= STRIDE ;
                *(void **)esp = esi ;
                *--esp = GOSCHK ;
                }
            esi = findl (n) ;
            if (esi == NULL)
                error (41, NULL) ; // 'No such line'
            newlin () ;
        }
    }

/************************************  FOR  ************************************/

static void xeq_TFOR (void)
    {
    void *ptr;
    unsigned char type;
    VAR v;

    ptr = getvar (&type);
    if (ptr == NULL)
        error (34, NULL); // 'Bad FOR variable'
    if (type == 0)
        ptr = putvar (ptr, &type);
    if (type >= 128)
        error (34, NULL); // 'Bad FOR variable'
    assign (ptr, type);
    if (*esi++ != TTO)
        error (36, NULL); // 'Missing TO'
    esp -= STRIDE;
    *(void **)esp = ptr;
    *--esp = (int) type;

    v = exprn (); // limit
    *--esp = (int)v.i.t;
    esp -= 2;
    *(long long *) esp = v.i.n;

    if (*esi == TSTEP)
        {
        esi++;
        v = exprn (); // step
        if (v.i.n == 0)
            error (35, NULL); // 'STEP cannot be zero'
        }
    else
        {
        v.i.t = 0;
        v.i.n = 1;
        }
    *--esp = (int)v.i.t;
    esp -= 2;
    *(long long *) esp = v.i.n;

    if (v.i.t == 0)
        *--esp = (int) (v.i.n >= 0);
    else
        *--esp = (int) (v.f >= 0.0);
    esp -= STRIDE;
    *(void **)esp = esi;
    *--esp = FORCHK;
    check ();
    }

/************************************* NEXT *************************************/

static void xeq_TNEXT (void)
    {
    while (1)
        {
        void *ptr = NULL;
        unsigned char type;
        VAR v, L, s;
        long long tmpll;
        signed char al = *esi;
        int b;

        if ((al != ':') && (al != 0x0D) &&
            (al != TELSE) && (al != ','))
            ptr = getvar (&type);

        while (*(int *)esp != FORCHK)
            {
            if (*(int *)esp == ONCHK)
                {
                esp++;
                onersp = *(heapptr **)esp;
                esp += STRIDE;
                errtrp = *esp++;
                }
            else if (*(int *)esp == LDCHK)
                {
                esp++;
                datptr = *esp++;
                }
            else
                error (32, NULL); // 'Not in a FOR loop'
            }

        if (ptr == NULL)
            ptr = *(void **)(esp + 9 + STRIDE);

        while (ptr != *(void **)(esp + 9 + STRIDE))
            {
            esp += 9 + 2 * STRIDE;
            while (*(int *)esp != FORCHK)
                {
                if (*(int *)esp == ONCHK)
                    {
                    esp++;
                    onersp = *(heapptr **)esp;
                    esp += STRIDE;
                    errtrp = *esp++;
                    }
                else if (*(int *)esp == LDCHK)
                    {
                    esp++;
                    datptr = *esp++;
                    }
                else
                    error (33, NULL); // 'Can't match FOR'
                }
            }

        type = (char) (int) *(esp + 8 + STRIDE);
        v = loadn (ptr, type);
        s.i.t = *(short *)(esp + 4 + STRIDE);
        s.i.n = *(long long *)(esp + 2 + STRIDE);
#if defined __GNUC__ && __GNUC__ < 5
        if ((v.i.t == 0) && (s.i.t == 0) && ((((tmpll = v.i.n + s.i.n) ^
                        v.i.n) >= 0) || ((int)(v.s.l ^ s.s.l) < 0)))
#else
            if ((v.i.t == 0) && (s.i.t == 0) &&
                (! __builtin_saddll_overflow (v.i.n, s.i.n, &tmpll)))
#endif
                v.i.n = tmpll;
            else
                {
                if (v.i.t == 0)
                    {
                    v.i.t = 1; // ARM
                    v.f = v.i.n;
                    }
                if (s.i.t == 0)
                    {
                    s.i.t = 1; // ARM
                    s.f = s.i.n;
                    }
                v.f += s.f;
                }
        storen (v, ptr, type);

        al = *(signed char *)(esp + 1 + STRIDE);
        L.i.t = *(short *)(esp + 7 + STRIDE);
        L.i.n = *(long long*)(esp + 5 + STRIDE);
        if ((v.i.t == 0) && (L.i.t == 0))
            {
            if (al)
                b = v.i.n > L.i.n;
            else
                b = v.i.n < L.i.n;
            }
        else
            {
            if (v.i.t == 0)
                {
                v.i.t = 1; // ARM
                v.f = v.i.n;
                }
            if (L.i.t == 0)
                {
                L.i.t = 1; // ARM
                L.f = L.i.n;
                }
            if (al)
                b = v.f > L.f;
            else
                b = v.f < L.f;
            }

        if (b)
            {
            esp += 9 + 2 * STRIDE;
            if (nxt () != ',')
                break;
            esi++;
            nxt ();
            }
        else
            {
            esi = *(void **)(esp + 1);
            break;
            }
        }
    }

/*********************************** REPEAT ************************************/

static void xeq_TREPEAT (void)
    {
    check ();
    esp -= STRIDE;
    *(void **)esp = esi;
    *--esp = REPCHK;
    }

/*********************************** UNTIL *************************************/

static void xeq_TUNTIL (void)
    {
    while (*(int *)esp != REPCHK)
        {
        if (*(int *)esp == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }
        else if (*(int *)esp == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else
            error (43, NULL); // 'Not in a REPEAT loop'
        }
        {
        long long n = expri ();
        if (n)
            esp +=  1 + STRIDE;
        else
            esi = *(void **)(esp+1);
        }
    }

/*********************************** WHILE *************************************/

static void xeq_TWHILE (void)
    {
    check ();
    esp -= STRIDE;
    *(void **)esp = esi;
    *--esp = WHICHK;

        {
        long long n = expri ();
        if (n) 
            return;
        esp += 1 + STRIDE;
        wsurch (TENDWHILE, TWHILE, 1);
        }
    }

/********************************* ENDWHILE ************************************/

static void xeq_TENDWHILE (void)
    {
    while (*(int *)esp != WHICHK)
        {
        if (*(int *)esp == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }
        else if (*(int *)esp == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else
            error (46, NULL); // 'Not in a WHILE loop'
        }
    signed char *esiold = esi;
    esi = *(void **)(esp+1);
        {
        long long n = expri ();
        if (n)
            return;
        esp += 1 + STRIDE;
        esi = esiold;
        }
    }

/************************************ EXIT *************************************/

static void xeq_TEXIT (void)
    {
    int level = 1;
    unsigned char type;
    char *ptr = NULL;
    signed char al = *esi++;

    if ((al == TFOR) && !termq ())
        ptr = getvar (&type);

    while (1)
        {
        int ebx = *esp;
        if (ebx == FORCHK)
            {
            void *forptr = *(void **)(esp + 9 + STRIDE);
            esp += 9 + 2 * STRIDE;
            if (al == TFOR)
                {
                if ((ptr == NULL) || (ptr == forptr))
                    {
                    wsurch (TNEXT, TFOR, level);
                    break;
                    }
                else
                    level++;
                }
            }

        else if (ebx == REPCHK)
            {
            esp += 1 + STRIDE;
            if (al == TREPEAT)
                {
                wsurch (TUNTIL, TREPEAT, level);
                break;
                }
            }

        else if (ebx == WHICHK)
            {
            esp += 1 + STRIDE;
            if (al == TWHILE)
                {
                wsurch (TENDWHILE, TWHILE, level);
                break;
                }
            }

        else if (ebx == ONCHK)
            {
            esp++;
            onersp = *(heapptr **)esp;
            esp += STRIDE;
            errtrp = *esp++;
            }

        else if (ebx == LDCHK)
            {
            esp++;
            datptr = *esp++;
            }
        else
            {
            if ((al == TFOR) && (level > 1))
                error (33, NULL); // 'Can't match FOR'
            if (al == TFOR)
                error (32, NULL); // 'Not in a FOR loop'
            if (al == TREPEAT)
                error (43, NULL); // 'Not in a REPEAT loop'
            if (al == TWHILE)
                error (46, NULL); // 'Not in a WHILE loop'
            else
                error (16, NULL); // 'Syntax error'
            }
        }
    skip ();
    }

/************************************ SWAP *************************************/

static void xeq_TSWAP (void)
    {
    void *ptr1, *ptr2;
    unsigned char type1, type2;
    unsigned int n;
    ptr1 = getvar (&type1);
    comma ();
    nxt ();
    ptr2 = getvar (&type2);
    if ((ptr1 == NULL) || (ptr2 == NULL))
        error (16, NULL); // 'Syntax error'
    if ((type1 == 0) || (type2 == 0))
        error (26, NULL); // 'No such variable'
    if (type1 != type2)
        error (6, NULL); // 'Type mismatch'
    if ((type1 == 36) || (type1 & BIT6))
        n = sizeof(size_t); // whole array
    else if (type1 == 128)
        {
        unsigned int t;
        n = memchr (ptr1, 0x0D, 0x10000) - ptr1 + 1;
        t = memchr (ptr2, 0x0D, 0x10000) - ptr2 + 1;
        if ((n > 0x10000) || (t > 0x10000))
            error (19, NULL); // 'String too long'
        if (n < t) n = t;
        }
    else if (type1 == 130)
        {
        unsigned int t;
        n = strlen (ptr1) + 1;
        t = strlen (ptr2) + 1;
        if (n < t) n = t;
        }
    else
        n = type1 & TMASK;

    if (n > ((char *)esp - (char *)zero - pfree - STACK_NEEDED))
        error (0, NULL); // No room
    memcpy (pfree + zero, ptr1, n);
    memcpy (ptr1, ptr2, n);
    memcpy (ptr2, pfree + zero, n);
    }

/************************************ CASE *************************************/

static void xeq_TCASE (void)
    {
    int level = 0;
    signed char *oldesi;
    heapptr *oldesp = NULL;
    VAR w;
    VAR v = expr ();

    if (*esi++ != TOF)
        error (37, NULL); // 'Missing OF'
    if (*esi++ != 0x0D)
        error (48, NULL); // 'OF not last'
    if (v.s.t == -1)
        oldesp = pushs (v);
    while (1)
        {
        esi = nsurch (TWHEN, TOTHERWISE, TOF, TENDCASE, level);
        if (esi == NULL)
            error (47, NULL); // 'Missing ENDCASE'
        oldesi = esi;
        esi += 3;
        if (*esi++ != TWHEN)
            {
            if (oldesp) esp = oldesp;
            break; // Not found
            }
        do
            {
            if (oldesp)
                {
                w = exprs ();
                if ((v.s.l == w.s.l) &&
                    (0 == memcmp (esp, w.s.p + zero, w.s.l)))
                    {
                    esp = oldesp;
                    while (*esi == ',')
                        {
                        esi++;
                        expr ();
                        }
                    bFlgChk = false;
                    return; // Found
                    }
                }
            else
                {
                w = exprn ();
                w = math (v, '=', w);
                if (w.i.n)
                    {
                    while (*esi == ',')
                        {
                        esi++;
                        expr ();
                        }
                    bFlgChk = false;
                    return; // Found
                    }
                }
            }
        while (*esi++ == ',');
        esi = oldesi;
        esi += (int)*(unsigned char *)esi;
        if (*(esi-2) == TOF)
            level = 1;
        else
            level = 0;
        }
    }

/******************************* TIME and TIME$ ********************************/

static void xeq_TTIMEL (void)
    {
    long long n;
    equals ();
    n = expri ();
    putime (n);
    }

/************************************ WAIT *************************************/

static void xeq_TWAIT (void)
    {
    int n;
    if (termq ())
        n = -1;
    else
        n = expri ();
    oswait (n);
    }

/*********************************** REPORT ************************************/

static void xeq_TREPORT (void)
    {
    report ();
    text (accs);
    }

/*********************************** MOUSE *************************************/

static void xeq_TMOUSE (void)
    {
    int b, x, y ;
    VAR v ;
    void *ptr ;
    unsigned char type ;

    switch (*esi++)
        {
        case TON:
            if (termq ())
                b = 0 ;
            else
                b = expri () ;
            mouseon (b) ;
            break ;

        case TOFF:
            mouseoff () ;
            break ;

        case TTO:
            x = expri () ;
            comma () ;
            y = expri () ;
            mouseto (x, y) ;
            break ;

        default:
            esi-- ;
            v.i.t = 0 ;
            mouse (&x, &y, &b) ;
            ptr = getput (&type) ;
            if (type >= 64)
                error (50, NULL) ; // 'Bad MOUSE variable'
            v.i.n = x ;
            storen (v, ptr, type) ;

            comma () ;
            nxt () ;
            ptr = getput (&type) ;
            if (type >= 64)
                error (50, NULL) ; // 'Bad MOUSE variable'
            v.i.n = y ;
            storen (v, ptr, type) ;

            comma () ;
            nxt () ;
            ptr = getput (&type) ;
            if (type >= 64)
                error (50, NULL) ; // 'Bad MOUSE variable'
            v.i.n = b ;
            storen (v, ptr, type) ;
        }
    }

/***********************************  SYS  *************************************/

static void xeq_TSYS (void)
    {
    int ni = 0, nf = 0;
    heapptr *oldesp = esp;
    VAR v = expr ();
    long long (*func) (size_t, size_t, size_t, size_t, size_t, size_t, 
        size_t, size_t, size_t, size_t, size_t, size_t);
    PARM parm;
    void *ptr = NULL;
    unsigned char type = 0;
    parm.f[0] = 0.0;
    parm.i[0] = 0;

    if (v.s.t == -1)
        {
        if (v.s.l > 255)
            error (19, NULL); // 'String too long'
        memcpy (accs, v.s.p + zero, v.s.l);
        *(accs + v.s.l) = 0;
        func = sysadr (accs);
        if (func == NULL)
            error (51, NULL); // 'No such system call'
        }
    else if (v.i.t == 0)
        func = (void *)(size_t) v.i.n;
    else
        func = (void *)(size_t) v.f;

#ifndef __EMSCRIPTEN__
    if ((size_t)func < 0x10000)
        error (8, NULL); // 'Address out of range'
#endif

    while (*esi == ',')
        {
        esi++;
        v = expr ();
        if (v.s.t == -1)
            {
            if ((v.s.l != 0) && 
                (*(v.s.p + v.s.l + (char *) zero - 1) == 0))
                parm.i[ni++] = (size_t) (v.s.p + zero); // use in-situ
            else
                {
                int n = (v.s.l + 4) & -4;
                if (n > ((char *)esp-(char *)zero-pfree-STACK_NEEDED))
                    error (0, NULL); // 'No room'
                esp -= n >> 2;
                memcpy ((char *)esp, v.s.p + zero, v.s.l);
                memset ((char *)esp + v.s.l, 0, 1);
                parm.i[ni++] = (size_t) esp;
                }
#ifdef _WIN32
            if (nf < 8) nf++;
#endif
            }
        else if (v.i.t == 0)
            {
            parm.i[ni++] = v.i.n;
#ifdef _WIN32
            if (nf < 8) nf++;
#endif
            }
        else
            {
            parm.f[nf++] = (double) v.f;
#if defined(__x86_64__) || defined(__aarch64__) || defined(ARMHF)
#ifdef _WIN32
            union { double f; long long i; } u;
            u.f = v.f;
            parm.i[ni++] = u.i;
#endif
#else
            union
                {
                double f;
                struct { int l; int h; } i;
                } u;
            u.f = v.f;
            parm.i[ni++] = u.i.l;
            parm.i[ni++] = u.i.h;
#endif
            }
        if ((ni > 16) || (nf > 8)) 
            error (31, NULL); // 'Incorrect arguments'
        }

    while (ni)
        {
        if (parm.i[--ni] == memhdc)
            break;
        }

    if (*esi == TTO)
        {
        esi++;
        nxt ();
        ptr = getput (&type);
        }

    v.i.t = 0;
    if (type == 8)
        {
        v.i.t = 1; // ARM
        v.f = fltcall_ (func, &parm);
        }
    else if (parm.i[ni] == memhdc)
        v.i.n = guicall (func, &parm);
    else
        v.i.n = apicall_ (func, &parm);

    esp = oldesp;

    if ((type != 40) && (type != 8))
        v.i.n = (int) v.i.n;

    if (ptr)
        storen (v, ptr, type);
    }

/*********************************** OSCLI *************************************/

static void xeq_TOSCLI (void)
    {
    fixs (exprs ());
    oscli (accs);
    }

/*********************************** SOUND *************************************/

static void xeq_TSOUND (void)
    {
    if (*esi == TOFF)
        {
        esi++;
        quiet ();
        return;
        }
    short chan = expri ();
    comma ();
    signed char ampl = expri ();
    comma ();
    unsigned char pitch = expri ();
    comma ();
    unsigned char duration = expri ();
    sound (chan, ampl, pitch, duration);
    }

/********************************** ENVELOPE ***********************************/

static void xeq_TENVEL (void)
    {
    int n;
    signed char envelope[14];
    for (n = 0; n < 14; n++)
        {
        envelope[n] = expri ();
        if (n != 13)
            comma ();
        }
    envel (envelope);
    }

/************************************* DIM *************************************/

static void xeq_TDIM (void)
    {
    while (1)
        {
        void *ebp;
        int ebx = 0; // data size
        int ecx = 0; // dims count
        char *edx; // heap pointer
        unsigned char type = 0;
        signed char *oldesi;

        nxt ();
        oldesi = esi;
        ebp = getdim (&type);
        if (ebp == NULL)
            error (10, NULL); // 'Bad DIM statement'
        check ();

        // Get array dimensions:
        if (*esi == '(') // Array or structure array ?
            {
            do
                {
                int n;
                esi++;
                n = expri ();
                if (n < 0)
                    error (10, NULL); // 'Bad DIM statement'
                *--esp = (n + 1);
                ecx += 1;
                }
            while (*esi == ',');
            braket ();
            }

        edx = pfree + (char *) zero; // Must be after getdim and expri!

        // Build structure descriptor:
        if ((type == STYPE) && (*esi != '.'))
            {
            edx += 4; // room for structure size
            ebx = structure ((void **)&edx); 
            ISTORE(pfree + zero, ebx); // structure size
            }

        // Build array descriptor above structure descriptor:
        if (ecx != 0)
            {
            int i;
            char *edi = pfree + (char *) zero;
            if ((edx > edi) && (edx < (char *)esp))
                edi = edx;

            type |= BIT6; // Flag array
            ebx += (type & TMASK);
            *edi++ = (unsigned char) ecx;
            for (i = ecx-1; i >= 0; i--)
                {
                int eax = *(int *)(esp + i);
                ISTORE(edi, eax);
                edi += 4;
                ebx *= eax;
                if (ebx < 0)
                    error (11, NULL); // 'DIM space'
                }
            esp += ecx;
            ecx = ecx * 4 + 1; // size of array descriptor
            }

        // Support DIM a%(100) 100

        if ((ecx != 0) && (!termq ()) && (*esi != ','))
            {
            ebx = 0;
            ecx = 0;
            esi = oldesi;
            ebp = getvar (&type);
            if (ebp == NULL)
                error (16, NULL); // 'Syntax error'
            if (type == 0)
                error (26, NULL); // 'No such variable'
            }

        // Complete DIM processing:
        // ebx = total size of data (excludes descriptors)
        // ecx = size of array descriptor (0 if no array)
        // edx = top of structure descriptor (< pfree if tagged)
        // if (edx < pfree) edx is base (not top) of descriptor!
        // ebp = varptr (*ebp == 1 for PRIVATE)
        // type = type (BIT4 set if structure, BIT6 set if array)

        // ------ Just allocate memory from heap or stack ------

        if ((ebx == 0) && (ecx == 0))
            {
            int n;
            VAR v;

            if ((type & (BIT6 + BIT7)) || (type < 4))
                error (10, NULL); // 'Bad DIM statement
            if (nxt () == TLOCAL)
                {
                esi++;
                n = expri () + 1;
                if (n < 0)
                    error (10, NULL); // 'Bad DIM'
                if (n > 0)
                    {
                    isloc ();
                    n = (n + 7) & -8;
                    if (n > ((char *)esp - (char *)zero - pfree - STACK_NEEDED))
                        error (0, NULL); // No room
                    esp -= (n >> 2);
                    edx = (char *) esp;								
                    *--esp = n;
                    esp -= STRIDE; *(void **)esp = NULL; // 'data'
                    esp -= STRIDE; *(void **)esp = NULL; // 'desc'
                    esp -= STRIDE; *(void **)esp = NULL; // 'varptr'
                    *--esp = 0; // 'type'
                    *--esp = DIMCHK;
                    }
                else
                    edx = (char *) esp;
                }
            else
                {
                n = expri () + 1;
                if (n < 0)
                    error (10, NULL); // 'Bad DIM'
                edx = pfree + (char *) zero;
                if ((edx + n + STACK_NEEDED) > (char*) esp)
                    error (11, NULL); // 'DIM space'
                pfree += n;
                }
            memset (edx, 0, n);
            v.i.t = 0;
            v.i.n = (size_t) edx;
            storen (v, ebp, type);
            }

        // -------- Normal or PRIVATE DIM (on heap) -----------

        else if (ILOAD(ebp) == 0)
            {
            char *edi = pfree + (char *) zero;
            if ((edx < edi) || (edx > (char *) esp))
                {
                edi = edx;  // tag structure
                edx = pfree + (char *) zero;
                }

            if (ecx == 0)
                CSTORE(ebp, edi); // no array
            else
                CSTORE(ebp, edx); // array

            edx += ecx; // add in array descriptor
            if (type & 0x10)
                while ((size_t)edx & 7)
                    {
                    ecx++;
                    edx++;
                    }

            if ((edx + ebx + STACK_NEEDED) > (char *) esp)
                error (11, NULL); // 'DIM space'
            pfree = edx + ebx - (char *) zero;

            if (type == (STYPE + 0x40)) // structure array ?
                {
                char *tmp = edx - ecx; 
                int eax = arrlen ((void **)&tmp);
                edx += (2 * sizeof(size_t)) * eax; // 8 or 16
                while (eax--)
                    {
                    VSTORE(tmp, edi); // struct fmt
                    tmp += sizeof(void*);
                    VSTORE(tmp, edx); // struct data
                    tmp += sizeof(void*);
                    edx += ILOAD(edi);
                    }
                ebx = edx - tmp;
                edx = tmp;
                }

            if (ebx)
                {
                memset (edx, 0, ebx);
                if (type == STYPE)
                    VSTORE(ebp + sizeof(void*), edx);
                }
            }

        // --------------- LOCAL DIM (on stack) -----------------

        else if (VLOAD(ebp) == (void *)1)
            {
            int n;
            char *edi = pfree + (char *) zero;
            int eax = 0;

            if ((edx > edi) && (edx < (char *)esp))
                eax = edx - edi; // size of struct desc
            if ((pfree + (char *) zero + 2*(eax + ecx) + ebx + STACK_NEEDED)
                > (char *)esp)
                error (11, NULL); // 'DIM space'

            n = (ebx + 7) & -8;	 // data size 
            edi = (char *) esp - n; // data pointer
            n += (eax + ecx + 7) & -8; // add descriptors
            esp -= n >> 2;	// make space on stack

            if (ecx == 0)
                CSTORE(ebp, edi - eax); // no array
            else
                CSTORE(ebp, edi - ecx); // array

            if (eax)
                fixup (pfree + zero, edi - eax - ecx - pfree - (char *) zero);
            else if (ecx == 0)
                CSTORE(ebp, edx); // tagged structure

            memcpy (edi - eax - ecx, pfree + zero, eax + ecx); // copy descriptors

            if ((edx > (pfree + (char *) zero)) && (edx < (char*) esp))
                edx = edi - eax - ecx;

            if (type == (STYPE + 0x40)) // structure array ?
                {
                char *tmp = VLOAD(ebp); 
                int eax = arrlen ((void **)&tmp);
                edi += (2 * sizeof(size_t)) * eax; // 8 or 16
                while (eax--)
                    {
                    VSTORE(tmp, edx); // struct fmt
                    tmp += sizeof(void*);
                    VSTORE(tmp, edi); // struct data
                    tmp += sizeof(void*);
                    edi += ILOAD(edx);
                    }
                ebx = edi - tmp;
                edi = tmp;
                }

            *--esp = n;
            esp -= STRIDE;
            *(void **)esp = edi; // data
            esp -= STRIDE;
            *(void **)esp = VLOAD(ebp);
            esp -= STRIDE;
            *(void **)esp = ebp; // varptr
            *--esp = (int)type;
            *--esp = DIMCHK;

            if (ebx)
                {
                memset (edi, 0, ebx);
                if (type == STYPE)
                    VSTORE(ebp + sizeof(void*), edi);
                }
            }

        // ------ Re-DIM an existing array or structure ------

        else // compare descriptors to check same dimensions 
            {
            char *edi = VLOAD(ebp); // old pointer
            if (edx < (pfree + (char *) zero))
                {
                char *eax = edi;
                if (ecx)
                    eax = VLOAD(edi + ecx);
                if (eax != edx)
                    error (10, NULL); // 'Bad DIM'
                }
            else if (edx > (pfree + (char *) zero))
                {
                if (ecx)
                    edi -= edx - pfree - (char *) zero;
                ecx += edx - pfree - (char *) zero;
                fixup (pfree + zero, edi - pfree - (char *) zero);
                }
            if ((ecx != 0) && memcmp (pfree + zero, edi, ecx))
                error (10, NULL); // 'Bad DIM statement'
            }

        if (nxt () != ',')
            return;
        esi++;
        }
    }

/*********************************** Label *************************************/

static void xeq_LABEL (void)    // '('
    {
    while ((*esi != 0x0D) && (*esi++ != ')'));
    }

/******************************* Star command **********************************/

static void xeq_STAR (void)     // '*'
    {
    oscli ((char *) esi);
    esi = (signed char*) memchr ((char *) esi, 0x0D, 255);
    }

/********************************* Assembler ***********************************/

static void xeq_ASSEM (void)    // '['
    {
    assemble ();
    }

/******************************** End of Line **********************************/

static void xeq_EOL (void)  // 0x0D
    {
	signed char al;
    esi = tmpesi + 1;
    newlin ();
    al = *esi;
    if (al == TELSE)
        {
        esi -= 3;
        esi = nsurch (TENDIF, 0, TTHEN, TENDIF, 0);
        if (esi == NULL)
            error (49, NULL); // 'Missing ENDIF'
        esi += 3;
        }
    else if ((al == TWHEN) || (al == TOTHERWISE))
        {
        esi -= 3;
        esi = nsurch (TENDCASE, 0, TOF, TENDCASE, 0);
        if (esi == NULL)
            error (47, NULL); // 'Missing ENDCASE'
        esi += 3;
        }
    else if (al == '\\')
        {
        if (al != *(esi-5))
            error (16, NULL); // Syntax error
        esi = (signed char*) memchr ((const char *) esi, 0x0D, 255);
        }
    }

/*********************************** LEFT$ *************************************/
/********************************** RIGHT$ *************************************/
/*********************************** MID$  *************************************/

// static void xeq_TRIGHT (void)
// static void xeq_TMID (void)
static void xeq_TLEFT (void)
    {
    VAR v,r;
    int n = 1, s = 0;
    unsigned char type;
    void *ptr = getvar (&type);
    if (ptr == NULL)
        error (16, NULL); // 'Syntax error'
    if (type == 0)
        error (26, NULL); // 'No such variable'
    if (type & BIT6)
        error (14, NULL); // 'Bad use of array'
    if (!(type & BIT7))
        error (6, NULL);  // 'Type mismatch'

    if (*esi == ',')
        {
        esi++;
        n = expri ();
        if (al_token == TMID)
            {
            s = n - 1;
            n = 0x7FFFFFFF;
            if (*esi == ',')
                {
                esi++;
                n = expri ();
                }
            }
        }
    else
        {
        if (al_token == TMID)
            error (16, NULL); // Syntax error
        if (al_token == TLEFT)
            n = -1;
        }

    v = loads (ptr, type); // simply to find string length
    braket ();
    equals ();
    r = exprs ();

    if (n == -1)
        n = v.s.l - 1;
    if (n > (int) r.s.l) // careful with signedness!
        n = r.s.l;
    if ((s < 0) || (s >= v.s.l) || (n == 0))
        return;
    if (al_token == TRIGHT)
        s = (unsigned int)v.s.l - n;
    if (s < 0)
        s = 0;
    if (n > ((unsigned int)v.s.l - s))
        n = (unsigned int)v.s.l - s;

    memcpy (v.s.p + s + zero, r.s.p + zero, n);
    }

/************************************* LET *************************************/

static void xeq_TLET (void)
    {
    void *ptr, *ebp;
    unsigned char type;

    ptr = getput (&type);
    if ((type & (BIT4 | BIT6)) == 0) // scalar
        {
        if (type < 128)
            assign (ptr, type); // scalar numeric
        else
            assigns (ptr, type); // scalar string
        return;
        }

    if ((type & BIT6) == 0) // structure
        {
        int len, srclen;
        void *srcptr, *fmt, *srcfmt;
        unsigned char srctype;
        equals ();
        nxt ();
        srcptr = getvar (&srctype);
        if (srcptr == NULL)
            error (56, NULL); // 'Bad use of structure'
        if (srctype == 0)
            error (26, NULL); // 'No such variable'
        if (srctype != type)
            error (6, NULL);  // 'Type mismatch'
        fmt = VLOAD(ptr);
        srcfmt = VLOAD(srcptr);
        if ((fmt < (void *)2) || (srcfmt < (void *)2))
            error (56, NULL); // 'Bad use of structure'
        len = ILOAD(fmt);
        srclen = ILOAD(srcfmt);
        if (len != srclen)
            error (6, NULL);  // 'Type mismatch'
        memcpy (VLOAD(ptr + sizeof(void *)),
            VLOAD(srcptr + sizeof(void *)), len);
        return;
        }

    ptr = VLOAD(ptr);
    if (ptr < (void *)2)
        error (14, NULL); // 'Bad use of array'

    signed char op = nxt ();
    unsigned int ecx = arrlen (&ptr); // number of array elements
    unsigned int eax = ecx * (type & TMASK); // array size in bytes
    if (eax > ((char *)esp - (char *)zero - pfree - STACK_NEEDED))
        error (0, NULL); // 'No room'
    esp -= (eax + 3) >> 2;
    ebp = esp;
    memset ((char *)ebp, 0, eax); // Zero 'array descriptors'

    esi++;
    if (op != '=')
        {
        if ((op == '+') || (op == '-') || (op == '*') || (op == '/') ||
            ((op >= TAND) & (op <= TOR)))
            equals ();
        else
            error (4, NULL); // Mistake
        }

    ecx = expra (ebp, ecx, type); // Evaluate array expression

    type &= ~BIT6;
    if (type < 128)
        {
        while (ecx--)
            {
            modify (loadn((void *)ebp, type), ptr, type, op);
            ebp += type & TMASK; // GCC extension
            ptr += type & TMASK; // GCC extension
            }
        }
    else
        {
        VAR v = {0};
        while (ecx--)
            {
            modifs (NLOAD(ebp), ptr, type, op);
            stores (v, ebp, type);
            ebp += 8; // GCC extension (void has size 1)
            ptr += 8; // GCC extension (void has size 1)
            }
        }
    esp += (eax + 3) >> 2;
    }

static void xeq_default (void)
    {
    esi = tmpesi;
    xeq_TLET ();
    }

typedef void (*xeq_func)(void);

static const xeq_func xeq_list[] =
    {
    xeq_default,    // TAND	        -128
	xeq_default,    // TDIV	        -127
	xeq_default,    // TEOR	        -126
	xeq_default,    // TMOD	        -125
	xeq_default,    // TOR	        -124
	xeq_TERROR,     // TERROR       -123
	xeq_TLINE,      // TLINE        -122
	xeq_TOFF,       // TOFF	        -121
	xeq_default,    // TSTEP        -120
	xeq_default,    // TSPC	        -119
	xeq_default,    // TTAB	        -118
	xeq_TREM,       // TELSE        -117
	xeq_default,    // TTHEN        -116
    xeq_TLINO,      // TLINO        -115
    xeq_default,    // TOPENIN      -114
    xeq_default,    // TPTRR        -113
    xeq_default,    // TPAGER       -112
    xeq_default,    // TTIMER       -111
    xeq_default,    // TLOMEMR      -110
    xeq_default,    // THIMEMR      -109
    xeq_default,    // TABS         -108
    xeq_default,    // TACS         -107
    xeq_default,    // TADVAL       -106
    xeq_default,    // TASC         -105
    xeq_default,    // TASN         -104
    xeq_default,    // TATN         -103
    xeq_default,    // TBGET        -102
    xeq_default,    // TCOS         -101
    xeq_default,    // TCOUNT       -100
    xeq_default,    // TDEG         -99
    xeq_default,    // TERL         -98
    xeq_default,    // TERR         -97
    xeq_default,    // TEVAL        -96
    xeq_default,    // TEXP         -95
    xeq_TEXTR,      // TEXTR        -94
    xeq_default,    // TFALSE       -93
    xeq_default,    // TFN          -92
    xeq_default,    // TGET         -91
    xeq_default,    // TINKEY       -90
    xeq_default,    // TINSTR       -89
    xeq_default,    // TINT         -88
    xeq_default,    // TLEN         -87
    xeq_default,    // TLN          -86
    xeq_default,    // TLOG         -85
    xeq_default,    // TNOT         -84
    xeq_default,    // TOPENUP      -83
    xeq_default,    // TOPENOUT     -82
    xeq_default,    // TPI          -81
    xeq_default,    // TPOINT       -80
    xeq_default,    // TPOS         -79
    xeq_default,    // TRAD         -78
    xeq_default,    // TRND         -77
    xeq_default,    // TSGN         -76
    xeq_default,    // TSIN         -75
    xeq_default,    // TSQR         -74
    xeq_default,    // TTAN         -73
    xeq_default,    // TTO          -72
    xeq_default,    // TTRUE        -71
    xeq_default,    // TUSR         -70
    xeq_default,    // TVAL         -69
    xeq_default,    // TVPOS        -68
    xeq_default,    // TCHR         -67
    xeq_default,    // TGETS        -66
    xeq_default,    // TINKEYS      -65
    xeq_TLEFT,      // TLEFT        -64
    xeq_TLEFT,      // TMID         -63
    xeq_TLEFT,      // TRIGHT       -62
    xeq_default,    // TSTR         -61
    xeq_default,    // TSTRING      -60
    xeq_default,    // TEOF         -59
    xeq_default,    // TSUM         -58
    xeq_TWHILE,     // TWHILE       -57
    xeq_TCASE,      // TCASE        -56
    xeq_TWHEN,      // TWHEN        -55
    xeq_default,    // TOF	        -54
    xeq_TENDIF,     // TENDCASE     -53
    xeq_TWHEN,      // TOTHERWISE   -52
    xeq_TENDIF,     // TENDIF	    -51
    xeq_TENDWHILE,  // TENDWHILE    -50
    xeq_TPTRL,      // TPTRL	    -49
    xeq_TPAGEL,     // TPAGEL	    -48
    xeq_TTIMEL,     // TTIMEL	    -47
    xeq_TLOMEML,    // TLOMEML	    -46
    xeq_THIMEML,    // THIMEML	    -45
    xeq_TSOUND,     // TSOUND	    -44
    xeq_TBPUT,      // TBPUT	    -43
    xeq_TCALL,      // TCALL	    -42
    xeq_TCHAIN,     // TCHAIN	    -41
    xeq_TCLEAR,     // TCLEAR	    -40
    xeq_TCLOSE,     // TCLOSE	    -39
    xeq_TCLG,       // TCLG	        -38
    xeq_TCLS,       // TCLS	        -37
    xeq_TREM,       // TDATA	    -36
    xeq_TREM,       // TDEF	        -35
    xeq_TDIM,       // TDIM	        -34
    xeq_TDRAW,      // TDRAW	    -33
    xeq_TEND,       // TEND	        -32
    xeq_TENDPROC,   // TENDPROC     -31
    xeq_TENVEL,     // TENVEL	    -30
    xeq_TFOR,       // TFOR	        -29
    xeq_TGOSUB,     // TGOSUB	    -28
    xeq_TGOTO,      // TGOTO	    -27
    xeq_TGCOL,      // TGCOL	    -26
    xeq_TIF,        // TIF	        -25
    xeq_TINPUT,     // TINPUT	    -24
    xeq_TLET,       // TLET	        -23
    xeq_TLOCAL,     // TLOCAL	    -22
    xeq_TMODE,      // TMODE	    -21
    xeq_TMOVE,      // TMOVE	    -20
    xeq_TNEXT,      // TNEXT	    -19
    xeq_TON,        // TON	        -18
    xeq_TVDU,       // TVDU	        -17
    xeq_TPLOT,      // TPLOT	    -16
    xeq_TPRINT,     // TPRINT	    -15
    xeq_TPROC,      // TPROC	    -14
    xeq_TREAD,      // TREAD	    -13
    xeq_TREM,       // TREM	        -12
    xeq_TREPEAT,    // TREPEAT	    -11
    xeq_TREPORT,    // TREPORT	    -10
    xeq_TRESTOR,    // TRESTOR	    -9
    xeq_TRETURN,    // TRETURN	    -8
    xeq_TRUN,       // TRUN	        -7
    xeq_TSTOP,      // TSTOP	    -6
    xeq_TCOLOUR,    // TCOLOUR	    -5
    xeq_TTRACE,     // TTRACE	    -4
    xeq_TUNTIL,     // TUNTIL	    -3
    xeq_TWIDTH,     // TWIDTH	    -2
    xeq_TOSCLI,     // TOSCLI	    -1
    xeq_NULL,       //              0
    xeq_TCIRCLE,    // TCIRCLE	    1
    xeq_TELLIPSE,   // TELLIPSE     2
    xeq_TFILL,      // TFILL	    3
    xeq_TMOUSE,     // TMOUSE	    4
    xeq_TORIGIN,    // TORIGIN	    5
    xeq_TQUIT,      // TQUIT	    6
    xeq_TRECT,      // TRECT	    7
    xeq_TSWAP,      // TSWAP	    8
    xeq_TSYS,       // TSYS	        9
    xeq_TTINT,      // TTINT	    10
    xeq_TWAIT,      // TWAIT	    11
    xeq_TINSTALL,   // TINSTALL     12
    xeq_EOL,        //              13
    xeq_TPRIVATE,   // TPRIVATE     14
    xeq_default,    // TBY	        15
    xeq_TEXIT,      // TEXIT	    16
    xeq_default,    //              17
    xeq_default,    //              18
    xeq_default,    //              19
    xeq_default,    //              20
    xeq_default,    //              21
    xeq_default,    //              22
    xeq_default,    //              23
    xeq_default,    //              24
    xeq_default,    //              25
    xeq_default,    //              26
    xeq_default,    //              27
    xeq_default,    //              28
    xeq_default,    //              29
    xeq_default,    //              30
    xeq_default,    //              31
    xeq_default,    // ' '          32
    xeq_default,    // '!'          33
    xeq_default,    // '"'          34
    xeq_default,    // '#'          35
    xeq_default,    // '$'          36
    xeq_default,    // '%'          37
    xeq_default,    // '&'          38
    xeq_default,    // '''          39
    xeq_LABEL,      // '('          40
    xeq_default,    // ')'          41
    xeq_STAR,       // '*'          42
    xeq_default,    // '+'          43
    xeq_default,    // '''          44
    xeq_default,    // '-'          45
    xeq_default,    // '.'          46
    xeq_default,    // '/'          47
    xeq_default,    // '0'          48
    xeq_default,    // '1'          49
    xeq_default,    // '2'          50
    xeq_default,    // '3'          51
    xeq_default,    // '4'          52
    xeq_default,    // '5'          53
    xeq_default,    // '6'          54
    xeq_default,    // '7'          55
    xeq_default,    // '8'          56
    xeq_default,    // '9'          57
    xeq_COLON,      // ':'          58
    xeq_default,    // ';'          59
    xeq_default,    // '<'          60
    xeq_default,    // '='          61
    xeq_default,    // '>'          62
    xeq_default,    // '?'          63
    xeq_default,    // '@'          64
    xeq_default,    // 'A'          65
    xeq_default,    // 'B'          66
    xeq_default,    // 'C'          67
    xeq_default,    // 'D'          68
    xeq_default,    // 'E'          69
    xeq_default,    // 'F'          70
    xeq_default,    // 'G'          71
    xeq_default,    // 'H'          72
    xeq_default,    // 'I'          73
    xeq_default,    // 'J'          74
    xeq_default,    // 'K'          75
    xeq_default,    // 'L'          76
    xeq_default,    // 'M'          77
    xeq_default,    // 'N'          78
    xeq_default,    // 'O'          79
    xeq_default,    // 'P'          80
    xeq_default,    // 'Q'          81
    xeq_default,    // 'R'          82
    xeq_default,    // 'S'          83
    xeq_default,    // 'T'          84
    xeq_default,    // 'U'          85
    xeq_default,    // 'V'          86
    xeq_default,    // 'W'          87
    xeq_default,    // 'X'          88
    xeq_default,    // 'Y'          89
    xeq_default,    // 'Z'          90
    xeq_ASSEM,      // '['          91
    xeq_default,    // '\'          92
    xeq_default,    // ']'          93
    xeq_default,    // '^'          94
    xeq_default,    // '_'          95
    xeq_default,    // '`'          96
    xeq_default,    // 'a'          97
    xeq_default,    // 'b'          98
    xeq_default,    // 'c'          99
    xeq_default,    // 'd'          100
    xeq_default,    // 'e'          101
    xeq_default,    // 'f'          102
    xeq_default,    // 'g'          103
    xeq_default,    // 'h'          104
    xeq_default,    // 'i'          105
    xeq_default,    // 'j'          106
    xeq_default,    // 'k'          107
    xeq_default,    // 'l'          108
    xeq_default,    // 'm'          109
    xeq_default,    // 'n'          110
    xeq_default,    // 'o'          111
    xeq_default,    // 'p'          112
    xeq_default,    // 'q'          113
    xeq_default,    // 'r'          114
    xeq_default,    // 's'          115
    xeq_default,    // 't'          116
    xeq_default,    // 'u'          117
    xeq_default,    // 'v'          118
    xeq_default,    // 'w'          119
    xeq_default,    // 'x'          120
    xeq_default,    // 'y'          121
    xeq_default,    // 'z'          122
    xeq_default,    // '{'          123
    xeq_default,    // '|'          124
    xeq_default,    // '}'          125
    xeq_default,    // '~'          126
    xeq_default,    //              127
    };

// Execute the program:
VAR xeq (void)
    {
    bFlgChk = true;
	while (1) // for each statement
	    {
#if PICO_STACK_CHECK & 0x01
		if((&al < (signed char *)libtop + 0x800) && (&al >= (signed char *)userRAM))
			error(0, "Processor stack too deep!");
#endif
	 	if (( bFlgChk ) && (flags & (KILL + PAUSE + ALERT + ESCFLG)))
		    {
			heapptr jump = xtrap ();
			if (jump)
			    {
				esp -= STRIDE;
				*(void **)esp = esi;
				*--esp = GOSCHK;
				esi = jump + (signed char *) zero;
			    }
		    }
        bFlgChk = true;
		al_token = nxt ();
		tmpesi = esi;
		curlin = esi - (signed char *) zero;
		while (*++esi == ' ');

        if ( al_token == '=' ) return xeq_ENDFN ();
		xeq_list[(int)al_token + 0x80]();
        }
    }
