#ifndef PHYSICSIO_H
#define PHYSICSIO_H

#include "physics/Physics.h"

class PhysicsIO
{
 public:
  PhysicsIO() {}

  void SetCoordsOffset(const Coordinates & c);

  void Read(PhysObject & p, const string & fileName);
  void Write(const PhysObject & p, const string & fileName);

 private:
  Coordinates m_offset;
};

#endif 


