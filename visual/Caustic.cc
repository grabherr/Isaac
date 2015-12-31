#include "visual/Caustic.h"
#include "physics/Coordinates.h"



void Caustic::ComputeBottom(Canvas & out, const Canvas & in, double depth, int smooth)
{
  Canvas tmp = in;
  //cout << "Compute normals." << endl;
  int i, j;
  for (i=0; i<smooth; i++)
    tmp.Smooth(1.0);

  tmp.ComputeNormalsFromV();

  out.resize(in.X(), in.Y());
  out.PaintAll(0., 0., 0.);

  //cout << "Compute bottom." << endl;
  for (i=0; i<tmp.X(); i++) {
    for (j=0; j<tmp.Y(); j++) {

      //if (i > 240 && i < 270 && j > 110 && j < 130)
      //continue;
      int ox = i - m_overlaycoords.X();
      int oy = j - m_overlaycoords.Y();
      if (ox >= 0 && oy >= 0 && ox < m_overlay.X() && oy < m_overlay.Y()) {
	const RGBPixel & p = m_overlay.Get(ox, oy);
	if (p.R() < 255 && p.G() < 255 && p.B() < 255)
	  continue;
      }
      

      CanvasPixel &  p = tmp.Pixel(i, j);
      Coordinates c;
      c[0] = p.R();
      c[1] = p.G();
      c[2] = p.B();

      SphereCoordinates s = c.AsSphere();
      double theta = s.theta();
      //double fac = 1.-1/m_rho;
      double fac = 1/m_rho;
      //cout << "Before theta: " << theta << " fac " << fac << endl; 
      if (theta < PI_P/2. && theta > -PI_P/2.) {
	theta *= fac; 
      } else {
	theta = PI_P + (theta - PI_P)*fac;
      }
      //cout << "After theta: " << theta << endl; 
      s.SetTheta(theta);

      Coordinates t;
      t.FromSphere(s);
      t[0] = -t[0];
      t[1] = -t[1];
      t[2] = -t[2];
 
      double mul = depth;

      double ct = cos(theta);
      if (ct != 0.)
	mul /= ct;
      t *= mul;

      int x = (int)(i+t[0]+0.5);
      int y = (int)(j+t[1]+0.5);
      
      if (x >= 0 && y >= 0 && x < out.X() && y < out.Y()) {
	CanvasPixel &  l = out.Pixel(x, y);
	l.V() += 0.5;
      }      
    }
  }
  out.Smooth(1.);
  out.Smooth(1.);
  out.Smooth(1.);
  out.Smooth(1.);
}


void Caustic::ComputeBottom(Bitmap & out, const Bitmap & in, double depth, int smooth)
{
  Canvas canv_in;
  Canvas canv_out;
  
  canv_in.FromBitmap(in);

  int i, j;
  
  for (i=0; i<canv_in.X(); i++) {
    for (j=0; j<canv_in.Y(); j++) {
      CanvasPixel &  p = canv_in.Pixel(i, j);
      p.V() = p.R();
      p.R() = 0.;
      p.G() = 0.;
      p.B() = 0.;
    }
  }
  ComputeBottom(canv_out, canv_in, depth, smooth);

  
  for (i=0; i<canv_out.X(); i++) {
    for (j=0; j<canv_out.Y(); j++) {
      CanvasPixel &  p = canv_out.Pixel(i, j);      
      p.R() = p.V();
      p.G() = p.V();
      p.B() = p.V();
    }
  }
  canv_out.ToBitmap(out);

}

//================================================================================

void Caustic::ComputeTop(Canvas & out, const Canvas & in, const Canvas & temp, double depth, int smooth)
{
  Canvas tmp = in;
  //cout << "Compute normals." << endl;
  int i, j;
  for (i=0; i<smooth; i++)
    tmp.Smooth(1.0);

  tmp.ComputeNormalsFromV();

  out.resize(in.X(), in.Y());
  out.PaintAll(0., 0., 0.);

  //cout << "Compute bottom." << endl;
  for (i=0; i<tmp.X(); i++) {
    for (j=0; j<tmp.Y(); j++) {

      int ox = i - m_overlaycoords.X();
      int oy = j - m_overlaycoords.Y();
      if (ox >= 0 && oy >= 0 && ox < m_overlay.X() && oy < m_overlay.Y()) {
	const RGBPixel & p = m_overlay.Get(ox, oy);
	if (p.R() < 255 && p.G() < 255 && p.B() < 255)
	  continue;
      }
      

      CanvasPixel &  p = tmp.Pixel(i, j);
      Coordinates c;
      c[0] = p.R();
      c[1] = p.G();
      c[2] = p.B();

      SphereCoordinates s = c.AsSphere();
      double theta = s.theta();
      //double fac = 1.-1/m_rho;
      double fac = 1/m_rho;
      //cout << "Before theta: " << theta << " fac " << fac << endl; 
      if (theta < PI_P/2. && theta > -PI_P/2.) {
	theta *= fac; 
      } else {
	theta = PI_P + (theta - PI_P)*fac;
      }
      //cout << "After theta: " << theta << endl; 
      s.SetTheta(theta);

      Coordinates t;
      t.FromSphere(s);
      t[0] = -t[0];
      t[1] = -t[1];
      t[2] = -t[2];
 
      double mul = depth;

      double ct = cos(theta);
      if (ct != 0.)
	mul /= ct;
      t *= mul;

      int x = (int)(i+t[0]+0.5);
      int y = (int)(j+t[1]+0.5);

      CanvasPixel & toRender = out.Pixel(i, j);
     
      //x = i;
      //y = j;
      if (x >= 0 && y >= 0 && x < out.X() && y < out.Y()) {
	const CanvasPixel & l = temp.Pixel(x, y);
	toRender = l;
	//cout << "Add " << l.R() << " " << l.G() << " " << l.B() << endl;
	//cout << "    " << toRender.R() << " " << toRender.G() << " " << toRender.B() << endl;
      }      
    }
  }
}



void Caustic::ComputeTop(Bitmap & out, const Bitmap & in, const Bitmap & temp, double depth, int smooth)
{
  int i, j;
  Canvas canv_in;
  Canvas canv_out;
  Canvas bottom;

  canv_in.FromBitmap(in);

  bottom.FromBitmap(temp); 
 
  for (i=0; i<canv_in.X(); i++) {
    for (j=0; j<canv_in.Y(); j++) {
      CanvasPixel &  p = canv_in.Pixel(i, j);
      p.V() = p.R();
      p.R() = 0.;
      p.G() = 0.;
      p.B() = 0.;

      CanvasPixel &  pp = bottom.Pixel(i, j);
      pp.V() = 0.;
    }
  }
  ComputeTop(canv_out, canv_in, bottom, depth, smooth);

  /*
  for (i=0; i<canv_out.X(); i++) {
    for (j=0; j<canv_out.Y(); j++) {
      CanvasPixel &  p = canv_out.Pixel(i, j);      
      p.R() = p.V();
      p.G() = p.V();
      p.B() = p.V();
    }
    }*/
  canv_out.ToBitmap(out);

}
