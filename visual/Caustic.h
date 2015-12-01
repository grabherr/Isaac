#ifndef CAUSTIC_H
#define CAUSTIC_H

#include "visual/Canvas.h"

class Caustic
{
 public:
  Caustic() {}

  void ComputeBottom(Canvas & out, const Canvas & in, double depth);
  void ComputeBottom(Bitmap & out, const Bitmap & in, double depth);
 

};

#endif //CAUSTIC_H
