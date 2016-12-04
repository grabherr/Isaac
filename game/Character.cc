#include "game/Character.h"
#include "base/RandomStuff.h"

Character::Character()
{
  m_strength = RandomFloat(1.);
  m_attraction = RandomFloat(1.);
  m_index = -1;
  m_gender = RandomFloat(2.)-1;
  m_target = -1;
  m_attraction_ext = 0;
  m_strength_ext = 0;
  m_action = 0.;
}


void Character::Act(svec<Character> & character)
{
  
}

void Character::Suffer(double action, const Character & c)
{
}
