// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

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


