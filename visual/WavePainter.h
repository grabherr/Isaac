#ifndef WAVEPAINTER_H
#define WAVEPAINTER_H

#include "visual/Canvas.h"
#include <math.h>

const double PI_W = 3.1415926535897932384626433832795;


class WaveForm
{
public:
  WaveForm() {
    m_val.resize(24, 0.);
    int i;
    double r1 = 0.2;
    double r2 = 0.13;
    for (i=0; i<m_val.isize(); i++) {
      double x = ((double)i-0.3*(double)m_val.isize());
      double v = -1./(1+r1*x*x);
      cout << i << " " << x << " " << v << endl;
      m_val[i] += v;
    }
    
    for (i=0; i<m_val.isize(); i++) {
      double x = ((double)i-0.7*(double)m_val.isize());
      double v = +1./(1+r2*x*x);
      m_val[i] += v;
      cout << i << " " << x << " " << v << " " << m_val[i] << endl;
    }
    
    /*
    for (i=1; i<m_val.isize()-1; i++) {
      m_val[i] = 0.6*m_val[i] + 0.2*(m_val[i-1] + m_val[i+1]);
      cout << i << " -> " << m_val[i] << endl;
      }*/
  }

  double & operator[] (int i) {return m_val[i];}
  const double & operator[] (int i) const {return m_val[i];}

private:
  svec<double> m_val;
};


class D2Coords
{
public:
  D2Coords() {
    m_x = 0.;
    m_y = 0.;
  }

  const double & X() const {return m_x;}
  const double & Y() const {return m_y;}
  double & X() {return m_x;}
  double & Y() {return m_y;}

  double Length() const {
    return sqrt(m_x*m_x + m_y*m_y);
  }
  
protected:
  double m_x;
  double m_y;
};

//===============================
class OneWave
{
public:
  OneWave() {
    m_time = 0.;
    m_v = 1.;
    m_b = 8.;
  }

  const D2Coords & Center() const {return m_center;}
  D2Coords & Center() {return m_center;}

  void DrawNormalsSharp(Bitmap & b, double deltatime);
  void DrawNormalsSmooth(Bitmap & b, double deltatime);
  
  const double & Time() const {return m_time;}
  double & Time() {return m_time;}
protected:
  void DrawOne(Bitmap & b, double radius, double amp);
  D2Coords m_center;
  double m_time;
  double m_v;
  double m_b;
};



#endif //WAVEPAINTER_H
