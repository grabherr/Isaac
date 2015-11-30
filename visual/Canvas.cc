#include "visual/Canvas.h"
#include "physics/Coordinates.h"
#include <math.h>

void Canvas::ComputeNormalsFromV(double gridsize)
{
  int i, j;
  
  for (i=0; i<m_pixels.isize(); i++) {
    for (j=0; j<m_pixels[i].isize(); j++) {

      // Last pixels...
      if (i == m_pixels.isize() - 1) {
	Pixel(i, j) = Pixel(i-1, j);
	continue;
      }

      if (j == m_pixels[i].isize() - 1) {
	Pixel(i, j) = Pixel(i, j-1);
	continue;
      }
      

      Coordinates a, b, c;
      a[0] = gridsize*(double)i;
      a[1] = gridsize*(double)j;
      a[2] = Pixel(i, j).V();
      
      b[0] = gridsize*(double)i+1;
      b[1] = gridsize*(double)j;
      b[2] = Pixel(i+1, j).V();
      
      c[0] = gridsize*(double)i;
      c[1] = gridsize*(double)j+1;
      c[2] = Pixel(i, j+1).V();

      Coordinates d1 = b-a;
      Coordinates d2 = c-a;
      
      Coordinates n = d1.CrossProduct(d2);
      // Check order!!!!
      Pixel(i, j).Set(n[0], n[1], n[2]);
    }
  }

}

void Canvas::FromBitmap(const Bitmap & b)
{
  resize(b.X(), b.Y());
  int i, j;
  
  for (i=0; i<m_pixels.isize(); i++) {
    for (j=0; j<m_pixels[i].isize(); j++) {
      const RGBPixel & p = b.Get(i, j);
      Pixel(i, j).Set(p.r(), p.b(), p.g());
    }
  }

}

double LimitColor(double d)
{
  if (d < 0.)
    d = 0.;
  if (d > 0.999)
    d = 0.999;
  return d;	       
}

void Canvas::ToBitmap(Bitmap & b)
{
  b.SetSize(X(), Y());
  int i, j;
 
  for (i=0; i<m_pixels.isize(); i++) {
    for (j=0; j<m_pixels[i].isize(); j++) {
      RGBPixel & p = b.Get(i, j);
      p.Set_r(LimitColor(Pixel(i, j).R()));
      p.Set_g(LimitColor(Pixel(i, j).G()));
      p.Set_b(LimitColor(Pixel(i, j).B()));
    }
  }
}

double GetAbs(double d) {
  if (d < 0)
    return -d;
  return d;
}

void Canvas::Set(double x, double y, double r, double g, double b, double v)
{
  int i, j;

  int xx = (int)x;
  int yy = (int)y;
  
  double rx = x - xx;
  double ry = y - yy;
  
  for (i=xx-1; i<=xx+1; i++) {
    if (i < 0 || i >= X())
      continue;
    for (j=yy-1; j<=yy+1; j++) {
      if (j < 0 || j >= Y())
	continue;
      
      double a = x - i;
      double b = y - j;
      
      if (GetAbs(a) >= 1. || GetAbs(b) >= 1.)
	continue;
     
      double weight = 1. - GetAbs(a) - GetAbs(b);
      (m_pixels[i])[j].R() = weight*r;
      (m_pixels[i])[j].G() = weight*g;
      (m_pixels[i])[j].B() = weight*b;
      (m_pixels[i])[j].V() = weight*v;
    }
  }

}
