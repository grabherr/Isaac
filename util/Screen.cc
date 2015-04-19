#include <X11/Xlib.h>
#include "util/Screen.h"

void GetCurrScreenRes(int & width, int & height)
{

  Display* disp = XOpenDisplay(NULL);
  Screen*  scrn = DefaultScreenOfDisplay(disp);
  height = scrn->height;
  width  = scrn->width;
}
	
