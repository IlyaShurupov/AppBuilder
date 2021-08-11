
#ifdef unix

#include <X11/Xlib.h>
#include <iostream>
#include "X11/keysym.h"

// param ks  like XK_Shift_L, see /usr/include/X11/keysymdef.h

bool keyIsDown(int code) {
	
	// KeySym ks = code;
	KeySym ks;

    Display *dpy = XOpenDisplay(":0");
    char keys_return[32];
    XQueryKeymap(dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(dpy, ks);
    bool isPressed = !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
    XCloseDisplay(dpy);
    return isPressed;
}

#endif