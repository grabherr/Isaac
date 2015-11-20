#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"


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
  

  Bitmap bitmap;

  bitmap.SetSize(512, 256);
  int i, j;

  for (i=0; i<bitmap.X(); i++) {
    for (j=0; j<bitmap.Y(); j++) {
      RGBPixel & c = bitmap.Get(i, j);
      c.Set_R(250);
    }
  }

  bitmap.Write("red.bmp");


  bitmap.Read("data/Skyboxes/noise4.bmp");
  for (i=10; i<100; i++) {
    for (j=10; j<100; j++) {
      RGBPixel & c = bitmap.Get(i, j);
      c.Set_B(250);
    }
  }
  bitmap.Write("test.bmp");
  

  return 0;
}
