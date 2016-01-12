#ifndef BITMAPMANIP_H
#define BITMAPMANIP_H

#include "visual/Bitmap.h"
#include "visual/Canvas.h"

#define INFINITY_ORSO 9999999999999999999.

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

  double DoIt(svec<int> & out) {
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
    //cout << "Best for backtrace: " << index << " " << best << endl;
    for (i=m_data.isize()-1; i>=0; i--) {
      out[i] = index;
      svec<double> & l = m_scores[i];
      double min = -1.;
      int index2 = -1;
      for (k=index-2; k<=index+2; k++) {
	if (k < 0 || k >= l.isize())
	  continue;
	//cout << "k=" << k << endl;
	if (l[k] < 0)
	  continue;
	if (l[k] < min || min < 0.) {
	  index2 = k;
	  min = l[k];
	}
      }
      //cout << "New index " << index << " " << min << endl;
      index = index2;      
    }
    return best;
  }

private:
  svec < svec < double > > m_data;
  svec < svec < double > > m_scores;
  
};


//=========================================================
class BitmapMergeVert
{
 public:
  BitmapMergeVert() {
    m_lap = 200;
  }

  void SetOverlap(int l) {
    m_lap = l;
  }

  double Merge(Bitmap & outmap, 
	     const Bitmap & one, 
	     const Bitmap & two) {
    
    Canvas c1;
    c1.FromBitmap(one);
    Canvas c2;
    c2.FromBitmap(two);

    if (c1.X() != c2.X() || c1.Y() != c2.Y()) {
      cout << "ERROR: Image sizes must match!!!" << endl;
      return INFINITY_ORSO;
    }
    
    Canvas out;
    int lap = m_lap;
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
    double ret = dp.DoIt(result);
    for (k=0; k<c1.Y(); k++) {
      int pos = result[k] - lap + c1.X();
      //cout << "Found best: " << pos << endl;
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

    out.ToBitmap(outmap);
    return ret;
  }

 private:
  int m_lap;
  
};

//=========================================================
class BitmapMergeHoriz
{
 public:
  BitmapMergeHoriz() {
    m_lap = 200;
  }

  void SetOverlap(int l) {
    m_lap = l;
  }

  double Merge(Bitmap & outmap, 
	     const Bitmap & one, 
	     const Bitmap & two) {
    
    Canvas c1;
    c1.FromBitmap(one);
    Canvas c2;
    c2.FromBitmap(two);

    if (c1.X() != c2.X() || c1.Y() != c2.Y()) {
      cout << "ERROR: Image sizes must match!!!" << endl;
      return INFINITY_ORSO;
    }
    
    Canvas out;
    int lap = m_lap;
    int i, j, k;

    out.resize(c1.X(), c1.Y()*2-lap);
    DynProg dp;
    dp.Setup(c1.X(), lap);
    
    for (k=0; k<c1.X(); k++) {
      double min = -1.;
      double pos = -1;
      for (i=c1.Y()-lap; i<c1.Y(); i++) {     
	j = i-(c1.Y()-lap);
	//cout << i << " " << j << endl; 
	const CanvasPixel & a = c1.Pixel(k, i);
	const CanvasPixel & b = c2.Pixel(k, j);  
	double d = a.Distance(b);
	dp.Set(k, j, d);
	if (min < 0 || d < min) {
	  min = d;
	  pos = i;
	}
      }
    }
    svec<int> result;
    double ret = dp.DoIt(result);
    for (k=0; k<c1.X(); k++) {
      int pos = result[k] - lap + c1.Y();
      //cout << "Found best: " << pos << endl;
      for (i=0; i<pos; i++) {
	const CanvasPixel & a = c1.Pixel(k, i);
	CanvasPixel & f = out.Pixel(k, i);
	f = a;
      }
      for (i=pos-(c1.Y()-lap); i<c1.Y(); i++) {
	const CanvasPixel & a = c2.Pixel(k, i);
	CanvasPixel & f = out.Pixel(k, i+c1.Y()-lap);
	f = a;
      }
      
    }

    out.ToBitmap(outmap);
    return ret;
  }

 private:
  int m_lap;
  
};

//=========================================================
class BitmapSplitVert
{
 public:
  BitmapSplitVert() {
    m_lap = 200;
  }

  void SetOverlap(int l) {
    m_lap = l;
  }

  void Split(Bitmap & outmap, 
	     const Bitmap & in) {
    
  }

 private:
  int m_lap;
  
};

#endif
