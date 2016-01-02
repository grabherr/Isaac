#ifndef IMANIPULATOR_H
#define IMANIPULATOR_H



#include "physics/Physics.h"

class GamePhysObject;

//======================================================
class IManipulator
{
 public:
  IManipulator() {}
  virtual ~IManipulator() {}

  virtual void StartFeed(GamePhysObject & self) = 0;
  virtual void Interact(GamePhysObject & self, GamePhysObject & other) = 0;
  virtual void DoneFeed(GamePhysObject & self) = 0;
  
  virtual void Update(GamePhysObject & o, double deltatime) = 0; 

  virtual void CamPos(GamePhysObject & self, const Coordinates & c) = 0;


 
};

class IGlobal
{
 public:
  IGlobal() {}
  virtual ~IGlobal() {}

  virtual void StartFrame(double deltatime) = 0; 
  virtual void EndFrame(double deltatime) = 0; 
};


#endif //IMANIPULATOR_H
