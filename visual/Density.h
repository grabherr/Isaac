#ifndef DENSITY_H
#define DENSITY_H

#include "visual/Whiteboard.h"
#include "visual/Color.h"
#include <math.h>

class Density
{
 public:
  Density(int nX, int nY, 
	  double saturate = -1., // Uses max value if not set
	  const color & low = color(0.99, 0.99, 0.99),
	  const color & mid = color(0.99, 0.99, 0),
	  const color & hi = color(0.99, 0., 0.)) {
    m_lo = low;
    m_mi = mid;
    m_hi = hi;
    m_saturate = saturate;
    m_dens.resize(nX);
    for (int i=0; i<m_dens.isize(); i++)
      m_dens[i].resize(nY, 0.);
    m_max = 0.;
  }

  void Add(double x, double y, double weight = 1., bool bNormalize = false) {
    int x1 = (int)(x  + 0.5);
    int y1 = (int)(y  + 0.5);
    if (bNormalize) {
      x1 = (int)(x / (double)m_dens.isize() + 0.5);
      y1 = (int)(y / (double)m_dens[0].isize() + 0.5);
    }
    if (x1 < 0 || x1 > m_dens.isize()) {
      cout << "ERROR: x is out of bounds: " << x << endl; 
      return;
    }
    if (y1 < 0 || y1 > m_dens[0].isize()) {
      cout << "ERROR: y is out of bounds: " << y << endl; 
      return;
    }
      
    svec<double> & d = m_dens[x1];
    d[y1] += weight;
    if (d[y1] > m_max)
      m_max = d[y1];
  }

  void Draw(ns_whiteboard::whiteboard & board,
	    const ns_whiteboard::xy_coords & where, 
	    double pointsPerPixel) {
    int i, j;
    for (i=0; i<m_dens.isize(); i++) {      
      svec<double> & d = m_dens[i];
      for (j=0; j<d.isize(); j++) {
	double v = d[j];
	double x1 = where.first + pointsPerPixel*(double)i;
	double y1 = where.second + pointsPerPixel*(double)(j+1);
	double x2 = where.first + pointsPerPixel*(double)(i+1);
	double y2 = where.second + pointsPerPixel*(double)j;
	if (m_saturate > 0.) {
	  v /= m_saturate;
	} else {
	  v /= m_max;
	}
	if (v > 0.999)
	  v = 0.999;
	v = v*2 - 1.;
	color cc = GradientMult(v, m_lo, m_hi, m_mi);
	board.Add( new ns_whiteboard::rect( ns_whiteboard::xy_coords(x1, y1), 
                                            ns_whiteboard::xy_coords(x2, y2),
                                            cc) );

      }
    }   
   
 
  }

 private:
  svec< svec < double > > m_dens;
  double m_saturate;
  double m_max;
  color m_lo;
  color m_mi;
  color m_hi;
};





#endif


