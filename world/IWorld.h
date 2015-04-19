#ifndef IWORLD_H
#define IWORLD_H

#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "core/IAutUnit.h"
#include "creature/Isaac.h"

// World abstract interface
class IWorld
{
 public:
  IWorld() {}
  virtual ~IWorld() {}

  virtual int Register(IAutonomosUnit * pUnit, const Coordinates & pos) = 0;
  //  virtual int Register(Isaac * pUnit, const Coordinates & pos) = 0;

  virtual bool Try(Reaction & outcome, Coordinates & pos, Entity & ent, int handle, const IAction & action) = 0;
  virtual void GetEntities(svec<Entity> & ent, const Coordinates & pos) = 0;

  
};

#endif

