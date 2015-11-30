#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"
#include <math.h>
#include "base/RandomStuff.h"
#include "visual/Color.h"
#include "visual/WavePainter.h"



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
  


  int i, j;
 
  int x = 512;
  int y = 256;

  svec<OneWave> waves;
  waves.resize(200);
  for (i=0; i<waves.isize(); i++) {
    OneWave & wave = waves[i];
    wave.Center().X() = RandomFloat(x-1);
    wave.Center().Y() = RandomFloat(y-1);
    wave.Time() = 50.+RandomFloat(300.);
    //wave.Time() = 2.;

  }


  double deltatime = 1.5;
  for (int k=100; k<550; k++) {
    Bitmap bitmap;

    int index = RandomInt(waves.isize());
    OneWave & wave = waves[index];
    wave.Center().X() = RandomFloat(x-1);
    wave.Center().Y() = RandomFloat(y-1);
    //wave.Time() = 50.+RandomFloat(300.);
    wave.Time() = 2.;

    bitmap.SetSize(x, y);
    
    for (i=0; i<bitmap.X(); i++) {
      for (j=0; j<bitmap.Y(); j++) {
	RGBPixel & c = bitmap.Get(i, j);
	c.Set_R(128);
	c.Set_G(128);
	c.Set_B(128);
      }
    }

    for (i=0; i<waves.isize(); i++) {      
      waves[i].DrawNormalsSmooth(bitmap, deltatime);
    }
    
    char name[512];
    sprintf(name, "data/rain%d.bmp", k);
    bitmap.Write(name);

  }




  

  return 0;
}
