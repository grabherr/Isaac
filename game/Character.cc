#include "game/Character.h"
#include "base/RandomStuff.h"


Character::Character()
{
  m_size = 0;
  m_myIndex = -1;
  m_desire = -1;
  m_avoid = -1;
  m_act = 0;
  m_score = 0;
}

void Character::Print() const
{
  cout << "Printing contents for " << m_name << endl;
  cout << "Desired:  " << m_desire << endl;
  cout << "Action:   " << m_act << endl;
  cout << "Avoiding: " << m_avoid << endl;
  cout << "Coordinates ";
  m_self.Print();
  cout << endl;
  m_nn.Print();
}

void Character::SetupPeople(int n, int index)
{
  // Reserve 2 for in and out action, and 1 for success
  m_nn.Setup(20, n+3, 1);      
  m_nn.ReSetup(-1, 1);
  m_size = n+3;
  m_myIndex = index;

  m_top.resize(3, 1, 1, 20, 6);
  
  cout << "Set up w/ " << n << endl;
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
  m_score = s;
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
  IOEntity ent;
  ent.resize(3, 1, 1);
  ent.in(0) = in; 
  ent.in(1) = prop[0]; 
  ent.in(2) = prop[1]; 
  m_top.Update(ent, 0.6, m_score);
}
