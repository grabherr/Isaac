#ifndef PLANE_H
#define PLANE_H

#include "physics/Physics.h"

class FlatPlane
{
 public:
  FlatPlane() {}

  void SetCoordinates(const Coordinates & c) {
    m_plane = c;
  }

  bool Collide(PhysObject & object, double deltatime, svec<Coordinates> & touch);
  bool Collide(PhysObject & object, double deltatime);
  bool CollideSingle(PhysMinimal & p, 
		     PhysMinimal & center,
		     Coordinates & latImp,
		     Coordinates & rotImp,
		     double totalMass, 
		     double deltatime);

 private:
  Coordinates m_plane;
};



#endif 
