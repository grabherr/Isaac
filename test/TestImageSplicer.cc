#define FORCE_DEBUG

#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "visual/Bitmap.h"
#include "visual/Canvas.h"

#define INFINITY_ORSO 9999999999999999.

class DynProg
{
public:
  DynProg() {}

  void Setup(int x, int y) {
    int i;
    m_data.resize(x);
    m_scores.resize(x);
    for (i=0; i<x; i++) {
      m_data[i].resize(y, 0.);
      m_scores[i].resize(y, INFINITY_ORSO);
    }
  }

  void Set(int x, int y, double d) {
    (m_data[x])[y] = d;
  }

  void DoIt(svec<int> & out) {
    int i, j, k;
   
    out.resize(m_data.isize(), -1);
    m_scores[0] = m_data[0];
   
    for (i=1; i<m_data.isize(); i++) {
      svec<double> & d = m_data[i];
      svec<double> & s = m_scores[i];
      svec<double> & l = m_scores[i-1];
      
      for (j=2; j<d.isize()-2; j++) {
	for (k=j-2; k<=j+2; k++) {
	  if (l[k] < s[j] || s[j] < 0.)
	    s[j] = l[k];
	}
	s[j] += d[j];	
      }
    }
    svec<double> & last = m_scores[m_scores.isize()-1];
    int index = -1;
    double best = INFINITY_ORSO;
    for (i=2; i<last.isize()-2; i++) {
      if (best < 0. || last[i] < best) {
	if (last[i] >= 0.) {
	  best = last[i];
	  index = i;
	}
      }
    }
    cout << "Best for backtrace: " << index << " " << best << endl;
    for (i=m_data.isize()-1; i>=0; i--) {
      out[i] = index;
      svec<double> & l = m_scores[i];
      double min = -1.;
      int index2 = -1;
      for (k=index-2; k<=index+2; k++) {
	if (k < 0 || k >= l.isize())
	  continue;
	cout << "k=" << k << endl;
	if (l[k] < 0)
	  continue;
	if (l[k] < min || min < 0.) {
	  index2 = k;
	  min = l[k];
	}
      }
      cout << "New index " << index << " " << min << endl;
      index = index2;      
    }
  }

private:
  svec < svec < double > > m_data;
  svec < svec < double > > m_scores;
  
};


int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","input file");
  commandArg<string> oCmmd("-o","output file");
  commandLineParser P(argc,argv);
  P.SetDescription("Testing the file parser.");
  P.registerArg(fileCmmd);
  P.registerArg(oCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string outName = P.GetStringValueFor(oCmmd);
 

  Bitmap bitmap;
  bitmap.Read(fileName);

  Canvas c;
  c.FromBitmap(bitmap);

  Canvas out;
  int lap = 200;
  int i, j, k;

  out.resize(c.X()*2-lap, c.Y());
  DynProg dp;
  dp.Setup(c.Y(), lap);

  for (k=0; k<c.Y(); k++) {
    double min = -1.;
    double pos = -1;
    for (i=c.X()-lap; i<c.X(); i++) {     
      j = i-(c.X()-lap);
      //cout << i << " " << j << endl; 
      const CanvasPixel & a = c.Pixel(i, k);
      const CanvasPixel & b = c.Pixel(j, k);  
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
  for (k=0; k<c.Y(); k++) {
    int pos = result[k] - lap + c.X();
    cout << "Found best: " << pos << endl;
    for (i=0; i<pos; i++) {
      const CanvasPixel & a = c.Pixel(i, k);
      CanvasPixel & f = out.Pixel(i, k);
      f = a;
    }
    for (i=pos-(c.X()-lap); i<c.X(); i++) {
      const CanvasPixel & a = c.Pixel(i, k);
      CanvasPixel & f = out.Pixel(i+c.X()-lap, k);
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
