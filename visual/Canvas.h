#ifndef CANVAS_H
#define CANVAS_H

#include "base/SVector.h"
#include "visual/Bitmap.h"
#include "visual/Color.h"

class CanvasPixel
{
 public:
  CanvasPixel() {
    m_r = m_g = m_b = 0.;
    m_val = 0.;
  }

  void Set(double r, double g, double b, double v = 0.) {
    m_r = r;
    m_b = b;
    m_g = g;
    m_val = v;
  }

  const double & R() const {return m_r;}
  const double & G() const {return m_g;}
  const double & B() const {return m_b;}
  const double & V() const {return m_val;}
 
  double & R() {return m_r;}
  double & G() {return m_g;}
  double & B() {return m_b;}
  double & V() {return m_val;}

  void GreyScale() {
    m_r = m_val;
    m_g = m_val;
    m_b = m_val;
  }

  void ColorScale(const color & neg, 
		  const color & pos, 
		  const color & middle = color(0.999, 0.999, 0.999)) {
    color c = GradientMult(m_val, neg, pos, middle);
    m_r = c.R();
    m_g = c.G();
    m_b = c.B();
  }

  // Operators
  void operator += (double v) {
    m_val += v;
  }
  void operator -= (double v) {
    m_val -= v;
  }
  void operator += (const CanvasPixel & p) {
    m_val += p.m_val;
    m_r += p.m_r;
    m_g += p.m_g;
    m_b += p.m_b;
  }
  void operator -= (const CanvasPixel & p) {
    m_val -= p.m_val;
    m_r -= p.m_r;
    m_g -= p.m_g;
    m_b -= p.m_b;
  }
  
 private:
  double m_r;
  double m_g;
  double m_b;
  double m_val;
};


class Canvas
{
 public:
  Canvas() {    
  }

  Canvas(int x, int y) {    
  }
 
  void resize(int x, int y) {
    int i;
    m_pixels.resize(x);
    for (i=0; i<x; i++)
      m_pixels[i].resize(y);
  }

  int X() const {return m_pixels.isize();}

  int Y() const {
    if (m_pixels.isize() == 0)
      return 0;
    return m_pixels[0].isize();
  }

  CanvasPixel & Pixel(int i, int j) {
    return (m_pixels[i])[j];
  }
  const CanvasPixel & Pixel(int i, int j) const {
    return (m_pixels[i])[j];
  }

  void ComputeNormalsFromV(double gridsize = 1.);

  void FromBitmap(const Bitmap & b);
  void ToBitmap(Bitmap & b);

  void PaintAll(double r, double g, double b, double v = 0.) {
    for (int i=0; i<m_pixels.isize(); i++) {
      for (int j=0; j<m_pixels[i].isize(); j++) {
	(m_pixels[i])[j].Set(r, g, b, v);
      }
    }
  }

  void PaintAll(color c, double v = 0.) {
    PaintAll(c.R(), c.G(), c.B(), v);
  }

  void Set(double x,
	   double y,
	   double r, 
	   double g, 
	   double b, 
	   double v = 0.);
  
  void Set(double x, double y, color c, double v = 0.) {
    Set(x, y, c.R(), c.G(), c.B(), v);
  }

  void GreyScale() {
    for (int i=0; i<m_pixels.isize(); i++) {
      for (int j=0; j<m_pixels[i].isize(); j++) {
	(m_pixels[i])[j].GreyScale();
      }
    }
  }
  
  void ColorScale(const color & neg, 
		  const color & pos, 
		  const color & middle = color(0.999, 0.999, 0.999)) {
    for (int i=0; i<m_pixels.isize(); i++) {
      for (int j=0; j<m_pixels[i].isize(); j++) {
	(m_pixels[i])[j].ColorScale(neg, pos, middle);
      }
    }
  }


  void Smooth(double weight = 0.5);

 private:
  svec < svec < CanvasPixel > > m_pixels;
};


#endif //CANVAS_H
