#include "visual/WavePainter.h"

void OneWave::DrawNormalsSharp(Bitmap & bitmap, double deltatime)
{
  m_time += deltatime;
  double radius = m_v * m_time;
  DrawOne(bitmap, radius, 0.49);
  radius = m_v * (m_time - m_b);
  if (radius > 0.) 
    DrawOne(bitmap, radius, -0.49);
  
}

void OneWave::DrawNormalsSmooth(Bitmap & bitmap, double deltatime)
{
  m_time += deltatime;
  double radius1 = m_v * (m_time - m_b);
  double radius2 = m_v * m_time;

  double range = 2*(radius2 - radius1);
  double start = radius1 -  m_b;
  double end = radius2 +  m_b;
  if (start < 0)
    start = 0.;

  WaveForm waveForm;
  int i = 0;

  
  double amp = 0.5/(0.05*radius1 + 1);
  for (double r = start; r<=end; r += 1.) {
    //double a = sin((r-start)*PI_W/range);
    double a = waveForm[i];
    i++;
    cout << "r=" << r << " a=" << a << endl;
    DrawOne(bitmap, r, amp*a);
  }
 
  
}

double LimitRange(double v, double a)
{
  v += a;
  if (v < 0.)
    v = 0.;
  if (v >= 1.)
    v = 1.;
  return v;
}

void OneWave::DrawOne(Bitmap & bitmap, double radius, double amp)
{
  
  double g = 2 * PI_W * radius;
  double delta = 1./g;
  double phi;
  
  int lastI = -1;
  int lastJ = -1;

  for (phi=0.; phi<2 * PI_W; phi += delta) {
    double x = m_center.X() + radius*sin(phi);
    double y = m_center.Y() + radius*cos(phi);
    int i = (int)(x+0.5);
    int j = (int)(y+0.5);
    
    if (i == lastI && j == lastJ) 
      continue;

    lastI = i;
    lastJ = j;

    if (i < 0) {
      i = -i;
    } 
    if (j < 0) {
      j = -j;
    } 

    if (i >= bitmap.X()) {
      i -= bitmap.X();
      i = bitmap.X() - i;
    }

    if (j >= bitmap.Y()) {
      j -= bitmap.Y();
      j = bitmap.Y() - j;
    }
 
    if (i < 0 || j < 0 || i >=  bitmap.X() || j >= bitmap.Y())
      continue;

    
    RGBPixel & c = bitmap.Get(i, j);
    double v = LimitRange(c.r(), amp);   
    //cout << v << endl;
    /*color col = GradientMult(v, color(0., 0., 0.), color(0.99, 0.99, 0.99), color(0.2, 0.2, 0.9));
    c.Set_r(col.R());
    c.Set_g(col.G());
    c.Set_b(col.B());*/
   
    c.Set_r(v);
    v = LimitRange(c.g(), amp);       
    c.Set_g(v);
    v = LimitRange(c.b(), amp);       
    c.Set_b(v);
   
  }

}

