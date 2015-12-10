#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"
#include "visual/Canvas.h"
#include "physics/Gas.h"
#include "base/RandomStuff.h"

int main( int argc, char** argv )
{

  /* commandArg<string> fileCmmd("-i","input file");
  commandArg<int> bCmmd("-from","from column");
  commandArg<int> eCmmd("-to","to column");
  commandArg<bool> nCmmd("-newline","add newline", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(bCmmd);
  P.registerArg(eCmmd);
  P.registerArg(nCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  int from = P.GetIntValueFor(bCmmd);
  int to = P.GetIntValueFor(eCmmd);
  bool bN = P.GetBoolValueFor(nCmmd);*/
  
  Gas gas;
  gas.SetBoundaries(Coordinates(0, 0, 0), Coordinates(128, 20, 128)); 
  
  

  int i, j;

  for (i=0; i<200; i++) {
    GasParticle & p = gas[i];
    p.Pos() = Coordinates(64+RandomFloat(5), 10, 64+RandomFloat(5));
  }


  int k = 1000;
  for (j=0; j<50; j++) {
 
    Bitmap bitmap;
    bitmap.SetSize(512, 512);
    /*for (int x=0; x<bitmap.X(); x++) {
      for (int y=0; y<bitmap.Y(); y++) {
	RGBPixel & c = bitmap.Get(x, y);
	c.Set_R(255);
	c.Set_G(255);
	c.Set_B(255);
	//cout << "Set" << endl;
      }
      }*/

    
    for (i=0; i<gas.isize(); i++) {
      int x = 4*gas[i].Pos()[0];
      int y = 4*gas[i].Pos()[2];
      //cout << "Pos " << x << " " << y << endl;
      if (x < 0 || y < 0 || x >= bitmap.X() || y >= bitmap.Y())
	continue;
      RGBPixel & c = bitmap.Get(x, y);
      c.Set_R(255);
      c.Set_G(255);
      c.Set_B(255);
     
    }

    Canvas c;
    c.FromBitmap(bitmap);
    for (int x=0; x<c.X(); x++) {
      for (int y=0; y<c.Y(); y++) {
	CanvasPixel & p = c.Pixel(x, y);
	p.R() *= 4.;
	p.G() *= 4.;
	p.B() *= 4.;
	//cout << "Set" << endl;
      }
    }
   

    c.Smooth();
    c.Smooth();
    c.Smooth();
    c.Smooth();
    c.ToBitmap(bitmap);

    char name[256];
    sprintf(name, "data/gas%d.bmp", k);
    bitmap.Write(name);
    k++;
    cout << "Wrote " << name << endl;


    for (i=0; i<gas.isize(); i++) {
      GasParticle & p = gas[i];
      if (p.Pos()[2] < 7 && p.Pos()[0] > 54 && p.Pos()[0] < 74)
	p.Impulse()[2] += 18.;
    }

    cout << "Call update" << endl;
    gas.Update(0.05);
    cout << "Done update" << endl;
    
 }

  return 0;
}
