#include "engine/Manipulator.h"
#include "engine/GameEngine.h"

void MoveManipulator::StartFeed(GamePhysObject & self)
{
}

void MoveManipulator::Feed(GamePhysObject & self, GamePhysObject & other)
{
}

void MoveManipulator::DoneFeed(GamePhysObject & self)
{
}
  
void MoveManipulator::Update(GamePhysObject & o, double deltatime)
{
  PhysObject & p = o.GetPhysObject();
  double mass = p.GetTotalMass();
  PhysMinimal & m = p.GetCenterDirect();
  
  Coordinates dir = p.GetDirection();
  Coordinates pos = p.GetCenterDirect().GetPosition();

  if (m_moveTime > 0.) {
    
  }

  p.GetCenterDirect().SetPosition(pos);

  m_rotation += m_rotSpeed * deltatime;
  p.SetEngRotation(m_rotation);


}

void MoveManipulator::Interact(GamePhysObject & self, GamePhysObject & other)
{
}

void MoveManipulator::CamPos(GamePhysObject & self, const Coordinates & c)
{
}

void MoveManipulator::MoveTo(const Coordinates & c, double moveTime, double maxSpeed)
{
  m_target = c;
  m_maxSpeed = maxSpeed;
  m_moveTime = moveTime;
}
  
void MoveManipulator::SetRotation(const Coordinates & c)
{
  m_rotation = c;
}

void MoveManipulator::SetRotationSpeed(const Coordinates & c)
{
  m_rotSpeed = c;
}

void MoveManipulator::GrowShrink(double relSize, double time)
{
  m_targetSize = relSize;
  m_sizeTime = time;
}
 
