#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#include "arg.h"

#define die(r, msg)\
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
Atom *grps;
Display *dpy;

void
usage(void)
{
	die(1, "usage: %s [-l | -n] [[-t | -a | -s] number]");
}

void
init(void)
{
	XkbStateRec state;

	if (!(dpy = XOpenDisplay(NULL)))
		die(1, "cannot open display");
	if (XkbGetState(dpy, XkbUseCoreKbd, &state))
		die(1, "cannot get keyboard state");

	curr = state.group;
}

void
cleanup(void)
{
	XCloseDisplay(dpy);
}

void
printlt(void)
{
	int i;
	char *tmp, *str, *tok;
	XkbRF_VarDefsRec vd;

	if (!XkbRF_GetNamesProp(dpy, &tmp, &vd))
		die(1, "cannot extract keyboard properties");

	if (!(str = strdup(vd.layout)))
		die(1, "strdup failed");

	tok = strtok(str, ",");
	for (i = 0; tok; tok = strtok(NULL, ","), i++) {
		if (i != curr && !lstflag)
			continue;
		if (nmbrflag)
			printf("%d\n", i);
		else
			puts(tok);
	}

	free(str);
	free(tmp);
	XFree(vd.options);
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
	case 's':
		setsign = ARGC() == 'a' ? 1 : -1;
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
