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

 
  bool bUseLeaf = false;

  CalmWaterSurface surf;

  surf.SetSize(1024, 512);

  cout << "Read." << endl;
  surf.Read("data/wave.repo_big");
  cout << "Done." << endl;
  surf.SetQueueSize(200);
  //surf.SetInitSize(1);
  int k = 0;
  int x, y;
  double delta = 0.;

  int skip = surf.GetQueueSize();

  int counter = 0;

  int end = 1500;

  double depth = 10.;

  Bitmap leaf;
  if (bUseLeaf)
    leaf.Read("data/Textures/orange_leaf_small2.bmp");
  Coords2D leafcoords;
  leafcoords.X() = 200;
  leafcoords.Y() = 125;
  
  Bitmap ground;
  
  cout << "Read image." << endl;
  ground.Read("data/Textures/pebbles_water3.bmp");

  cout << "Go." << endl;
  for (i=1000; i<end; i++) {
    char name[512];
    sprintf(name, "data/calm%d.bmp", i);
    Bitmap out_single;
  
    surf.Get(out_single, delta);
    delta += 1.;

    if (counter < skip) {
      counter++;
      continue;
    }
  
    if (end - i == skip) {
      surf.FromCache();
    }

    Caustic cc;
    Bitmap caust;
    Bitmap surface;
    cc.SetRefractionIndex(1.33);
    cc.SetOverlay(leaf, leafcoords);

    double vv = out_single.Get(leafcoords.X() + leaf.X()/2,
			       leafcoords.Y() + leaf.Y()/2).r();
    cout << "vv=" << vv << endl;


    leafcoords.X() += 5.* (vv-0.5);
    leafcoords.Y() += 3.* (vv-0.5);

    // Original used for the video is 50
    //cc.ComputeBottom(caust, out_single, 50.);
    cc.ComputeBottom(caust, out_single, 100.);
    depth += 1.;
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

    

    out_single.Overlay(leaf, leafcoords.X(), leafcoords.Y());

    out_single.Write(name);
    cout << "Wrote " << name << endl;

    Bitmap tmp = ground;
    tmp.Multiply(caust, 2.0);
    caust = tmp;
    /*
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
      }*/
    

    sprintf(name, "data/calm_caustic%d.bmp", i);
    caust.Write(name);

    cc.ComputeTop(surface, out_single, caust, 80.);
    
    sprintf(name, "data/calm_surface%d.bmp", i);
    surface.Write(name);

    /*
    Bitmap both;
    both.SetSize(512, 512);
    for (x=0; x<out_single.X(); x++) {
      for (y=0; y<out_single.Y(); y++) {
	//RGBPixel & p1 = out_single.Get(x, y);
	RGBPixel & p1 = surface.Get(x, y);
	RGBPixel & p2 = caust.Get(x, y);
	both.Get(x, y) = p2;
 	both.Get(x, y+256) = p1;
      }
    }
    sprintf(name, "data/both%d.bmp", i);
    both.Write(name);
    */

    counter++;
  }
  


  return 0;
}
