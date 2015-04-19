#include "world/ExternalWorld.h"


void ExternalWorld::Print()
{

}

int ExternalWorld::Register(IAutonomosUnit * pUnit, const Coordinates & pos)
{
  m_reg.push_back(pUnit);
  pUnit->SetHandle(m_reg.isize()-1);
  return m_reg.isize()-1;
}

void ExternalWorld::GetEntities(svec<Entity> & ent, const Coordinates & pos)
{
  ent = m_ent;
}


bool ExternalWorld::Try(Reaction & outcome, Coordinates & pos, Entity & ent, int handle, const IAction & action)
{

  outcome.Reset();
  outcome.SetPain(m_pain);
  outcome.SetReward(m_reward);
  outcome.SetUnsuccess(1. - m_collision);

  return true;
}
