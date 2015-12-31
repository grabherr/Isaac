#ifndef CAUSTIC_H
#define CAUSTIC_H

#include "visual/Canvas.h"

class Caustic
{
 public:
  Caustic() {
    m_rho = 1.33;
  }

  void SetRefractionIndex(double v) {
    m_rho = v;
  }

  void ComputeBottom(Canvas & out, const Canvas & in, double depth, int smooth = 4);
  void ComputeBottom(Bitmap & out, const Bitmap & in, double depth, int smooth = 4);

  void ComputeTop(Canvas & out, const Canvas & in, const Canvas & temp, double depth, int smooth = 1);
  void ComputeTop(Bitmap & out, const Bitmap & in, const Bitmap & temp, double depth, int smooth = 1);
 
  void SetOverlay(const Bitmap & overlay, const Coords2D & coords) {
    m_overlay = overlay;
    m_overlaycoords = coords;
  }


 private:
  double m_rho;

  Bitmap m_overlay;
  Coords2D m_overlaycoords;

};

#endif //CAUSTIC_H
