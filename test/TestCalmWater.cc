#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Waves.h"
#include "visual/Caustic.h"




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

  int i;
  // Test the repo

 
  CalmWaterSurface surf;
  surf.Read("data/wave.repo");
  surf.SetQueueSize(250);
  surf.SetInitSize(1);
  int k = 0;
  int x, y;
  double delta = 0.;
  for (i=1000; i<1800; i++) {
    char name[512];
    sprintf(name, "data/calm%d.bmp", i);
    Bitmap out_single;
  
    surf.Get(out_single, delta);
    delta += 1.;


  
    Caustic cc;
    Bitmap caust;
    cc.ComputeBottom(caust, out_single, 50.);
    
    /*
    for (x=0; x<out_single.X(); x++) {
      for (y=0; y<out_single.Y(); y++) {
	RGBPixel & p = out_single.Get(x, y);
	double r = p.r();
	double b = p.b();
	double g = p.g();
	b = 0.999 - (0.999 - b)*0.4;
	g = 0.999 - (0.999 - g)*0.5;
	r = 0.999 - (0.999 - r)*0.8;
	p.Set_b(b);       
	p.Set_g(g);       
	p.Set_r(r);       
      }
      }*/

    out_single.Write(name);
    cout << "Wrote " << name << endl;




    for (x=0; x<caust.X(); x++) {
      for (y=0; y<caust.Y(); y++) {
	RGBPixel & p = caust.Get(x, y);
	double b = p.b();
	double g = p.g();
	b = 0.999 - (0.999 - b)*0.6;
	g = 0.999 - (0.999 - g)*0.8;
	p.Set_b(b);       
	p.Set_g(g);       
      }
    }

    sprintf(name, "data/calm_caustic%d.bmp", i);
    caust.Write(name);

    Bitmap both;
    both.SetSize(512, 512);
    for (x=0; x<out_single.X(); x++) {
      for (y=0; y<out_single.Y(); y++) {
	RGBPixel & p1 = out_single.Get(x, y);
	RGBPixel & p2 = caust.Get(x, y);
	both.Get(x, y) = p2;
 	both.Get(x, y+256) = p1;
      }
    }
    sprintf(name, "data/both%d.bmp", i);
    both.Write(name);

  }
  


  return 0;
}
