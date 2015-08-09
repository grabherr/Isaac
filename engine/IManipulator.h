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

  virtual void StartFeed() = 0;
  virtual void Feed(GamePhysObject & other) = 0;
  virtual void DoneFeed() = 0;
  
  virtual void Update(GamePhysObject & o, double deltatime) = 0; 

  virtual void Interact(GamePhysObject & other) = 0;


 
};



#endif //IMANIPULATOR_H
