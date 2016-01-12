#define FORCE_DEBUG

#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"
#include "visual/Canvas.h"
#include "visual/BitmapManip.h"



int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","input file");
  commandArg<string> fileCmmd2("-i2","input file 2", "");
  commandArg<string> oCmmd("-o","output file");
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(fileCmmd2);
  P.registerArg(oCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string fileName2 = P.GetStringValueFor(fileCmmd2);
  string outName = P.GetStringValueFor(oCmmd);
 

  Bitmap bitmap;
  bitmap.Read(fileName);
  if (fileName2 == "")
    fileName2= fileName;

  Bitmap bitmap2;
  bitmap2.Read(fileName2);

  Canvas c1;
  c1.FromBitmap(bitmap);
  Canvas c2;
  c2.FromBitmap(bitmap2);

  if (c1.X() != c2.X() || c1.Y() != c2.Y()) {
    cout << "Image sizes must match!!!" << endl;
    return -1;
  }

  Canvas out;
  int lap = 200;
  int i, j, k;

  out.resize(c1.X()*2-lap, c1.Y());
  DynProg dp;
  dp.Setup(c1.Y(), lap);

  for (k=0; k<c1.Y(); k++) {
    double min = -1.;
    double pos = -1;
    for (i=c1.X()-lap; i<c1.X(); i++) {     
      j = i-(c1.X()-lap);
      //cout << i << " " << j << endl; 
      const CanvasPixel & a = c1.Pixel(i, k);
      const CanvasPixel & b = c2.Pixel(j, k);  
      double d = a.Distance(b);
      dp.Set(k, j, d);
      if (min < 0 || d < min) {
	min = d;
	pos = i;
      }
    }
  }
  svec<int> result;
  dp.DoIt(result);
  for (k=0; k<c1.Y(); k++) {
    int pos = result[k] - lap + c1.X();
    cout << "Found best: " << pos << endl;
    for (i=0; i<pos; i++) {
      const CanvasPixel & a = c1.Pixel(i, k);
      CanvasPixel & f = out.Pixel(i, k);
      f = a;
    }
    for (i=pos-(c1.X()-lap); i<c1.X(); i++) {
      const CanvasPixel & a = c2.Pixel(i, k);
      CanvasPixel & f = out.Pixel(i+c1.X()-lap, k);
      f = a;
    }
    
  }


  /*
  int index1 = -1;
  int index2 = -1;
  double minDist = -1.;

  for (i=0; i<c.Y(); i++) {
    cout << i << " of " << c.Y() << endl;
    for (j=i+500; j<c.Y(); j++) {
      double d = 0;
      for (k=0; k<c.X(); k++) {
	const CanvasPixel & a = c.Pixel(k, i);
	const CanvasPixel & b = c.Pixel(k, j);  
	d += a.Distance(b);	
      }
      if (minDist < 0 || d < minDist) {
	minDist = d;
	index1 = i;	
	index2 = j;
      }
    }
  }

  for (k=0; k<c.X(); k++) {
    CanvasPixel & a = c.Pixel(k, index1);
    CanvasPixel & b = c.Pixel(k, index2);
    a.R() = 0.;
    a.G() = 0.;
    a.B() = 0.;
    b.R() = 0.;
    b.G() = 0.;
    b.B() = 0.;
    }*/

  out.ToBitmap(bitmap);
  bitmap.Write(outName);
  

  return 0;
}
