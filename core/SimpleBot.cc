#include "core/SimpleBot.h"
#include "base/RandomStuff.h"

void SimpleBot::Feed(const svec<Entity> & ent)
{
  int i;
  for (i=0; i<ent.isize(); i++) {
    const Entity & e = ent[i];
    if (e[0] > 0.5) {
      m_target = e.GetCoordinates();
    }
  }
}

void SimpleBot::GetAction(IAction & a) 
{
  if (m_bRand) {

    double x = RandomFloat(2.) - 1.;
    double y = RandomFloat(2.) - 1.;
    a.Set(0, x);
    a.Set(1, y);    
    return;
  } 

  a.Set(0, m_target.X());
  a.Set(1, m_target.Y());
  
  cout << "Set " << a.Get(0) << " " << a.Get(1) << endl;
}

void SimpleBot::Feedback(const Reaction & outcome, const Entity & e, const Coordinates & pos)
{
  if (outcome.Reward() > 0.5) {
    cout << "YUMM." << endl;    
  }
  if (outcome.Pain() > 0.7) {
    cout << "AAAAARGGGH" << endl;
  } else {
    if (outcome.Pain() > 0.2) {
      cout << "OUCH." << endl;
    }
  }
  if (pos == m_mypos) {
    cout << "Random." << endl;
    m_bRand = true;
  } else {
    m_bRand = false;
  }
  m_mypos = pos;
}
