#include <stdio.h>
#include <stdlib.h>
#include <X11/XKBlib.h>

#include "arg.h"

#define err(r, msg)\
	do {\
		fprintf(stderr, "%s: %s.\n", argv0, msg);\
		exit(r);\
	} while (0)

char *argv0;
int lstflag;
int nmbrflag;
int setflag;

int setto;
int setsign;
int curr;
int grpn;
Atom *grps;
Display *dpy;
XkbDescPtr kb;

void
usage(void)
{
	err(1, "usage: %s [-l | -n] [[-t | -a | -s] number]");
}

void
init(void)
{
	XkbStateRec state;

	if (!(dpy = XOpenDisplay(NULL)))
		err(1, "failed to open display");
	if (!(kb = XkbAllocKeyboard()))
		err(1, "failed to allocate keyboard");
	if (XkbGetNames(dpy, XkbGroupNamesMask, kb))
		err(1, "failed to get Group names");
	if (XkbGetState(dpy, XkbUseCoreKbd, &state))
		err(1, "failed to get keyboard state");

	curr = state.group;
	/* count groups */
	for (grps = kb->names->groups, grpn = 1;
	     grpn < XkbNumKbdGroups && grps[grpn];
	     grpn++);
}

void
cleanup(void)
{
	XFree(kb);
	XCloseDisplay(dpy);
}

void
printlt(void)
{
	int i;
	char *name;

	for (i = 0; i < grpn; i++) {
		if (i != curr && !lstflag)
			continue;
		if (nmbrflag) {
			printf("%d\n", i);
		} else {
			name = XGetAtomName(dpy, grps[i]);
			puts(name);
			XFree(name);
		}
	}
}

void
setlt(void)
{
	XkbLockGroup(dpy, XkbUseCoreKbd, setto + setsign * curr);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'l':
		lstflag = 1;
		break;
	case 'n':
		nmbrflag = 1;
		break;
	case 'a':
		/* FALLTHROUGH */
	case 's':
		setsign = ARGC() == 'a' ? 1 : -1;
		/* FALLTHROUGH */
	case 't':
		setflag = 1;
		setto = strtol(EARGF(usage()), NULL, 10);
		break;
	default:
		usage();
	} ARGEND
	if (argc > 0)
		usage();

	init();
	if (setflag)
		setlt();
	else
		printlt();
	cleanup();

	return 0;
}
