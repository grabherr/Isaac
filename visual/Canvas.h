#ifndef CANVAS_H
#define CANVAS_H

#include "base/SVector.h"
#include "visual/Bitmap.h"
#include "visual/Color.h"
#include <math.h>


class CMReadFileStream;
class CMWriteFileStream;

class Coords2D
{
public:
  Coords2D() {
    m_x = 0.;
    m_y = 0.;
  }
  Coords2D(double x, double y) {
    m_x = x;
    m_y = y;
  }

  const double & X() const {return m_x;}
  const double & Y() const {return m_y;}
  double & X() {return m_x;}
  double & Y() {return m_y;}

  double Length() const {
    return sqrt(m_x*m_x + m_y*m_y);
  }
  
  void operator -= (const Coords2D & d) {
    m_x -= d.m_x;
    m_y -= d.m_y;
  }
  void operator += (const Coords2D & d) {
    m_x += d.m_x;
    m_y += d.m_y;
  }

  Coords2D operator - (const Coords2D & d) const {
    Coords2D tmp = *this;
    tmp -= d;
    return tmp;
  }

  Coords2D operator + (const Coords2D & d) const {
    Coords2D tmp = *this;
    tmp += d;
    return tmp;
  }


protected:
  double m_x;
  double m_y;
};


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


//==================================================
//==================================================
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

  // White == invisible
  void Overlay(Bitmap & b, int xoff = 0, int yoff = 0);

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

  void Read(CMReadFileStream & f);
  void Write(CMWriteFileStream & f) const;


 private:
  svec < svec < CanvasPixel > > m_pixels;
};

//==================================================
//==================================================
class SparseCanvas
{
 public:
  SparseCanvas() { 
    m_x = 0;
    m_y = 0;
  }

  int X() const {return m_x;}
  int Y() const {return m_y;}

  // Everything within the specified interval is ignored
  void FromCanvas(const Canvas & c, double min, double max);

  // Note that the canvas needs to be properly sized
  void AddToCanvas(Canvas & c, int off_x, int off_y, double offset = 0., bool bWrap = false) const;


  void Read(CMReadFileStream & f);
  void Write(CMWriteFileStream & f) const;

 protected:

  svec <CanvasPixel> m_pixels;
  int m_x;
  int m_y;
  svec<int> m_i;
  svec<int> m_j;
  

};


#endif //CANVAS_H
