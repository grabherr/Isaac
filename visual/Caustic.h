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
 
 private:
  double m_rho;

};

#endif //CAUSTIC_H
