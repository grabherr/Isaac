#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "engine/IManipulator.h"


class MoveManipulator : public IManipulator
{
 public:
  MoveManipulator() {
    m_speed = 0.;
    m_maxSpeed = -1.;
    m_targetSize = -1.;
    m_moveTime = -1.;
    m_sizeTime = -1.;
    m_acc = 0.;
  }
  virtual ~MoveManipulator() {}

  virtual void StartFeed(GamePhysObject & self);
  virtual void Feed(GamePhysObject & self, GamePhysObject & other);
  virtual void DoneFeed(GamePhysObject & self);
  
  virtual void Update(GamePhysObject & o, double deltatime); 

  virtual void Interact(GamePhysObject & self, GamePhysObject & other);
  virtual void CamPos(GamePhysObject & self, const Coordinates & c);

  void MoveTo(const Coordinates & c, double time, double maxSpeed = -1.);
  
  void SetRotation(const Coordinates & c);

  void SetRotationSpeed(const Coordinates & c);

  void GrowShrink(double relSize, double time);
  
 protected:
  Coordinates m_target;
  double m_speed;
  double m_acc;
  double m_moveTime;
  double m_maxSpeed;
  Coordinates m_rotation;
  Coordinates m_rotSpeed;
  double m_targetSize;  
  double m_sizeTime;

 
};





#endif //MANIPULATOR_H

