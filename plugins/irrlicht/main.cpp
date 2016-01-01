// Irrlicht (http://irrlicht.sourceforge.net/) client for Isaac.
// This code is based on the coding examples provided with
// the source code.

//#define FORCE_DEBUG


#include "irrclient.h"





void GetCurrScreenRes(int & width, int & height)
{

  Display* disp = XOpenDisplay(NULL);
  Screen*  scrn = DefaultScreenOfDisplay(disp);
  height = scrn->height;
  width  = scrn->width;
}




// Main function

int main(int argc,char** argv)
{
  int resX = 1024;
  int resY = 768;
  bool bFS = false;

  if (argc >= 2) {
    resX = atol(argv[1]);
    resY = atol(argv[2]);
  }

  if (resX == -1 || resX == 0) {
    GetCurrScreenRes(resX, resY);
    std::cout << "Detecting resolution: " << resX << "x" << resY << endl;
  }

  if (argc > 2) {
    if (strcmp(argv[3], "true") == 0)
      bFS = true;
  }
    
  
  
  IrrlichtServer irr(resX, resY, bFS);

  irr.AddCamera(0, 0, 0);

  irr.WaitLoadTerrain();
  
  //irr.AddCube();

  irr.Run();

  
  
  return 0;
}

