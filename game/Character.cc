#include "game/Character.h"
#include "base/RandomStuff.h"


Character::Character()
{
  m_size = 0;
  m_myIndex = -1;
  m_desire = -1;
  m_avoid = -1;
  m_act = 0;
}


void Character::SetupPeople(int n, int index)
{
  // Reserve 2 for in and out action, and 1 for success
  m_nn.Setup(20, n+3, 1);      
  m_nn.ReSetup(-1, 1);
  m_size = n+3;
  m_myIndex = index;
}

void Character::FeedNeutral(const svec<double> & prop, const Coordinates & c, int index)
{
  if (index == m_myIndex)
    return;
  
  int i;
  double w = (m_self - c).Length();
  if (w < 10)
    w = 10;
  w = 10/w;

  NPCIO seq;
  seq.resize(m_size);
  seq.SetAllValid(false);
  for (i=0; i<prop.isize(); i++) {
    seq[i] = prop[i];
    seq.SetValid(i, true);
  }
  m_nn.Learn(seq, w);
  
}

void Character::FeedDone(const svec<double> & prop, double in)
{
}

void Character::SetScore(double s)
{
}

void Character::FeedAction(const svec<double> & prop, double in)
{
  int i;
  
  NPCIO seq;
  seq.resize(m_size);
  seq.SetAllValid(false);
  for (i=0; i<prop.isize(); i++) {
    seq[i] = prop[i];
    seq.SetValid(i, true);
  }
  seq[i] = in;
  seq.SetValid(i, true);
  m_nn.Learn(seq, 1.);    
}
